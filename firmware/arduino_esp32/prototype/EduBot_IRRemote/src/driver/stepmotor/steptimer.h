#ifndef _STEPTIMER_H_
#define _STEPTIMER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "esp32-hal.h"
#include "freertos/FreeRTOS.h"

struct hw_timer_s;
typedef struct hw_timer_s hw_timer_t;


void steptimerAlarmWrite(hw_timer_t *timer, uint64_t interruptAt, bool autoreload);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_ESP32_HAL_TIMER_H_ */