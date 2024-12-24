// WIP ESP32 TeensyCNC
// This main is just for testing motor control.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "pwm.h"
#include "motor.h"

extern volatile int32_t Target[2];
extern volatile int32_t EncoderPos[2];

void PID_callback(void);

void app_main(void)
{
    PWM_Init();
    Motor_Init();
    MotorEnable();

    bool dir=false;

    while(1)
    {
        ESP_EARLY_LOGI("TeensyCNC", "Target X: %d Encoder X: %d", Target[0], EncoderPos[0]);

        if(dir)
            Target[0]-=10;
        else
            Target[0]+=10;

        if(Target[0]>1000)
            dir=true;
        else if(Target[0]<-1000)
            dir=false;

        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}
