#include "StateMachine.h"
#include "BsmRelay.h"
#include "cmsis_os.h"

#define Lock_Delay_num 3
#define Door_Delay_num 10
#define Debug

extern scpi_choice_def_t cylinder_source[];
extern scpi_choice_def_t lock_source[];
extern scpi_choice_def_t led_source[];

uint16_t lock_num = 0;
uint16_t lock_idle = Lock_Delay_num;

uint16_t door_ready_num = 0;
uint16_t door_open_num = 0;
uint16_t door_close_num = 0;
uint16_t Release_flag = 0;
uint8_t system_status = SYS_ERR;
uint8_t system_old_status;

uint8_t StateMachine_Input()
{

    uint8_t* I_status = InputIO_Read(CHECK_NUM);
    uint8_t in_01_08 = I_status[0];
    uint8_t in_09_16 = I_status[1];
    uint8_t* O_status = OutputIO_Read(CHECK_NUM);
    uint8_t out_01_08 = O_status[0];
    uint8_t out_09_16 = O_status[1];

    system_old_status = system_status;
    
    Lock_Action(in_01_08, in_09_16, out_01_08, out_09_16);
    Idle_Action(in_01_08, in_09_16, out_01_08, out_09_16);
    Ready_Action(in_01_08, in_09_16, out_01_08, out_09_16);
    Running_Action(in_01_08, in_09_16, out_01_08, out_09_16);
    Emerge_Action(in_01_08, in_09_16, out_01_08, out_09_16);
    Release_detection(in_01_08, in_09_16, out_01_08, out_09_16);
    osDelay(100);
    
    if(system_status != system_old_status)
    {
        showStatus();
        system_old_status = system_status;
    }
    return 0;
}

uint8_t Lock_Action(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if(system_status == SYS_ERR)
    {
        if(out_01_08&power_out)
        {
            system_status = Idle;
        }
    }else if(system_status == Lock)
    {
        if((out_01_08&power_out))
        {
            system_status = Idle;
        }
    }

    uint8_t lock_ready_status = 0;
    if(in_09_16&(power_button>>8)) // power button press
    {
        lock_num ++;
        if(lock_num>=Lock_Delay_num) // power button confirm
        {
            lock_ready_status = 1;
        }
    }
    else{
        if(lock_idle>0)             // power button idle
        {
            lock_idle --;
        }
    }

    if((in_09_16&(power_button>>8)) && (lock_ready_status == 1)&&(lock_idle <= 0)) // power button must be idle, jump action.
    {

        if(out_01_08&power_out)
        {
            Lock_Write(lock_source[1]); // system lock on lock to lock
            system_status = Lock;
        }
        else
        {
            Lock_Write(lock_source[0]); // system lock off lock to idle
            LED_Write(led_source[0]);
            system_status = Idle;
        }
        lock_ready_status = 0;
        lock_num = 0;
        lock_idle = Lock_Delay_num;
    }
    return 0;
}

uint8_t Idle_Action(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if(system_status == Idle)
    {
        if(!(out_01_08&power_out))
        {
            system_status = Lock;
        }
        else if (out_01_08&led_yellow)
        {
            system_status = Ready;
        }
        // ready action door
        if((in_09_16&(door_button1>>8))||(in_09_16&(door_button2>>8)))
        {
            if(!(out_01_08&door_close)&&(in_01_08&door_sensor_up)) //TODO: add sender check
            {
                door_ready_num ++;
            }
        }

        if((door_ready_num >= Door_Delay_num)&&(Release_flag<=0))                   // door action ready num
        {
            LED_Write(led_source[3]);
            system_status = Ready;                             // door action ready
        }
    }
    return 0;
}

