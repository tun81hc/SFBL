/*
 * SHE.c
 *
 *  Created on: Jun 18, 2019
 *      Author: AUP81HC
 */
#include "SHE.h"
#include "string.h"
#include "stdio.h"


struct AES_ctx ctx;
//HAL_StatusTypeDef status;

//Init
void CMD_INIT(unsigned char Key_ID, unsigned char Key[16])
{
	Select_Key(Key_ID, Key);
	AES_init_ctx(&ctx, Key);
}

//ECB Encryption Process
void CMD_ENC_ECB(unsigned char* Key, unsigned char* PlainText, unsigned char* CipherText)
{
	AES_ECB_encrypt(&ctx, PlainText, CipherText);
}

void CMD_DEC_ECB(unsigned char* PlainText)
{
	//this function return CipherText value to PlainText varible
	AES_ECB_decrypt(&ctx, PlainText);
}

//CBC Encryption Process
void CMD_ENC_CBC(unsigned char* Key,  uint32_t length, unsigned char* PlainText, unsigned char* CipherText)
{
	AES_CBC_encrypt_buffer(&ctx, PlainText, length, CipherText);
}

void CMD_DEC_CBC(unsigned char* Key, unsigned char* CipherText, uint32_t length)
{
	AES_CBC_decrypt_buffer(&ctx, CipherText, length);
}

//Generate Sub-keys
void CMD_GENERATE_MAC_START(unsigned char *Key, unsigned char K1[16], unsigned char K2[16])
{
	CMAC_Start(Key, K1, K2);
}

//Update CMAC
void CMD_GENERATE_MAC_UPDATE(unsigned char *Key, uint8_t *length, unsigned char* PlainText, unsigned char X[16])
{
	CMAC_Update(PlainText, length, X);
}

void CMD_GENERATE_MAC_FINISH(unsigned char* K1, unsigned char* K2, unsigned char* PlainText, uint8_t *length, unsigned char* MAC)
{
	CMAC_Finish(K1, K2, PlainText, length, MAC);
}

void CMD_VERIFY_MAC(unsigned char* Key, unsigned char* PlainText,  uint8_t length, unsigned char* Reference_MAC)
{
		Verify_MAC(Key, PlainText, length, Reference_MAC);
}


