/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser test
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scpi/scpi.h"
#include "scpi-def.h"
#include "scpi_switch.h"
#include "BsmRelay.h"

static scpi_result_t SCPI_ConfigureSwitch(scpi_t *context)
{
    int32_t number[2] = 0;
    uint16_t switch_id = 1;
    SCPI_CommandNumbers(context, number, 1, 1);
    if(number[0] != 1)
    {
        switch_id = number[0];
    }
    uint32_t switch_value = 0;
    if(!SCPI_ParamUInt32(context, &switch_value, true))
    {
        return SCPI_RES_ERR;
    }
    if(Link_Write(switch_id, switch_value))
    {
        return SCPI_RES_ERR;
    }
    return SCPI_RES_OK;
}

static scpi_result_t SCPI_ReadSwitchState(scpi_t *context)
{
    int32_t number[2] = {0};
    uint16_t switch_id = 1;

    SCPI_CommandNumbers(context, number,1,1);
    if(number[0] !=1)
    {
        switch_id = number[0];
    }
    uint32_t mask = 0;
    Link_Read(switch_id, &mask);
    SCPI_ResultInt32(context, mask);
    return SCPI_RES_OK;
}

scpi_choice_def_t cylinder_source[] = {
    {"CLOSE", 0},
    {"OPEN", 1},
    {"CLOSING",2},
    {"OPENING",3},
    {"CLOSED",4},
    {"OPENED",5},
    {"CYL ERR",6},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

static scpi_result_t SCPI_ConfigureCylinder(scpi_t *context)
{
    int32_t number[2] = 0;
    uint16_t cylinder_id = 1;
    SCPI_CommandNumbers(context, number, 1, 1);
    if(number[0] != 1)
    {
        cylinder_id = number[0];
    }
    int32_t param;
    const char *name;
    if (!SCPI_ParamChoice(context, cylinder_source, &param, TRUE)) {
        return SCPI_RES_ERR;
    }
    SCPI_ChoiceToName(cylinder_source, param, &name);
    if(Cylinder_Write(cylinder_id, cylinder_source[param]))
    {
        return SCPI_RES_ERR;
    }
    return SCPI_RES_OK;
}

static scpi_result_t SCPI_ReadCylinderState(scpi_t *context)
{
    const char *name = "CYL ERR";
    scpi_choice_def_t status =Cylinder_Status(1);
    name = status.name;
    SCPI_ResultCharacters(context, name, strlen(name));
}

scpi_choice_def_t lock_source[] = {
    {"OFF", 0},
    {"ON", 1},
    {"LOCK ERR",2},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

static scpi_result_t SCPI_ConfigureLOCK(scpi_t *context)
{
    int32_t param;
    const char *name;
    if (!SCPI_ParamChoice(context, lock_source, &param, TRUE)) {
        return SCPI_RES_ERR;
    }
    SCPI_ChoiceToName(lock_source, param, &name);
    if(Lock_Write(lock_source[param]))
    {
        return SCPI_RES_ERR;
    }
    return SCPI_RES_OK;
}

static scpi_result_t SCPI_ReadLOCKState(scpi_t *context)
{
    const char *name = "LOCK ERR";
    scpi_choice_def_t status =Lock_Status();
    name = status.name;
    SCPI_ResultCharacters(context, name, strlen(name));
}

scpi_choice_def_t led_source[] = {
    {"OFF", 0},
    {"GREEN", 1},
    {"RED", 2},
    {"YELLOW", 3},
    {"LED ERR", 4},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

static scpi_result_t SCPI_ConfigureLED(scpi_t *context)
{
    int32_t param;
    const char *name;
    if (!SCPI_ParamChoice(context, led_source, &param, TRUE)) {
        return SCPI_RES_ERR;
    }
    SCPI_ChoiceToName(led_source, param, &name);
    if(LED_Write(led_source[param]))
    {
        return SCPI_RES_ERR;
    }
    return SCPI_RES_OK;
}

static scpi_result_t SCPI_ReadLEDState(scpi_t *context)
{
    const char *name = "LED ERR";
    scpi_choice_def_t status =LED_Status();
    name = status.name;
    SCPI_ResultCharacters(context, name, strlen(name));
}

scpi_choice_def_t sys_source[] = {
    {"Lock", 0},
    {"Idle", 1},
    {"Ready", 2},
    {"Running", 3},
    {"Emergency", 4},
    {"Complete",5},
    {"SYS ERR",6},
    SCPI_CHOICE_LIST_END /* termination of option list */
};
static scpi_result_t SCPI_ReadSystemState(scpi_t *context)
{
    const char *name = "SYS ERR";
    scpi_choice_def_t status =SYS_Status();
    name = status.name;
    SCPI_ResultCharacters(context, name, strlen(name));
}

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    {
        .pattern = "*CLS",
        .callback = SCPI_CoreCls,
    },
    {
        .pattern = "*IDN?",
        .callback = SCPI_CoreIdnQ,
    },
    {
        .pattern = "*RST",
        .callback = SCPI_CoreRst,
    },
    {
        .pattern = "*STB?",
        .callback = SCPI_CoreStbQ,
    },
    {
        .pattern = "*WAI",
        .callback = SCPI_CoreWai,
    },
    {
        .pattern = "*OPC?",
        .callback = SCPI_CoreOpcQ,
    },
    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {
        .pattern = "SYSTem:ERRor[:NEXT]?",
        .callback = SCPI_SystemErrorNextQ,
    },
    {
        .pattern = "SYSTem:ERRor:COUNt?",
        .callback = SCPI_SystemErrorCountQ,
    },
    {
        .pattern = "CONFigure:SWITch#",
        .callback = SCPI_ConfigureSwitch,
    },
    {
        .pattern = "READ:SWITch#:STATe?",
        .callback = SCPI_ReadSwitchState,
    },
    {
        .pattern = "CONFigure:CYLInder#",
        .callback = SCPI_ConfigureCylinder,
    },
    {
        .pattern = "READ:CYLInder#:STATe?",
        .callback = SCPI_ReadCylinderState,
    },
    {
        .pattern = "CONFigure:LOCK",
        .callback = SCPI_ConfigureLOCK,
    },
    {
        .pattern = "READ:LOCK:STATe?",
        .callback = SCPI_ReadLOCKState,
    },
    {
        .pattern = "CONFigure:LED",
        .callback = SCPI_ConfigureLED,
    },
    {
        .pattern = "READ:LED:STATe?",
        .callback = SCPI_ReadLEDState,
    },
    {
        .pattern = "READ:SYSTem:STATe?",
        .callback = SCPI_ReadSystemState,
    },
    SCPI_CMD_LIST_END};

scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;
