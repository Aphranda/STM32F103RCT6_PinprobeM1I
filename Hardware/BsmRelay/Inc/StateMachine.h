#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef enum {
    Unlocked = 0,   // Machine is unlocked
    Lock = 1,       // Machine is locked

    StartTest = 2,  // Machine Hardware Start
    EndTest = 3,    // Machine Hardware End
    Testing = 4,    // Machine Software Testing

    Ready = 5,      // Machine Hardware Ready
    Error = 6,      // Machine Hardware Error
    Idle = 7,       // Machine Hardware Idle
    Emerge = 8,     // Machine Hardware Merge
} MachineState;



typedef enum {
    twinkle = 0,        // Machine LED twinkle
    Green = 1,          // Machine LED Green
    Red = 2,            // Machine LED Red
    Yellow = 3,         // Machine LED Yellow
    Led_OFF = 4,        // Machine LED OFF
} LEDState;


typedef enum {
    PressDown = 0,      // Machine is normal press
    PressUp = 1,        // Machine is Error and Stop
} EStopButtonState;

typedef enum {
    PressFall = 0,      // Machine is single Falling press
    PressRise = 1,      // Machine is single Rising press
} StartButtonState;

typedef enum {
    Trigger = 0,        // Machine is unsafe trigger
    UnTriggered = 1,    // Machine is safe
} SafeSensorState;

typedef enum {
    Up = 0,             // Cylinder is UP
    Down = 1,            // Cylinder is Mid          // Cylinder is Down
} CylinderState;

typedef struct Action_Map {
    MachineState Machine;
    LEDState led;
    EStopButtonState eStopButton;
    StartButtonState startButton;
    SafeSensorState  safeSensor;
    CylinderState cylinder;
} MachineAction;

uint8_t StateMachine_Input();
uint8_t Lock_Action();
uint8_t Idle_Action();
uint8_t Ready_Action();
uint8_t Running_Action();
uint8_t Emerge_Action();
#endif /* __STATEMACHINE_H */