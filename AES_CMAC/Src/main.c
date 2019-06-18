/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "aes.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RNG_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//UART2

//Random Number Generation
int randomX = 0;
char arr[11];

//CBC output
//unsigned char out_result[32];
//unsigned char out_result_1[64];

//CMAC variable
unsigned char key_CMAC[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
unsigned char const_Zero[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char const_Rb[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87};

unsigned char* M1;
unsigned char M2[16] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
unsigned char M3[40] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
						0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
						0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11};

unsigned char M4[64] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
						0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
						0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
						0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};


/* USER CODE END 0 */

void Receive_UART(unsigned char RXbuff[256], int *lenRX);
void Verify_MAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* input, unsigned char* Output);
void AES_CMAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* T);
void splitting8to4(unsigned char value, unsigned char byte[2]);
void Hex2string(unsigned char* input, unsigned char* output);
void Generate_Subkey(unsigned char* key, unsigned char* key1, unsigned char* key2);
void XOR_16bytes(unsigned char *a, unsigned char *b, unsigned char *out);
void LeftShift_OneBit(unsigned char *input,unsigned char *output);
void padding( unsigned char *M_last, unsigned char *pad, uint32_t len);

void Delay1(int num);
void CMAC_Start(struct AES_ctx ctx, unsigned char* K, unsigned char K1[16], unsigned char K2[16]);
void CMAC_Update(struct AES_ctx ctx, unsigned char* M, unsigned char X[16]);
void CMAC_Finish(struct AES_ctx ctx, unsigned char* K1, unsigned char* K2, unsigned char* M, uint32_t len, unsigned char T[16]);


//example Case #1: Encrypting 16 bytes (1 block) using AES-CBC with 128-bit key
/*
Key       : 0x06a9214036b8a15b512e03d534120006
IV        : 0x3dafba429d9eb430b422da802c9fac41
Plaintext : "Single block msg"

uint8_t key_1[] = { 0x06,0xa9,0x21,0x40,0x36,0xb8,0xa1,0x5b,0x51,0x2e,0x03,0xd5,0x34,0x12,0x00,0x06};
uint8_t iv_1[] = { 0x3d,0xaf,0xba,0x42,0x9d,0x9e,0xb4,0x30,0xb4,0x22,0xda,0x80,0x2c,0x9f,0xac,0x41};
char* Plaintext = "Single block msg";
*/


