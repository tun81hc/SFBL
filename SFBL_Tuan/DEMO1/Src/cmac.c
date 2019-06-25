#include "cmac.h"
#include "main.h"

#include "string.h"
#include "math.h"


unsigned char const_Zero[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char const_Rb[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87};


void Delay1(int num)
{
	for(int i = 0; i< num; i++){

	}
}

unsigned char MAC_Bootloader[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
bool Verify_MAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* Reference_MAC)
{
	AES_CMAC(K, M, len, MAC_Bootloader);
	if (0 == memcmp(MAC_Bootloader, Reference_MAC, 16))
		return true;
	else
		return false;
}


void CMAC_Start(unsigned char* K, unsigned char K1[16], unsigned char K2[16])
{
	struct AES_ctx ctx;
	Generate_Subkey(K, K1, K2);
	AES_init_ctx(&ctx, K);
}

void CMAC_Update(unsigned char* M, uint8_t *length, unsigned char X[16])
{
	struct AES_ctx ctx;
	uint8_t block = 0;
	unsigned char Y[16];

	block = ceil(*length/16.0);

	memcpy(X, const_Zero, 16);

	for(int i = 0; i < (block-1);i++)
	{
	XOR_16bytes(X, &M[16*i], Y);
	AES_ECB_encrypt(&ctx,Y, X);
	}
	*length = *length%16;
}


void CMAC_Finish(unsigned char* K1, unsigned char* K2, unsigned char* M, uint8_t *length, unsigned char T[16])
{
	struct AES_ctx ctx;
	unsigned char padded[16];
	unsigned char M_last[16];
	unsigned char Y[16];

	if(*length == 16)
		{
			XOR_16bytes(M,K1, M_last);
		}
		else {
			padding(M, padded, *length%16);
			XOR_16bytes(padded,K2, M_last);
		}
	XOR_16bytes(T, M_last, Y);
	AES_ECB_encrypt(&ctx, Y, T);
}


void AES_CMAC(unsigned char* K, unsigned char* M, uint32_t len, unsigned char* T)
{
	struct AES_ctx ctx;

	memcpy(T, const_Zero, 16);
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

void Select_Key(unsigned char Key_ID, unsigned char Key[16])
{
 	unsigned char Key1[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};



 	switch(Key_ID){
 	case 1:
 		memcpy(Key, Key1, 16);
 		break;

 	case 2:
 		break;

 	default:
 		break;

 	}
}


