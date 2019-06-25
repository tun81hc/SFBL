/*
 * cmac.h
 *
 *  Created on: Jun 17, 2019
 *      Author: AUP81HC
 */

#ifndef CMAC_H_
#define CMAC_H_
#include "aes.h"

typedef enum { false = 0, true } bool;


void AES_CMAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* T);
void splitting8to4(unsigned char value, unsigned char byte[2]);
void Hex2string(unsigned char* input, unsigned char* output);
void Generate_Subkey(unsigned char* key, unsigned char* key1, unsigned char* key2);
void XOR_16bytes(unsigned char *a, unsigned char *b, unsigned char *out);
void LeftShift_OneBit(unsigned char *input,unsigned char *output);
void padding( unsigned char *M_last, unsigned char *pad, uint32_t len);

void Delay1(int num);
void CMAC_Start(unsigned char* K, unsigned char K1[16], unsigned char K2[16]);
void CMAC_Update(unsigned char* M, uint8_t *length, unsigned char X[16]);
void CMAC_Finish(unsigned char* K1, unsigned char* K2, unsigned char* M, uint8_t *length, unsigned char T[16]);

bool Verify_MAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* Reference_MAC);
void Select_Key(unsigned char Key_ID, unsigned char Key[16]);
#endif /* CMAC_H_ */
