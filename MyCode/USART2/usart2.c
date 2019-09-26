#include "sys.h"
#include "usart2.h"
#include "beep.h"

u8 USART2_RX_BUF[USART2_REC_LEN]; //数组用来存储接收到的数据，而USART3_REC_LEN为最多能接收的字节限度
u16 USART2_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
void usart2_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructurea;
	  USART_InitTypeDef USART_InitStruct;
	 
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能串口2
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOB
	  USART_DeInit(USART2);//串口3复位 不是必要的一步
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);    //PA2设置为复用推挽输出
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);  //PA3设置为浮空输入	 
  
	  NVIC_InitStructurea.NVIC_IRQChannel=USART2_IRQn;
	  NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructurea.NVIC_IRQChannelSubPriority=2;
	  NVIC_Init(&NVIC_InitStructurea);          //串口2中断的设置
		
	  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//串口2中断使能和中断类型设置
	
	  USART_InitStruct.USART_BaudRate=bound;//串口波特率
	  USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	  USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Rx;//收发模式
	  USART_InitStruct.USART_Parity=USART_Parity_No;//无奇偶校验位
	  USART_InitStruct.USART_StopBits=USART_StopBits_1;//一个停止位
	  USART_InitStruct.USART_WordLength=USART_WordLength_8b;//字长为8位数据格式
	  USART_Init(USART2,&USART_InitStruct);                 //串口2的一些参数设置
		
	  USART_Cmd(USART2,ENABLE);//串口2使能
	
	  
}
void USART2_IRQHandler()//串口3的中断
{
//	u8 res;//暂时缓存接收的数据
//   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//判断是否为串口3接收中断
//		{
//			res=USART_ReceiveData(USART2);//接收到数据放进res
//			USART2_RX_BUF[USART2_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
//			USART2_RX_STA++;                         //字节长度++
//			if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	+; 
//			USART2_RX_STA|=0x8000;//串口2接收完成
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除接收中断标志   
//		}
	
		u8 res;//暂时缓存接收的数据
		if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)//判断是否为串口2接收中断
			{
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除接收中断标志 
				res = USART_ReceiveData(USART2);//接收到数据放进res
				
				//此处基于德飞莱不能做电脑端串口回显
				
				if(res=='0')//接收到1，点亮LED
				{
					GPIO_SetBits(BEEP_PORT, BEEP_PIN);
				}
				if(res=='1')//其他情况熄灭LED
				{
					GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
				}		
				USART2_RX_STA=0;
		
			}
}






