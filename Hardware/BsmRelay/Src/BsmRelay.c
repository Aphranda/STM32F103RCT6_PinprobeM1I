#include "BsmRelay.h"

extern uint8_t* usart3_buff_IsReady;
extern uint8_t* usart3_buff_Occupied;


uint8_t Cylinder_Write(uint32_t cylinder_id, scpi_choice_def_t cylinder_value)
{
    switch (cylinder_id)
    {
    case 1:
        WriteIO(2,cylinder_value.tag);
        break;
    case 2:
        WriteIO(3,cylinder_value.tag);
        break;
    default:
        break;
    }
    return 0;
}

uint8_t Lock_Write(scpi_choice_def_t lock_value)
{
    switch (lock_value.tag)
    {
    case 0:
        WriteIO(7,1);
        WriteIO(8,1);
        break;
    case 1:
        WriteIO(7,0);
        WriteIO(8,0);
        break;
    default:
        break;
    }
    return 0;
}

uint8_t LED_Write(scpi_choice_def_t led_value)
{
    switch (led_value.tag)
    {
    case 0: // led OFF
        WriteIO(4,0);
        WriteIO(5,0);
        WriteIO(6,0);
        break;
    case 1: // led G
        WriteIO(4,1);
        WriteIO(5,0);
        WriteIO(6,0);
        break;
    case 2: // led R
        WriteIO(4, 0);
        WriteIO(5, 1);
        WriteIO(6, 0);
        break;
    case 3: // led Y
        WriteIO(4, 0);
        WriteIO(5, 0);
        WriteIO(6, 1);
        break;
    default:
        break;
    }
    return 0;
}

uint8_t IO_Read(uint8_t checkNum, uint8_t direction, uint8_t* trueData){
    uint8_t State_count = 0;
    while (State_count < checkNum)
    {   
        // Read bsm input IO status
        uint8_t* data;
        ReadIO(direction, data);

        uint8_t crcData[2];
        uint8_t IOData[5];

        memcpy(IOData, usart3_buff_IsReady, 5);
        crcData[0] = usart3_buff_IsReady[5];
        crcData[1] = usart3_buff_IsReady[6];

        if(modbus_crc_compare(5, IOData, crcData))
        {
            trueData[0] = IOData[3];
            trueData[1] = IOData[4];
            return 0;
        }
    }
    return 0; 
}

uint8_t* inputIO_Read(uint8_t checkNum){
    uint8_t* data;
    IO_Read(checkNum, 2, data);
    return data;
}

uint8_t* OutputIO_Read(uint8_t checkNum){
    uint8_t* data;
    IO_Read(checkNum, 1, data);
    return data;
}