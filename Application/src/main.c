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
#include "bsp.h"
uint32_t sang = 0;
GPIO_InitTypeDef GPIO_InitStruct;
__attribute__((__section__(".user_data"))) uint8_t RefMAC[16]={0x1c,0x7b,0x45,0x47,0x74,0xb9,0x50,0x6c,0x32,0x9b,0x11,0xc4,0xfa,0xf0,0x79,0x36};
int main(void)
{
	BSP_Init();

	while(1)
	{
		ToggleLED(0);
		ToggleLED(1);
		for(sang = 0;sang<1000000;sang++){}

	/*GPIO_ToggleBits(GPIOA,GPIO_Pin_7);
	GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
	for(sang = 0;sang<10000;sang++){}*/
	}

}
