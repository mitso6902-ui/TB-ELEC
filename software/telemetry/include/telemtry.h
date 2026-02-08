#include <stdio.h>
#include <stdbool.h>
enum state {
    BOOT, ARMED, DRILLING, FAULT, SHUTDOWN
};

struct drillVals {
    char* team;
    float time_unix; 
    bool mining; 
    float elevation; 
    float roll;
    float pitch;
}; 