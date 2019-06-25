/*
 * KeyMng.h
 *
 *  Created on: Jun 18, 2019
 *      Author: LAS81HC
 */

#ifndef KEYMNG_H_
#define KEYMNG_H_
#define KEYMNG_INDEX_0                      (0U)
#define KEYMNG_INDEX_1                      (1U)
#define KEYMNG_INDEX_2                      (2U)
#define KEYMNG_INDEX_3                      (3U)
typedef enum
{
	KEYFLAG_WRITE_PROTECTION         = (0x80U),
	KEYFLAG_BOOT_PROTECTION          = (0x40U),
	KEYFLAG_DEBUGGER_PROTECTION      = (0x20U),
	KEYFLAG_KEY_USAGE                = (0x10U),
	KEYFLAG_WILDCARD                 = (0x08U),
	KEYFLAG_VERIFY_ONLY              = (0x04U),
	KEYFLAG_SLOT_EMPTY               = (0x12U),
	KEYFLAG_PLAIN_KEY                = (0x02U)
}KEY_FLAG;

typedef enum {
	ERC_NO_ERROR = 0,
	ERC_SEQUENCE_ERROR,
	ERC_KEY_NOT_AVAILABLE=1,
	ERC_KEY_INVALID,
	ERC_KEY_EMPTY,
	ERC_NO_SECURE_BOOT,
	ERC_KEY_WRITE_PROTECTED=2,
	ERC_KEY_UPDATE_ERROR,
	ERC_RNG_SEED,
	ERC_NO_DEBUGGING,
	ERC_BUSY,
	ERC_MEMORY_FAILURE,
	ERC_GENERAL_ERROR=3
} SHE_ERROR_CODE;
SHE_ERROR_CODE KeyMng_UpdateKey(uint8_t Id, uint32_t* Data );
SHE_ERROR_CODE KeyMng_ReadKey( uint8_t Id, uint32_t* result );
void KeyMng_WriteKey();
void KeyMng_Int();
#endif /* KEYMNG_H_ */
