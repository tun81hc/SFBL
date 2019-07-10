/*
 * KeyMng.c
 *
 *  Created on: Jun 18, 2019
 *      Author: LAS81HC
 */
#include "flash_if.h"
#include "KeyMng.h"
typedef struct
{
	uint8_t KeyId;
	KEY_FLAG KeyFlags;
	uint8_t counter;
	uint8_t reserved;
	uint32_t KeyData[4];
}Keyslot;

Keyslot keyslot_t[10];
uint32_t i=0;
uint32_t* buff=(uint32_t*)&keyslot_t;
uint32_t flashdestination= (uint32_t)0x08080000;
uint32_t num1,num2,num3,num;
uint32_t res;
SHE_ERROR_CODE ErrorCode;

SHE_ERROR_CODE KeyMng_UpdateKey(uint8_t Id, uint32_t* Data )
{
	for(i=0;i<10;i++ )
	{
		if (keyslot_t[i].KeyFlags== KEYFLAG_WRITE_PROTECTION )
		{
			ErrorCode= ERC_KEY_WRITE_PROTECTED;
		}
		else
		{
			if(Id != keyslot_t[i].KeyId)
				ErrorCode = ERC_KEY_NOT_AVAILABLE;
			else
			{
				if(Id == keyslot_t[i].KeyId)
				{
					keyslot_t[i].KeyData[KEYMNG_INDEX_0] = 	Data[0];
					keyslot_t[i].KeyData[KEYMNG_INDEX_1] = 	Data[1];
					keyslot_t[i].KeyData[KEYMNG_INDEX_2] = 	Data[2];
					keyslot_t[i].KeyData[KEYMNG_INDEX_3] = 	Data[3];
					ErrorCode= ERC_NO_ERROR;
				}
				else
					ErrorCode= ERC_GENERAL_ERROR;
			}
		}
	}
	return ErrorCode;
}

SHE_ERROR_CODE KeyMng_ReadKey( uint8_t Id, uint32_t* result )
{
	for(i=0;i<10;i++)
	{
			keyslot_t[i]= *(Keyslot*)(0x08080000 +0x14*i);
	}
	for(i=1;i<10;i++ )
	{
		if(Id != keyslot_t[i].KeyId)
			ErrorCode = ERC_KEY_NOT_AVAILABLE;
		else
		{
			if(Id ==keyslot_t[i].KeyId)
			{
				result[0] = keyslot_t[i].KeyData[KEYMNG_INDEX_0];
				result[1] = keyslot_t[i].KeyData[KEYMNG_INDEX_1];
				result[2] = keyslot_t[i].KeyData[KEYMNG_INDEX_2];
				result[3] = keyslot_t[i].KeyData[KEYMNG_INDEX_3];
				ErrorCode= ERC_NO_ERROR;
			}
		}
	}
	return ErrorCode;
}
void KeyMng_WriteKey()
{
	HAL_FLASH_Unlock();
	FLASH_If_Erase(0x08080000);
	i=sizeof(Keyslot)*10/4;
	FLASH_If_Write(flashdestination,buff,i);
	HAL_FLASH_Lock();
}
void KeyMng_Int()
{
	for(i=0;i<10;i++)
	{
		keyslot_t[i].KeyId=i;
		keyslot_t[i].counter=0;
	}

}


