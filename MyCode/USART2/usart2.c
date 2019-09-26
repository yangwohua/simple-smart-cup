#include "sys.h"
#include "usart2.h"
#include "beep.h"

u8 USART2_RX_BUF[USART2_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
u16 USART2_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void usart2_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructurea;
	  USART_InitTypeDef USART_InitStruct;
	 
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ�ܴ���2
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOB
	  USART_DeInit(USART2);//����3��λ ���Ǳ�Ҫ��һ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);    //PA2����Ϊ�����������
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);  //PA3����Ϊ��������	 
  
	  NVIC_InitStructurea.NVIC_IRQChannel=USART2_IRQn;
	  NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructurea.NVIC_IRQChannelSubPriority=2;
	  NVIC_Init(&NVIC_InitStructurea);          //����2�жϵ�����
		
	  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//����2�ж�ʹ�ܺ��ж���������
	
	  USART_InitStruct.USART_BaudRate=bound;//���ڲ�����
	  USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
	  USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Rx;//�շ�ģʽ
	  USART_InitStruct.USART_Parity=USART_Parity_No;//����żУ��λ
	  USART_InitStruct.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	  USART_InitStruct.USART_WordLength=USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	  USART_Init(USART2,&USART_InitStruct);                 //����2��һЩ��������
		
	  USART_Cmd(USART2,ENABLE);//����2ʹ��
	
	  
}
void USART2_IRQHandler()//����3���ж�
{
//	u8 res;//��ʱ������յ�����
//   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����3�����ж�
//		{
//			res=USART_ReceiveData(USART2);//���յ����ݷŽ�res
//			USART2_RX_BUF[USART2_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
//			USART2_RX_STA++;                         //�ֽڳ���++
//			if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
//			USART2_RX_STA|=0x8000;//����2�������
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);//��������жϱ�־   
//		}
	
		u8 res;//��ʱ������յ�����
		if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)//�ж��Ƿ�Ϊ����2�����ж�
			{
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);//��������жϱ�־ 
				res = USART_ReceiveData(USART2);//���յ����ݷŽ�res
				
				//�˴����ڵ·������������Զ˴��ڻ���
				
				if(res=='0')//���յ�1������LED
				{
					GPIO_SetBits(BEEP_PORT, BEEP_PIN);
				}
				if(res=='1')//�������Ϩ��LED
				{
					GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
				}		
				USART2_RX_STA=0;
		
			}
}