///Case #2: Encrypting 32 bytes (2 blocks) using AES-CBC with 128-bit key
/*
Key       : 0xc286696d887c9aa0611bbb3e2025a45a
IV        : 0x562e17996d093d28ddb3ba695a2e6f58
Plaintext : 0x000102030405060708090a0b0c0d0e0f
	        101112131415161718191a1b1c1d1e1f
Ciphertext: 0xd296cd94c2cccf8a3a863028b5e1dc0a
	        7586602d253cfff91b8266bea6d61ab1

unsigned char key_1[] = { 0xc2,0x86,0x69,0x6d,0x88,0x7c,0x9a,0xa0,0x61,0x1b,0xbb,0x3e,0x20,0x25,0xa4,0x5a};
unsigned char iv_1[] = { 0x56,0x2e,0x17,0x99,0x6d,0x09,0x3d,0x28,0xdd,0xb3,0xba,0x69,0x5a,0x2e,0x6f,0x58};
unsigned char Plaintext[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};

*/


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	struct AES_ctx ctx;

	unsigned char K1[16];
	unsigned char K2[16];

	unsigned char key1[32];
	unsigned char key2[32];

	unsigned char X[16];

	//unsigned char T[16];
	unsigned char T_result[32];
	//unsigned char T1[16];

	unsigned char RXbuff[256];
	unsigned char Data_buff[256];
	//unsigned char Data_buff1[256];
	unsigned char Data_buff2[256];

	unsigned char M_Last[16];

	unsigned char buffer2[8]= "STOP_NOW";

	int size_temp = 0;
	int n_block = 0;

	int flag1 = 0;

	int lenRX = 0;
	int New_LenRX = 0;
	int Data_buff_size = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RNG_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  /*
  randomX = HAL_RNG_GetRandomNumber(&hrng);
  itoa(randomX,arr,10);
  HAL_UART_Transmit(&huart2, (unsigned char *)" \n", 2, 20);
  HAL_UART_Transmit(&huart2, (unsigned char *)arr, 10, 20);
  HAL_UART_Transmit(&huart2, (unsigned char *)" \n", 2, 20);

  //Example Encrypted
  AES_init_ctx_iv(&ctx, key_1, iv_1);
  AES_CBC_encrypt_buffer(&ctx, Plaintext, 32,out_result);

  //AES_CBC_decrypt_buffer(&ctx, out_result, 128);

  Hex2string(out_result, out_result_1);

  HAL_UART_Transmit(&huart2, (unsigned char *)out_result_1, 64, 20);
  HAL_UART_Transmit(&huart2, (unsigned char *)"\n", 2, 20);
  */


  //Receive UART
  CMAC_Start(ctx, key_CMAC, K1, K2);
  Hex2string(K1, key1);
  Hex2string(K2, key2);

  HAL_UART_Transmit(&huart2, (unsigned char *)"Key1: ", 6, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)key1, 32, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)"\n", 2, 40);

  HAL_UART_Transmit(&huart2, (unsigned char *)"Key2: ", 6, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)key2, 32, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)"\n\n", 2, 40);

  memcpy(X, const_Zero, 16);
  memset(Data_buff, 0, 256);
  memset(M_Last, 0, 16);


  while(1)
  {
  Receive_UART(RXbuff, &lenRX);
  //unsigned char RX_Real[lenRX];
  //memcpy((unsigned char *)RX_Real,(unsigned char *)RXbuff,lenRX);

  //Output Message through UART
  HAL_UART_Transmit(&huart2, (unsigned char *)"Message: ", 8, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)RXbuff, lenRX, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)"\n", 2, 40);



  //data buff luu data da nhap vao
  //data buff 1 luu cac block < 16 bytes
  //unsigned char Data_buff;
  //int size_temp = 0;

  if(0 == memcmp( RXbuff, buffer2, lenRX))
  {
	  Data_buff_size += size_temp;
	  break;
  }




  if(size_temp == 0)
  {
	  memset(Data_buff2, 0, 256);
	  memcpy(Data_buff2, RXbuff, lenRX);
	  New_LenRX = lenRX;
  } else
  {
	  if(size_temp > 0)
	  {
		  int i = 0;
		  New_LenRX = (size_temp + lenRX);
		  memset(Data_buff2, 0, 256);
		  for(int g = 0; g< New_LenRX; g ++)
		  {
			  if(g < size_temp)
			  {
			  Data_buff2[g] = M_Last[g];
			  }
			  else {
				  Data_buff2[g] = RXbuff[i];
				  i++;
			  }
		  }
	  }
  }

  if(New_LenRX == 16)
  {
	  //CMAC_Update(ctx, Data_buff2, X);  //update 1 lan
	  memset(M_Last, 0, 16);
	  memcpy(M_Last, Data_buff2, New_LenRX);
	  memcpy(&Data_buff[16*flag1],Data_buff2, 16);
	  Data_buff_size += 16;
	  size_temp = 16;

  }else {
	  if(New_LenRX > 16)
	  {
		  n_block = ceil(New_LenRX/16.0);
		  size_temp = New_LenRX%16;

		  for(int i = 0; i< (n_block - 1); i++)
		  {
			  CMAC_Update(ctx, &Data_buff2[16*i], X);

			  memcpy(&Data_buff[16*flag1],&Data_buff2[16*i], 16);
			  Data_buff_size += 16;

			  flag1 += 1;
		  }
		  if(size_temp == 0)
		  {
			  memset(M_Last, 0, 16);
			  memcpy(M_Last,&Data_buff2[16*(n_block-1)], 16);
			  size_temp = 16;
		  } else
		  {
		  memset(M_Last, 0, 16);
		  memcpy(M_Last, &Data_buff2[16*(n_block-1)], size_temp);
		  }

	  } else {
		  memset(M_Last, 0, 16);
		  memcpy(M_Last, Data_buff2, New_LenRX);
		  size_temp = New_LenRX;
	  }
  }

}

  CMAC_Finish(ctx, K1, K2,M_Last, size_temp, X);

  //AES_CMAC(key_CMAC, RXbuff, lenRX, T);
  Hex2string(X, T_result);

  HAL_UART_Transmit(&huart2, (unsigned char *)"CMAC: ", 6, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)T_result, 32, 40);
  HAL_UART_Transmit(&huart2, (unsigned char *)"\n\n", 4, 40);

  //Verify_MAC(key_CMAC,RX_Real,lenRX, T, T1);
  Delay1(3000000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //while (1)
  //{
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  //}
  /* USER CODE END 3 */
}


