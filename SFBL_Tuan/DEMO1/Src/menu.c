
/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "common.h"
#include "flash_if.h"
#include "ymodem.h"
#include "KeyMng.h"
#include "cmac.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;

uint32_t FlashProtection = 0;
uint8_t aFileName[FILE_NAME_LENGTH];



/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

//Calculate CMAC variable
unsigned char Key[16];
unsigned char T[16];
unsigned char T1[32];

//reprogramming variable
uint8_t Ymodem_Data[9000];
uint8_t Key2[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
uint8_t RefMAC[16]={0xaf,0x7a,0xe2,0x89,0x9f,0xb7,0x20,0xff,0xd5,0xc3,0x0c,0xba,0x08,0x71,0x8d,0x8b};

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint32_t sizeA = 0;
  uint32_t sizeB = 0;
  uint8_t number[11] = {0};
  COM_StatusTypeDef result;

  sizeA = 0;
  sizeB = 0;
  memset(Ymodem_Data, 0,sizeof(Ymodem_Data));

  Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  result = Ymodem_Receive( &sizeA, Ymodem_Data);
  for(int i = 0; i< 1000000; i++){}		//Delay

  if (result == COM_OK)
  {
    Serial_PutString((uint8_t *)"\n\n\r Receiving Completed Successfully!\n\r--------------------------------\r\n Name: ");
    Serial_PutString(aFileName);
    Int2Str(number, sizeA);
    Serial_PutString((uint8_t *)"\n\r Size: ");
    Serial_PutString(number);
    Serial_PutString((uint8_t *)" Bytes\r\n");
    Serial_PutString((uint8_t *)"-------------------\n");
  }
  else if (result == COM_LIMIT)
  {
    Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (result == COM_DATA)
  {
    Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r");
  }
  else if (result == COM_ABORT)
  {
    Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r");
  }
  else
  {
    Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r");
  }

  sizeB = sizeA - 32;
  //Reprogramming
  if(Verify_MAC(Key2, &Ymodem_Data[32], sizeB, RefMAC)){
		Serial_PutString((uint8_t *)"\r\nReprogramming SUCCESS !");
		Serial_PutString((uint8_t *)"\r\nYour file Authentication and Integration!");
		Serial_PutString((uint8_t *)"\r\nFLASHING to FLASH ............");
		FLASH_If_Write(0x08008000, (uint32_t *)Ymodem_Data, sizeB);
		Serial_PutString((uint8_t *)"\r\nDONE!!!\r\n\n");
  }
  else {
	  Serial_PutString((uint8_t *)"\r\nReprogramming FAIL !");
	  Serial_PutString((uint8_t *)"\r\nYour file does not Authentication and Integration!");
	  Serial_PutString((uint8_t *)"\r\nReenter New File !!!\r\n\n");
	  Main_Menu();
  }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0;

  Serial_PutString((uint8_t *)"\n\n\rSelect Receive File\n\r");

  HAL_UART_Receive(&huart2, &status, 1, RX_TIMEOUT);
  if ( status == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

    if (status != 0)
    {
      Serial_PutString((uint8_t *)"\n\rError Occurred while Transmitting File\n\r");
    }
    else
    {
      Serial_PutString((uint8_t *)"\n\rFile uploaded successfully \n\r");
    }
  }
}

uint32_t randomX = 0;
unsigned char arrRandom[10];
unsigned char MAC1[16];
unsigned char MAC11[32];
unsigned char MAC2[32];
uint8_t flag1 = 1;
uint8_t count1 = 0;
void Security_Access(void)
{
	Serial_PutString((uint8_t *)"\r\n-------------Security Access---------------\r\n");

	memset(MAC1, 0, 16);
	memset(MAC11, 0, 32);
	Serial_PutString((uint8_t *)"\r\nSecurity Code: ");
	//randomX = HAL_RNG_GetRandomNumber(&hrng);
	randomX = 1478523694;
	InttoString(randomX,arrRandom);
	Serial_PutString((uint8_t *)arrRandom);
	AES_CMAC(Key2, arrRandom, 10, MAC1);
	Hex2string(MAC1,MAC11);

	while(flag1 == 1)
	{

	Serial_PutString((uint8_t *)"\r\nEnter CMAC: ");
	HAL_UART_Receive(&huart2, MAC2, 32, 10000);
	Serial_PutString((uint8_t *)"\r\n ...\n\r");
	for(int j=0; j< 32;j++)
		{
			if(MAC2[j] == MAC11[j])
			{
				count1 += 1;
				if(count1 == 31)
					flag1 = 0;
			}
			else flag1 = 1;
		}
	memset(MAC2, 0, 32);
	}
	Serial_PutString((uint8_t *)"\r\n SUCCESS\n\r");
}


void Menu_Intro(void)
{
	Serial_PutString((uint8_t *)"\r\n======================================================================");
	Serial_PutString((uint8_t *)"\r\n=              (C) COPYRIGHT 2015 STMicroelectronics                 =");
	Serial_PutString((uint8_t *)"\r\n=                                                                    =");
	Serial_PutString((uint8_t *)"\r\n=  STM32L4xx In-Application Programming Application  (Version 9.9.9) =");
	Serial_PutString((uint8_t *)"\r\n=                                                                    =");
	Serial_PutString((uint8_t *)"\r\n=                                   By Security Team                 =");
	Serial_PutString((uint8_t *)"\r\n======================================================================");
	Serial_PutString((uint8_t *)"\r\n\r\n");
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
uint8_t key = 0;
void Main_Menu(void)
{
	while (1)
	{
	Serial_PutString((uint8_t *)"\r\n=================== Main Menu ============================\r\n\n");
	Serial_PutString((uint8_t *)"  Download image to the internal Flash ----------------- 1\r\n\n");
	Serial_PutString((uint8_t *)"  Upload image from the internal Flash ----------------- 2\r\n\n");
	Serial_PutString((uint8_t *)"  Verify and Execute the loaded application ------------ 3\r\n\n");
	Serial_PutString((uint8_t *)"  Load Key to Flash ------------------------------------ 4\r\n\n");
	Serial_PutString((uint8_t *)"=========================================================\r\n\n");

	/* Clean the input path */
	__HAL_UART_FLUSH_DRREGISTER(&huart2);

	/* Receive key */
	HAL_UART_Receive(&huart2, &key, 1, RX_TIMEOUT);

	switch (key)
	{
	case '1' :
	  /* Download user application in the Flash */
	  SerialDownload();

	  break;
	case '2' :

	  /* Upload user application from the Flash */
	  SerialUpload();
	  break;

	case '3' :
		/* Verify application and execute application*/
		Serial_PutString((uint8_t *)"Verify Application......\r\n\n");
		//Select_Key((uint8_t)1, Key);
		KeyMng_ReadKey(3,(uint32_t*)Key);
		if(Verify_MAC(Key, (unsigned char *)0x08008020, 6240, (unsigned char *)0x08008000)){
			Serial_PutString((uint8_t *)"Program Verify SUCCESS !\r\n\n");
			Serial_PutString((uint8_t *)"Start program execution......\r\n\n");

			//Deinit HAL and UART2
			HAL_DeInit();
			HAL_UART_DeInit(&huart2);

			//Execute application
			SCB->VTOR = APPLICATION_ADDRESS+0x20;
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4 + 0x20);
			JumpToApplication = (pFunction) JumpAddress;
			// Initialize user application's Stack Pointer
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS +0x20);
			JumpToApplication();
		}
		else Serial_PutString((uint8_t *)"Program Verify FAIL !\r\n\n");
		break;

	case '4':
		/* Download Key in the Flash */
		Serial_PutString((uint8_t *)"Key Updating......\r\n\n");
		KeyMng_Int();
		KeyMng_UpdateKey(3,(uint32_t*)Key2);
		KeyMng_WriteKey();
		Serial_PutString((uint8_t *)"DONE !!\r\n\n");
		break;

	/*hidden feature: Calculate MAC !!!!!! */
	case 'm':
		KeyMng_ReadKey(3,(uint32_t*)Key);
		AES_CMAC(Key , (unsigned char*)0x08008020, 6240, T);
		Hex2string(T, T1);
		//FLASH_If_Write((uint32_t)0x08008000,(uint32_t *)T, 16);
		//Serial_PutString((uint8_t *)T);
		//Serial_PutString((uint8_t *)"\r\n\n");
		Serial_PutString((uint8_t *)T1);
		Serial_PutString((uint8_t *)"\r\n\n");
		break;

	default:
	Serial_PutString((uint8_t *)"Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
	break;
    }
  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
