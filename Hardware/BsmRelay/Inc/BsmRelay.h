#ifndef __BSMRELAY_H
#define __BSMRELAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "scpi-def.h"
#include "RS485.h"

uint8_t Cylinder_Write(uint32_t cylinder_id, scpi_choice_def_t cylinder_value);

uint8_t Lock_Write(scpi_choice_def_t lock_value);

uint8_t LED_Write(scpi_choice_def_t led_value);

uint8_t IO_Read(uint8_t checkNum, uint8_t direction, uint8_t* trueData);

uint8_t* InputIO_Read(uint8_t checkNum);

uint8_t* OutputIO_Read(uint8_t checkNum);

#endif // !__BSMRELAY_H