void Delay1(int num)
{
	for(int i = 0; i< num; i++){

	}
}


void Receive_UART(unsigned char RXbuff[256], int *lenRX)
{
	int flag_NULL = 1;
	int z = 0;
	while(flag_NULL == 1)
	{
	memset(RXbuff, 0, 256);
	HAL_UART_Transmit(&huart2, (unsigned char*)"Nhap Message:", 13, 20);
	HAL_UART_Receive(&huart2, RXbuff, 256, 8000);

	for(int j=0; j< 5;j++)
		{
			if(RXbuff[j] == 0  && j>=1)
				flag_NULL = 1;
			else flag_NULL = 0;
		}
	*lenRX = 0;
	if(flag_NULL == 0)

	for(z = 0; z < 256; z++)
	{
		if(RXbuff[z] == 0)
			break;
		*lenRX = *lenRX + 1;
	}
	HAL_UART_Transmit(&huart2, (unsigned char*)"\n", 2, 20);
	}

}

void Verify_MAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* input, unsigned char* Output)
{
	AES_CMAC(K, M, len, Output);
	if (0 == memcmp((unsigned char*) Output, (unsigned char*) input, 16))
		HAL_UART_Transmit(&huart2, (unsigned char *)"VALID\n", 7, 40);
	else
		HAL_UART_Transmit(&huart2, (unsigned char *)"INVALID\n", 9, 40);
}



void CMAC_Start(struct AES_ctx ctx, unsigned char* K, unsigned char K1[16], unsigned char K2[16])
{
	Generate_Subkey(K, K1, K2);
	AES_init_ctx(&ctx, K);
}

void CMAC_Update(struct AES_ctx ctx, unsigned char* M, unsigned char X[16])
{
	unsigned char Y[16];
	XOR_16bytes(X, M, Y);
	AES_ECB_encrypt(&ctx,Y, X);
}

void CMAC_Finish(struct AES_ctx ctx, unsigned char* K1, unsigned char* K2, unsigned char* M, uint32_t len, unsigned char T[16])
{
	unsigned char padded[16];
	unsigned char M_last[16];
	unsigned char Y[16];

	if(len == 16)
		{
			XOR_16bytes(M,K1, M_last);
		}
		else {
			padding(M, padded, len%16);
			XOR_16bytes(padded,K2, M_last);
		}
	XOR_16bytes(T, M_last, Y);
	AES_ECB_encrypt(&ctx, Y, T);
}


