/************************************************************************************
						
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"
#include "time.h"
#include "oled.h"
#include "bmp.h"
#include "ds18b20.h"
#include "beep.h"
#include "sys.h"
#include "usart2.h"

float temper;
float water, water1, water2, chazhi;
u16 i = 0;
u8 j = 0;

int main(void)
{	
    uart_init(9600);	 //串口初始化为9600
    usart2_init(9600);
//      printf("ok!!!");
    delay_init();
    BEEP_Init();
    GPIO_SetBits(BEEP_PORT, BEEP_PIN);
    TIM3_Init(10000, 7200-1); 
    TIM4_Init(10000, 7200-1);
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    Init_HX711pin();
    OLED_Init();			//初始化OLED  
    OLED_Clear();
    //while (DS18B20_Init());
    DS18B20_Init();
    OLED_ShowCHinese(0,0,0); //剩
    OLED_ShowCHinese(18,0,1);//余
    OLED_ShowCHinese(36,0,7);//的
    OLED_ShowCHinese(54,0,2);//水
    OLED_ShowCHinese(72,0,3);//量
    OLED_ShowCHinese(90,0,10);//为
    OLED_ShowCHinese(36,2,14);OLED_ShowCHinese(54,2,15);//毫升
    
    OLED_ShowCHinese(0,4,5);//检    
    OLED_ShowCHinese(18,4,6);//测    
    OLED_ShowCHinese(36,4,7);//的    
    OLED_ShowCHinese(54,4,8);//温    
    OLED_ShowCHinese(72,4,9);//度    
    OLED_ShowCHinese(90,4,10);//为 
    
    Get_Maopi();				//称毛皮重量
    delay_ms(1000);
    Get_Maopi();				//重新获取毛皮重量 

    while(1);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))
	{
        water1 = water; 
        Get_Weight();
        Weight_Shiwu = Weight_Shiwu * 0.289;    //水杯净重165g
        water = Weight_Shiwu - 165;        
        OLED_ShowNum(6,2,water,3,16);//显示ASCII字符的码值
        temper = DS18B20_GetTemperture();        
        OLED_ShowNum(10,6,temper,3,16);//显示ASCII字符的码值
        water2 = water;
        chazhi = water2 - water1;
        if (chazhi > 100)
        {
            i = 0;
            beep = 0;
            delay_ms(50);
            beep = 1;
            delay_ms(50);
        }
        if (Weight_Shiwu < 100)
        {
            i = 0;
        }
          
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	

    //显示差值
    //OLED_ShowNum(6,2,chazhi,3,16);//显示ASCII字符的码值
}


/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM4,TIM_IT_Update))
    {
        i++;
        if(i == 5)
        {
           
            for(j=0; j<10; j++)
            {
                beep=0;
                delay_ms(50);
                beep=1;
                delay_ms(50);
            }
            i = 0;
        }
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}



