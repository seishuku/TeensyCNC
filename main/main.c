// WIP ESP32 TeensyCNC
// This main is just for testing PWM outputs.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pwm.h"
#include "motor.h"

extern volatile int32_t Target[2];

void app_main(void)
{
    PWM_Init();
    Motor_Init();
    
    uint32_t a=0, b=0, c=0, d=0;

    while(1)
    {
        PWM_SetRatio(0, a);
        PWM_SetRatio(1, b);
        PWM_SetRatio(2, c);
        PWM_SetRatio(3, d);

        if(a<512)
            a++;
        else if(b<512)
            b++;
        else if(c<512)
            c++;
        else if(d<512)
            d++;
        else
            a=b=c=d=0;

        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}
