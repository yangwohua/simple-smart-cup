#ifndef USART2_H_
#define USART2_H_
#include "sys.h"
#include "stdio.h"	
#define USART2_REC_LEN  200  	//�����������ֽ��� 200
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u16 USART2_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void usart2_init(u32 bound);
#endif
