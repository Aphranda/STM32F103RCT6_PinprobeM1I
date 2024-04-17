#include "StateMachine.h"
#include "BsmRelay.h"

uint16_t lock_num;
uint16_t door_open_num;
uint16_t door_close_num;
uint16_t door_execution_num=0;
uint8_t system_status;

uint8_t StateMachine_Input()
{
    Lock_Action();
    HAL_Delay(100);
    return 0;
}

uint8_t Lock_Action()
{
    uint8_t* I_status = InputIO_Read(CHECK_NUM);
    uint8_t in_01_08 = I_status[0];
    uint8_t in_09_16 = I_status[1];
    uint8_t* O_status = OutputIO_Read(CHECK_NUM);
    uint8_t out_01_08 = O_status[0];
    uint8_t out_09_16 = O_status[1];

    uint8_t lock_ready_status = 0;
    if(in_09_16&(power_button>>8)) // power button press
    {
        lock_num ++;
        if(lock_num>=5) // power button confirm
        {
            lock_ready_status = 1;
        }
    }



    if((in_09_16&(power_button>>8)) && lock_ready_status == 1) // power button drive power out
    {

        if(out_01_08&power_out)
        {
            usart1_buff_IsReady = "CONFigure:LOCK ON\r\n"; // system lock on
            system_status = 0;
        }
        else
        {
            usart1_buff_IsReady = "CONFigure:LOCK OFF\r\n"; // system lock off
            system_status = 1;
        }
        lock_ready_status = 0;
        lock_num = 0;
    }
    return 0;
}


uint8_t Ready_Action()
{
    uint8_t* O_status = OutputIO_Read(CHECK_NUM);
    uint8_t out_01_08 = O_status[0];
    uint8_t out_09_16 = O_status[1];
    uint8_t* I_status = InputIO_Read(CHECK_NUM);
    uint8_t in_01_08 = I_status[0];
    uint8_t in_09_16 = I_status[1];

    uint8_t door_ready_status = 0;

    // ready action door
    if((in_09_16&(door_button1<<8))&&(in_09_16&(door_button2<<8)))
    {
        door_open_num ++;
        if(door_open_num >= 5)
        {
            door_ready_status = 1;
        }
        if(door_execution_num !=0)
        {
            door_execution_num--;
        }
    }

    // if system is Idle, map to ready
    if(system_status == 0)
    {
        if(((in_09_16&(door_button1<<8))||(in_09_16&(door_button2<<8)))&& (door_ready_status ==1)&&(door_execution_num<=0))
        {
            if(!(out_01_08&door_sensor_up))
            {
                usart1_buff_IsReady = "CONFigure:CYLInder1 OPEN\r\n";
                door_execution_num = 20;
            }
            door_open_num = 0;
            door_ready_status = 0;
        }

        if((in_09_16&(door_button1<<8))||(in_09_16&(door_button2<<8))&&(door_ready_status == 1) && (door_execution_num<=0))
        {
            if(!(out_01_08&door_sensor_up))
            {
                usart1_buff_IsReady = "CONFigure:LED YELLOW\r\n";
                door_execution_num = 20;
            }
            door_open_num = 0;
            door_ready_status = 0;
        }

        if((in_09_16 & 0x06 == 0x06)&&(door_ready_status == 1)&&(door_execution_num>0))
        {
            door_close_num ++;
        }
        if(door_close_num>20)
        {
            door_execution_num = 20;
            door_close_num = 0;
            door_ready_status = 0;
        }

    }


    return 0;
}

uint8_t Running_Action()
{
    InputIO_Read(CHECK_NUM);
    return 0;
}

uint8_t Emerge_Action()
{
    InputIO_Read(CHECK_NUM);
    return 0;
}