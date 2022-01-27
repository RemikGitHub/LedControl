#ifndef _KEYS_FRAME
#define _KEYS_FRAME

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

void GPIO_Keys_Conf(void);
void FM_TIM2_TIMER_Pre(uint16_t prescaler, uint16_t periode);
void FM_TIM2_NVIC_Pre(void);

#endif
