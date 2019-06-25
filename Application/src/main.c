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
__attribute__((__section__(".user_data"))) uint8_t RefMAC[16]={0x2E,0x5B,0xC7,0xB5,0x7C,0xA4,0xA8,0xA2,0x97,0xDA,0xA8,0x38,0x59,0xAA,0x8D,0x3B};
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
