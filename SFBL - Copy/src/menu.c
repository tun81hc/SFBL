
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"


unsigned char Keydata[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
//unsigned char Reference_MAC[16]={0x2E,0x5B,0xC7,0xB5,0x7C,0xA4,0xA8,0xA2,0x97,0xDA,0xA8,0x38,0x59,0xAA,0x8D,0x3B};
uint8_t result[16];
unsigned char K1[16], K2[16],X[16],MAC;
uint8_t RefMACApp[16];
unsigned char* PlainText="sang0209";
unsigned char CipherText[16];
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };
uint8_t FileName[FILE_NAME_LENGTH];
unsigned char x[16];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */

void SerialDownload(void)
{
  uint8_t Number[10] = "          ";
  int32_t Size = 0;

  SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  if (Size > 0)
  {
    SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    SerialPutString(FileName);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    SerialPutString("-------------------\n");
  }
  else if (Size == -1)
  {
    SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (Size == -2)
  {
    SerialPutString("\n\n\rVerification failed!\n\r");
  }
  else if (Size == -3)
  {
    SerialPutString("\r\n\nAborted by user.\n\r");
  }
  else
  {
    SerialPutString("\n\rFailed to receive the file!\n\r");
  }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0 ; 

  SerialPutString("\n\n\rSelect Receive File\n\r");

  if (GetKey() == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

    if (status != 0) 
    {
      SerialPutString("\n\rError Occurred while Transmitting File\n\r");
    }
    else
    {
      SerialPutString("\n\rFile uploaded successfully \n\r");
    }
  }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */

void Main_Menu(void)
{
  uint8_t key = 0;
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n=              (C) COPYRIGHT 2011 STMicroelectronics                 =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=  STM32F4xx In-Application Programming Application  (Version 1.0.0) =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=                                   By MCD Application Team          =");
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n\r\n");

  /* Test if any sector of Flash memory where user application will be loaded is write protected */
  if (FLASH_If_GetWriteProtectionStatus() == 0)   
  {
    FlashProtection = 1;
  }
  else
  {
    FlashProtection = 0;
  }

  while (1)
  {

    SerialPutString("\r\n================== Main Menu ============================\r\n\n");
    SerialPutString("  Download Image To the STM32F4xx Internal Flash ------- 1\r\n\n");
    SerialPutString("  Upload Image From the STM32F4xx Internal Flash ------- 2\r\n\n");
    SerialPutString("  Load key to flash------------------------------------- 3\r\n\n");
    SerialPutString("  Execute The New Program------------------------------- 4\r\n\n");

    if(FlashProtection != 0)
    {
      SerialPutString("  Disable the write protection ------------------------- 5\r\n\n");
    }

    SerialPutString("==========================================================\r\n\n");

    /* Receive key */
    key = GetKey();

    if (key == '1')
    {
      /* Download user application in the Flash */
      SerialDownload();
    }
    else if (key == '2')
    {
      /* Upload user application from the Flash */
      SerialUpload();
    }
    else if (key == '4')  /*execute the new program*/
    {
    	/*Read KeyData with KeyID*/
    	KeyMng_ReadKey(3,(uint32_t*) result);
    	SerialPutString("Calculate CMAC of application--------------------------- \r\n\n");
    	//AES_CMAC(result,(unsigned char*)0x08008010, 128, CMACresult);
    	for(uint8_t i=0;i<16;i++)
    	{
    		RefMACApp[i]= *(uint8_t*)(0x8008000+0x01*i);
    	}

    	if (Verify_MAC(result, (unsigned char*)0x08008010, 128,RefMACApp) == 1)
    	{
    		SerialPutString("Verify Application: PASS-------------------------------- \r\n\n");
    		SerialPutString("Execute the new program -------------------------------- \r\n\n");
			USART_DeInit(USART1);
			GPIO_DeInit(GPIOA);
			SCB->VTOR = APPLICATION_ADDRESS+0x10;
					__disable_irq();
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4+0x10);

			//JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS);
		    //Jump to user application
			Jump_To_Application = (pFunction) ((uint32_t)JumpAddress);
			//Initialize user application's Stack Pointer
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS+0x10);
			Jump_To_Application();
    	}
    	else
    		SerialPutString("Verify Application: FAIL-------------------------------- \r\n\n");
    }
    else if (key == '3')
    {
    	/* Update key ID 3 with KeyData*/
    	KeyMng_Int();
    	KeyMng_UpdateKey(3,(uint32_t*)Keydata);
    	KeyMng_WriteKey();
    	SerialPutString("Load key successfully ------------------------- \r\n\n");
    }
    else if ((key == 0x34) && (FlashProtection == 1))
    {
      /* Disable the write protection */
      switch (FLASH_If_DisableWriteProtection())
      {
        case 1:
        {
          SerialPutString("Write Protection disabled...\r\n");
          FlashProtection = 0;
          break;
        }
        case 2:
        {
          SerialPutString("Error: Flash write unprotection failed...\r\n");
          break;
        }
        default:
        {
        }
      }
    }
    else
    {
      if (FlashProtection == 0)
      {
        SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
      }
      else
      {
        SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
      }
    }
  }
}

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
