/*
 * bsp.c
 *
 *  Created on: May 19, 2019
 *      Author: Phuong
 */
#include "bsp.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"



static uint32_t usTicks = 0;
static uint32_t second = 0;
static uint32_t milisecond = 0;

static IO_Structure LED_Table[MAX_LED_INDEX] = {
		{LED0_PORT,LED0_PIN},
		{LED1_PORT,LED1_PIN},
};

static IO_Structure BTN_Table[MAX_BTN_INDEX] = {
		{BTN0_PORT,BTN0_PIN},
		{BTN1_PORT,BTN1_PIN},
};

/* Initialize LEDs and buttons */
void BSP_Init(){
	GPIO_InitTypeDef gpioConfig;
	NVIC_InitTypeDef nvicConfig;
	EXTI_InitTypeDef extiConfig;
	USART_InitTypeDef serialConfig;

	RCC_AHB1PeriphClockCmd(LED_RCC | BTN_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG | SERIAL_RCC, ENABLE);



	gpioConfig.GPIO_Pin = LED0_PIN;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED0_PORT,&gpioConfig);

	gpioConfig.GPIO_Pin = LED1_PIN;
	GPIO_Init(LED1_PORT,&gpioConfig);

	gpioConfig.GPIO_Mode = GPIO_Mode_IN;
	gpioConfig.GPIO_Speed = GPIO_Speed_100MHz;
	gpioConfig.GPIO_Pin = BTN0_PIN;
	GPIO_Init(BTN0_PORT,&gpioConfig);

	gpioConfig.GPIO_Pin = BTN1_PIN;
	GPIO_Init(BTN1_PORT,&gpioConfig);

	gpioConfig.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioConfig.GPIO_Pin = BTNWKUP_PIN;
	GPIO_Init(BTNWKUP_PORT,&gpioConfig);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	extiConfig.EXTI_Line= EXTI_Line0;
	extiConfig.EXTI_LineCmd = ENABLE;
	extiConfig.EXTI_Mode = EXTI_Mode_Interrupt;
	extiConfig.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&extiConfig);

	nvicConfig.NVIC_IRQChannel = EXTI0_IRQn;
	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	nvicConfig.NVIC_IRQChannelPreemptionPriority = 0;
	nvicConfig.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicConfig);
	NVIC_EnableIRQ(EXTI0_IRQn);

	nvicConfig.NVIC_IRQChannel = USART1_IRQn;
	nvicConfig.NVIC_IRQChannelCmd = ENABLE;
	nvicConfig.NVIC_IRQChannelPreemptionPriority = 0x01;
	nvicConfig.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicConfig);
	NVIC_EnableIRQ(USART1_IRQn);

	gpioConfig.GPIO_Pin = SERIAL_TX_PIN;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
	gpioConfig.GPIO_Mode = GPIO_Mode_AF;
	gpioConfig.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PinAFConfig(SERIAL_PORT,SERIAL_TX_PINSOURCE,GPIO_AF_USART1);
	GPIO_Init(SERIAL_PORT,&gpioConfig);

	gpioConfig.GPIO_Pin = SERIAL_RX_PIN;
	GPIO_PinAFConfig(SERIAL_PORT,SERIAL_RX_PINSOURCE,GPIO_AF_USART1);
	GPIO_Init(SERIAL_PORT,&gpioConfig);

	serialConfig.USART_BaudRate = SERIAL_BAUDRATE;
	serialConfig.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	serialConfig.USART_Parity = USART_Parity_No;
	serialConfig.USART_StopBits = USART_StopBits_1;
	serialConfig.USART_WordLength = USART_WordLength_8b;
	serialConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(SERIAL_PERIPH, &serialConfig);
	USART_ITConfig(SERIAL_PERIPH,USART_IT_RXNE,ENABLE);
	USART_Cmd(SERIAL_PERIPH,ENABLE);

	SysTick_Config(SystemCoreClock/1000000);
}
uint8_t toggle = 0;
void SysTick_Handler(){
	usTicks++;
	if(usTicks % 1000 == 0){
		if (++milisecond % 1000 == 0){
			if(toggle) ToggleLED(0);
			second++;
		}
	}
}

uint32_t getCurrentTime_us(){
	return usTicks;
}

uint32_t getElapsedTime_us(uint32_t start){
	return (usTicks >= start) ? (usTicks - start) : (0xFFFFFFFF + start - usTicks + 1);
}

void delay_us(uint32_t amount){
	uint32_t now;
	now = getCurrentTime_us();
	while(getElapsedTime_us(now) < amount){};
}

uint32_t getCurrentTime_ms(){
	return milisecond;
}

uint32_t getElapsedTime_ms(uint32_t start){
	return (milisecond >= start) ? (milisecond - start) : (0xFFFFFFFF + start - milisecond + 1);
}

void delay_ms(uint32_t amount){
	uint32_t now;
	now = getCurrentTime_ms();
	while(getElapsedTime_ms(now) < amount){};
}

uint32_t getCurrentTime_s(){
	return second;
}

uint32_t getElapsedTime_s(uint32_t start){
	return (second >= start) ? (second - start) : (0xFFFFFFFF + start - second + 1);
}

void delay_s(uint32_t amount){
	uint32_t now;
	now = getCurrentTime_s();
	while(getElapsedTime_s(now) < amount){};
}

void TurnOnLED(LED_Index index){
	IO_Structure LED_Params;
	if (index < MAX_LED_INDEX) {
		LED_Params = LED_Table[index];
		GPIO_ResetBits(LED_Params.port,LED_Params.pin);
	}
}

void TurnOffLED(LED_Index index){
	IO_Structure LED_Params;
	if (index < MAX_LED_INDEX){
		LED_Params = LED_Table[index];
		GPIO_SetBits(LED_Params.port,LED_Params.pin);
	}
}

void ToggleLED(LED_Index index){
	IO_Structure LED_Params;
	if (index < MAX_LED_INDEX){
		LED_Params = LED_Table[index];
		GPIO_ToggleBits(LED_Params.port,LED_Params.pin);
	}
}

void writeLED(LED_Index index,LED_State state){
	IO_Structure LED_Params;
	if (index < MAX_LED_INDEX){
		LED_Params = LED_Table[index];
		GPIO_WriteBit(LED_Params.port,LED_Params.pin, state);
	}
}

BTN_State GetButtonState(BTN_Index index){
	IO_Structure BTN_Params;
	BTN_State retVal;
	retVal = BTN_NOT_PRESSED;
	if (index < MAX_BTN_INDEX){
		BTN_Params = BTN_Table[index];
		retVal = GPIO_ReadInputDataBit(BTN_Params.port,BTN_Params.pin);
	}
	return retVal;
}

void Serial_putchar(uint8_t byte){
	while(USART_GetFlagStatus(SERIAL_PERIPH,USART_FLAG_TXE) == RESET){};
	USART_SendData(SERIAL_PERIPH,(uint16_t)byte);
}

void Serial_print(char* string){
	uint8_t i;
	uint8_t length;
	length = strlen(string);
	for (i=0; i<length; i++){
		Serial_putchar(string[i]);
	}
}


