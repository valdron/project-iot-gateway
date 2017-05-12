#include <open62541.h>
#include <stdio.h>
#include "state.h"

// State:
// 1 Byte 
// Bit 0: 1 = on; 0 = off
// Bit 1: 1 = in bewegung; 0 = still
// Bit 2: 1 = working; 0 = waiting
// Bit 3: 1 = fehler; 0 = kein fehler

// reserved
// Bit 4: 1 = switching aufsatz; 0 = off
// Bit 5: 1 = on; 0 = off
// Bit 6: 1 = on; 0 = off
// Bit 7: 1 = on; 0 = off

UA_DateTimeStruct getCurrentTimestruct() {
    return UA_DateTime_toStruct( UA_DateTime_now());
}

UA_Byte getState(UA_DateTimeStruct dt) {

    //off
    if(dt.hour > 21 || dt.hour < 6)
        return 0b00000000;
    //Moving
    if(dt.sec % 6 == 0)
        return 0b00000111;
    //Nieten
    if(dt.sec % 6 == 1 && dt.milliSec < 500)
        return 0b00000101;
    //Moving + switching
    if(dt.sec % 6 < 3)
        return 0b00010111;
    //Bohren
    if(dt.sec % 6 == 3 && dt.milliSec < 500)
        return 0b00000101;
    //switching to schrauber
    if(dt.sec % 6 < 4)
        return 0b00010101;
    //schreuben
    if(dt.sec % 6 == 4 && dt.milliSec < 500)
        return 0b00000101;
    //moving back + switching
    if(dt.sec % 6 < 5)
        return 0b00010111;

    return 0b00000001;
}



