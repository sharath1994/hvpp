
#ifndef __PLATFORM_H_43f49d920a3444f8b23bd0b8872d020c
#define __PLATFORM_H_43f49d920a3444f8b23bd0b8872d020c 1

typedef uint16_t mytime_t;

#define TIMING_MS_PER_TICK 4

#define TOSYSTEMTIME(ms) (ms/(TIMING_MS_PER_TICK))

#endif
