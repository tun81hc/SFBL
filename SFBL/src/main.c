/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "main.h"
#include "ymodem.h"
#include "flash_if.h"
#include "menu.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval.c"
#include "common.h"
#include "KeyMng.h"
#include "cmac.h"
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
static void IAP_Init(void);
Bootloader_State BL_State = Init;

/*2b*/
int main(void)
{
	IAP_Init();
	GPIO();
	SerialPutString("Booting.................................................\r\n\n");
	FLASH_If_Init();
	/*while (1){*/
	for(int i = 0; i< 10000000; i++){}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
	//*if(1)
	{
		Security_Access();
		Main_Menu ();
	}
	else
	{
		uint8_t Key[16],RefMACApp[16];
		KeyMng_ReadKey(3,(uint32_t*) Key);
		SerialPutString("Calculate CMAC of application--------------------------- \r\n\n");

		for(uint8_t i=0;i<16;i++)
		{
			RefMACApp[i]= *(uint8_t*)(0x8008000+0x01*i);
		}
		if (Verify_MAC(Key, (unsigned char *)0x08008010, 5488,RefMACApp) == 1)
		{
			SerialPutString("Verify Application: PASS-------------------------------- \r\n\n");
			SerialPutString("Execute the new program -------------------------------- \r\n\n");
			USART_DeInit(USART1);
			GPIO_DeInit(GPIOA);
			SCB->VTOR = APPLICATION_ADDRESS+0x10;
					__disable_irq();
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4+0x10);
			//Jump to user application
			Jump_To_Application = (pFunction) ((uint32_t)JumpAddress);
			//Initialize user application's Stack Pointer
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS+0x10);
			Jump_To_Application();
		}
		else
		{
			SerialPutString("Verify Application: FAIL-------------------------------- \r\n\n");
			SerialPutString("Please re-programming------------------------------------ \r\n\n");
			Security_Access();
			Main_Menu();
		}
	}
//	}

}
void IAP_Init(void)
{
 USART_InitTypeDef USART_InitStructure;

  /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
  /* USART configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
}
void GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitDef;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	//Initialize pins
	GPIO_Init(GPIOA, &GPIO_InitDef);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOE, &GPIO_InitDef);

}
