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
    Stop = 7,       // Machine Hardware Stop
    Emerge = 8,     // Machine Hardware Merge

} MachineState;

typedef enum {
    RF1 = 0, //
    RF2 = 1, //
    RF3 = 2, //
    RF4 = 3, //
    RF5 = 4, //
    RF6 = 5, //
    RF7 = 6, //
    RF8 = 7, //
    RF9 = 8, //
    RF10 = 9, //
    RF11 = 10, //
    RF12 = 11, //
    RF13 = 12, //
    RF14 = 13, //
    RF15 = 14, //
    RF16 = 15 //
} RFState;

typedef enum {
    twinkle = 0,        // Machine LED twinkle
    Greed = 1,          // Machine LED Green
    Red = 2,            // Machine LED Red
    Yellow = 3,         // Machine LED Yellow
    Led_OFF = 4,        // Machine LED OFF
} LEDState;


typedef enum {
    buttonUnlocked = 0,       // Machine is unlocked
    buttonLock = 1,           // Machine is unlocked
} LockButtonState;

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
    RFState rfSwitch;
    LockButtonState lockButton;
    EStopButtonState eStopButton;
    StartButtonState startButton;
    SafeSensorState  safeSensor;
    CylinderState cylinder;
} MachineAction;

#endif /* __STATEMACHINE_H */