void AES_CMAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* T)
{
	struct AES_ctx ctx;
	unsigned char K1[16];		//K1 128-bit subkeys 1
	unsigned char K2[16];		//K2 128-bit subkeys 2
	unsigned char padded[16];

	unsigned char X[16];
	unsigned char Y[16];

	unsigned char M_last[16]; 		//is the last block xor-ed with K1 or K2
	uint32_t n = 0; 			//for number of blocks to be processed
	uint32_t flag = 0; 			//for denoting if last block is complete or not

	Generate_Subkey(K, K1, K2);
	AES_init_ctx(&ctx, K);

	n = ceil(len/16.0);
	if(n == 0)
	{
		n = 1;
		flag = 0;
	} else {
		if(len%16==0)
			flag = 1;
			else {
				flag = 0;
			}
	}

	if(flag == 1)
	{
		XOR_16bytes(&M[16*(n-1)],K1, M_last);
	}
	else {
		padding(&M[16*(n-1)], padded, len%16);
		XOR_16bytes(padded,K2, M_last);
	}

	memcpy(X, const_Zero, 16);

	//method 1: using ECB
	for(int i = 0; i < (n-1);i++)
	{
		XOR_16bytes(X, &M[16*i], Y);
		AES_ECB_encrypt(&ctx,Y, X);
	}
	XOR_16bytes(M_last, X, Y);
	AES_ECB_encrypt(&ctx, Y, T);
}

void padding ( unsigned char *M_last, unsigned char *pad, uint32_t len)
{
    for (int j=0; j<16; j++ ) {
        if ( j < len) {
            pad[j] = M_last[j];
        } else if ( j == len) {
            pad[j] = 0x80;
        } else {
            pad[j] = 0x00;
        }
    }
}

void Generate_Subkey(unsigned char* K, unsigned char* K1, unsigned char* K2)
{
	struct AES_ctx ctx;
	unsigned char temp[16];
	unsigned char L[16];

	AES_init_ctx(&ctx, K);
	AES_ECB_encrypt(&ctx,const_Zero, L);
	//Hex2string(L, L1);  //show result AES-128(key,0)

	if((L[0] & 0x80) == 0) //if MSB(L) = 0 then K1 := L << 1
		LeftShift_OneBit(L,K1);
	else  // else K1 := (L << 1) XOR const_Rb;
	{
		LeftShift_OneBit(L,temp);
		XOR_16bytes(temp, const_Rb, K1);

	}

	if((K1[0] & 0x80) == 0) //if MSB(K1) = 0 then K2 := K1 << 1
		LeftShift_OneBit(K2, K1);
	else  // else K2 := (K1 << 1) XOR const_Rb
	{
		LeftShift_OneBit(K1,temp);
		XOR_16bytes(temp, const_Rb, K2);
	}
}

void XOR_16bytes(unsigned char *a, unsigned char *b, unsigned char *out)
  {
      int i;
      for (i=0;i<16; i++)
      {
          out[i] = a[i] ^ b[i];
      }
  }

void LeftShift_OneBit(unsigned char *input,unsigned char *output)
 {
     int i;
     unsigned char overflow = 0;

     for ( i=15; i>=0; i-- ) {
         output[i] = input[i] << 1;
         output[i] |= overflow;
         overflow = (input[i] & 0x80)?1:0;
     }
     return;
 }

void splitting8to4(unsigned char value, unsigned char byte[2])
{
	byte[0] = value >> 4;     // high byte
	byte[1] = value & 0x0F; // low byte
}

void Hex2string(unsigned char* input, unsigned char* output)
{
	int j = 0;
	uint8_t byte_arr[2];
	  for(int i =0; i<16; i++)
	 	    {
	 	    	splitting8to4(input[i],byte_arr);
	 	    	for(int z = 0; z< 2; z ++)
	 	    	{
	 	    		if(byte_arr[z] < 10)
	 	    			output[j] = byte_arr[z] + 48;
	 	    		else output[j] = byte_arr[z] + 87;

	 	    		j++;
	 	    	}
	 	    	memset(byte_arr, 0, sizeof(byte_arr));
	 	    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RNG;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

