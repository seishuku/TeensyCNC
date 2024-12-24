#ifndef __PWM_H__
#define __PWM_H__

#ifndef PWM_MAX_RATIO
#define PWM_MAX_RATIO (1024-1)
#endif

void PWM_SetRatio(uint8_t Channel, uint16_t Ratio);
void PWM_Init(void);

#endif