uint8_t Ready_Action(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if(system_status == Ready)
    {
        if(!(out_01_08&led_yellow))
        {
            system_status = Idle;
        }
        uint8_t door_ready_status = 0;
        // ready action door
        if((in_09_16&(door_button1>>8))||(in_09_16&(door_button2>>8)))
        {
            if(!(out_01_08&door_close)&&(in_01_08&door_sensor_up)&&(Release_flag<=0)) //TODO: add sensor check
            {
                door_close_num ++;
            }
            if(door_close_num >= Door_Delay_num*2)                   // door action read num
            {
                door_ready_status = 1;                              // door action ready
            }
        }
        // if system is Idle, map to ready
        if(((in_09_16&(door_button1>>8))&&(in_09_16&(door_button2>>8)))&&(door_ready_status ==1)&&(Release_flag<=0))
        {
            if(!(out_01_08&door_close)&&(in_01_08&door_sensor_up)) // TODO: add door sensor check
            {
                Cylinder_Write(1, cylinder_source[0]); // door closing
                door_close_num = 0;
                door_ready_status = 0;
                Release_flag = Door_Delay_num;
                system_status = Running;
            }
        }
    }
    return 0;
}

uint8_t Running_Action(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if(system_status == Running)
    {
        if((in_01_08&door_sensor_up)&&(out_01_08&door_open))
        {
            system_status == Ready;
        }

        if((in_01_08&door_sensor_down)&&(out_01_08&door_close))
        {
            system_status == Complete;
        }

        uint8_t door_idle_status = 0;
        // ready action door
        if((in_09_16&(door_button1>>8))||(in_09_16&(door_button2>>8)))
        {
            if(!(out_01_08&door_open)&&(in_01_08&door_sensor_down)) //open door
            {
                door_open_num ++;
            }
            if(door_open_num >= Door_Delay_num/2)                   // door action read num
            {
                door_idle_status = 1;                              // door action ready
            }
        }
        if(((in_09_16&(door_button1>>8))||(in_09_16&(door_button2>>8)))&& (door_idle_status ==1)&&(Release_flag<=0))
        {
            if(!(out_01_08&door_open))
            {
                Cylinder_Write(1, cylinder_source[1]);  // door opening
                LED_Write(led_source[0]);
                door_open_num = 0;
                door_idle_status = 0;
                Release_flag = Door_Delay_num;
                system_status = Idle;
            }
        }
    return 0;
    }
}

uint8_t Emerge_Action(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if(system_status == Emergency)
    {
        if((in_01_08&laser_sensor1)&&(in_01_08&laser_sensor2)&&(in_01_08&laser_sensor3)&&(in_09_16&(laser_sensor4>>8))&&(out_01_08&door_open)&&((in_09_16&(stop_button>>8))))
        {
            system_status = Lock;
            LED_Write(led_source[0]);
        }
    }

    if((in_09_16&(stop_button>>8))!=0x08)
    {
        system_status = Emergency;
        Cylinder_Write(1,cylinder_source[1]);
        Lock_Write(lock_source[1]);
        LED_Write(led_source[2]);
    }
    else if(((in_01_08&laser_sensor1)!=0x20)||((in_01_08&laser_sensor2)!=0x40)||((in_01_08&laser_sensor3)!=0x80)||((in_09_16&(laser_sensor4>>8)!=0x01)))
    {
        system_status = Emergency;
        Cylinder_Write(1,cylinder_source[1]);
        Lock_Write(lock_source[1]);
        LED_Write(led_source[2]);
    }
    return 0;
}

uint8_t Release_detection(uint8_t in_01_08, uint8_t in_09_16, uint8_t out_01_08, uint8_t out_09_16)
{
    if((!(in_09_16&(door_button1>>8)))&&(!(in_09_16&(door_button2>>8))))
    {
        if(Release_flag>0)
        {
            Release_flag --;
        }
    }
}

uint8_t showStatus()
{
    #ifdef Debug
    switch (system_status)
    {
    case 0:
        U1_Printf("LOCK\r\n");
        break;
    case 1:
        U1_Printf("IDLE\r\n");
        break;
    case 2:
        U1_Printf("READY\r\n");
        break;
    case 3:
        U1_Printf("RUNNING\r\n");
        break;
    case 4:
        U1_Printf("EMERGE\r\n");
        break;
    default:
        break;
    }
    #endif // DEBUG
    return 0;
}