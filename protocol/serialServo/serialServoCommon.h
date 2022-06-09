#ifndef serialServoCommon
#define serialServoCommon

#include <stdint.h>

static float countToDegreesValue = 180 / 1024;
static float degreesToCountValue = 1024 / 180;


double serialServoCountToDegrees(uint16_t count);
uint16_t serialServoDegreesToCount(double degrees);





#endif