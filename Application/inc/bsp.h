/* bsp.h */
/* 19-May-19 */
/* @brief: STM32F407VET Black Board support package */

#ifndef __BSP_BLACK
#define __BSP_BLACK

#include "stm32f4xx_gpio.h"

#define TRUE 1
#define FALSE 0

#define LED_RCC RCC_AHB1Periph_GPIOA
#define LED0_PORT GPIOA
#define LED0_PIN GPIO_Pin_6
#define LED1_PORT GPIOA
#define LED1_PIN GPIO_Pin_7

#define BTN_RCC RCC_AHB1Periph_GPIOE
#define BTNWKUP_PORT GPIOA
#define BTNWKUP_PIN GPIO_Pin_0
#define BTN0_PORT GPIOE
#define BTN0_PIN GPIO_Pin_4
#define BTN1_PORT GPIOE
#define BTN1_PIN GPIO_Pin_3

#define SERIAL_BAUDRATE 115200
#define SERIAL_PERIPH USART1
#define SERIAL_RCC RCC_APB2Periph_USART1
#define SERIAL_PORT GPIOA
#define SERIAL_TX_PIN GPIO_Pin_9
#define SERIAL_TX_PINSOURCE GPIO_PinSource9
#define SERIAL_RX_PIN GPIO_Pin_10
#define SERIAL_RX_PINSOURCE GPIO_PinSource10

typedef enum {LED_ON = 0, LED_OFF = 1} LED_State;
typedef enum {BTN_PRESSED = 0, BTN_NOT_PRESSED = 1} BTN_State;
typedef enum {BTN0 = 0, BTN1 = 1, MAX_BTN_INDEX} BTN_Index;
typedef enum {LED0 = 0, LED1 = 1, MAX_LED_INDEX} LED_Index;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} IO_Structure;






/* @brief: initialize board: LEDs, Buttons initialization */
void BSP_Init();
void TurnOnLED(LED_Index index);
void TurnOffLED(LED_Index index);
void ToggleLED(LED_Index index);
void writeLED(LED_Index index,LED_State state);

BTN_State GetButtonState(BTN_Index index);

uint32_t getCurrentTime_us();
uint32_t getElapsedTime_us(uint32_t start);
void delay_us(uint32_t amount);
uint32_t getCurrentTime_ms();
uint32_t getElapsedTime_ms(uint32_t start);
void delay_ms(uint32_t amount);
uint32_t getCurrentTime_s();
uint32_t getElapsedTime_s(uint32_t start);
void delay_s(uint32_t amount);

void Serial_putchar(uint8_t byte);
void Serial_print(char* string);
#endif
