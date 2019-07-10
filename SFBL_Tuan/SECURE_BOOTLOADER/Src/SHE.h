/*
 * SHE.h
 *
 *  Created on: Jun 18, 2019
 *      Author: AUP81HC
 */

#ifndef SHE_H_
#define SHE_H_

#include "stm32l4xx_hal.h"
#include "aes.h"
#include "cmac.h"


void CMD_INIT(unsigned char Key_ID, unsigned char Key[16]);

void CMD_ENC_ECB(unsigned char* Key, unsigned char* PlainText, unsigned char* CipherText);
void CMD_DEC_ECB(unsigned char* CipherText);
void CMD_ENC_CBC(unsigned char* Key,  uint32_t length, unsigned char* PlainText, unsigned char* CipherText);
void CMD_DEC_CBC(unsigned char* Key, unsigned char* CipherText, uint32_t length);

void CMD_GENERATE_MAC_START(unsigned char *Key, unsigned char K1[16], unsigned char K2[16]);
void CMD_GENERATE_MAC_UPDATE(unsigned char *Key, uint8_t *length, unsigned char* PlainText, unsigned char X[16]);
void CMD_GENERATE_MAC_FINISH(unsigned char* K1, unsigned char* K2, unsigned char* PlainText, uint8_t *length, unsigned char* MAC);

void CMD_VERIFY_MAC(unsigned char* Key, unsigned char* PlainText,  uint8_t length, unsigned char* Reference_MAC);

#endif /* SHE_H_ */
