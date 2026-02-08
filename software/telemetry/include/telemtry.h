#include <stdio.h>
enum state {
    BOOT, ARMED, DRILLING, FAULT, SHUTDOWN
};

struct drillVals {
    char* team = "SmithEngineeringHyperloop"; 
    float time_unix; 
    bool mining = false; 
    float elevation; 
    float roll;
    float pitch;
};