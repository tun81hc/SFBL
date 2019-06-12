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
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
static void IAP_Init(void);
Bootloader_State BL_State = Init;

int main(void)
{
	FLASH_If_Init();
	if (1)
	  {
		IAP_Init();
			/*USART_SendData(USART1,'A');*/
			/*while(1);*/
	    /* Display main menu */

		USART_SendData(USART1, 'A');
		 Main_Menu ();
		  }
		  /* Keep the user application running */
		  else
		  {
		    /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
		    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
		    {
		      /* Jump to user application */
		      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		      Jump_To_Application = (pFunction) JumpAddress;
		      /* Initialize user application's Stack Pointer */
		      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		      Jump_To_Application();
		    }
		  }

	  while (1)
	  {}
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









