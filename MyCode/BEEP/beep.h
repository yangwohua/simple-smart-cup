#ifndef _beep_H
#define _beep_H

#include "sys.h"

/*  蜂鸣器时钟端口、引脚定义 */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_8
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB

#define beep PBout(8)

void BEEP_Init(void);

#endif
