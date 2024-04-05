#pragma once

#if ESP32_17320S019N
#define oneWirePin    21
#define PressureAdcPin  4
#define actuatorPin1  5
#define actuatorPin2  6
#define actuatorPin3  7
#define actuatorPin4  15
#define actuatorPin5  16
#define actuatorPin6  17
#endif

#if ESP32_2432S032C
#define oneWirePin    22
#define PressureAdcPin  35
#define actuatorPin1  21
#endif

#if WT32SC01PLUS
#define oneWirePin    11
#define PressureAdcPin  10
#define actuatorPin1  12
#define actuatorPin2  13
#define actuatorPin3  14
#define actuatorPin4  21
#endif
