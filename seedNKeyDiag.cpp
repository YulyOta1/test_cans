//*****************************************************************************
/// @file        seedNKeyXcpWin.c
/// @author      Alex Lunkenheimer (ETAS/PAC-Lr)
/// @responsible author
/// @brief       DLL-Main for Seed'n Key for XCP
/// @history     2002-09-16,Lr: - draft
///              2003-02-07,Lr: - DLL-API using extern "C"
///                             - adapted privileges to XCP-Spec V1.0,Rev22
//*****************************************************************************

//*****************************************************************************
//                               includes
//*****************************************************************************
#include <assert.h>
#include "seedNKeyDiag.h"
#include <iostream>

//*****************************************************************************
//                               variables
//*****************************************************************************

//*****************************************************************************
//                               prototypes
//*****************************************************************************

//*****************************************************************************
/// @fn   DllMain ... Windows DLL entrance point
//*****************************************************************************
KEYGEN_API GenerateKeyEx_EXT(
	const unsigned char* ipSeedArray,              /* Array for the seed [in] */
	unsigned int iSeedArraySize,                   /* Length of the array for the seed [in] */
	const unsigned int iSecurityLevel,             /* Security level [in] */
	const char* ipVariant,                         /* Name of the active variant [in] */
	unsigned char* iopKeyArray,                    /* Array for the key [in, out] */
	unsigned int iMaxKeyArraySize,                 /* Maximum length of the array for the key [in] */
	unsigned int& oActualKeyArraySize)             /* Length of the key [out] */
{
	//HMODULE hDll = LoadLibrary(TEXT("SA_EOl.dll"));

	//if (!hDll) {
	//	DWORD error = GetLastError();
	//	printf("Ошибка загрузки SA_EOL.dll. Код ошибки: %lu\n", error);
	//	return 1;
	//}
	//else {
	//	printf("Успешная загрузка SA_EOL.dll");
	//}
	HMODULE hDll = LoadLibrary(TEXT("SA_EXT.dll"));

	if (!hDll) {
		DWORD error = GetLastError();
		printf("Ошибка загрузки SA_EXT.dll. Код ошибки: %lu\n", error);
		return 1;
	}
	else {
		printf("Успешная загрузка SA_EXT.dll\n");
	}

	typedef bool(__stdcall* GenerateKeyExFn)(
		const unsigned char* pSeedArray,
		unsigned int nSeedArraySize,
		const unsigned int nSecurityLevel,
		const char* pVariant,
		unsigned char* pKeyArray,
		unsigned int nKeyArraySize,
		unsigned int& nSize);
	GenerateKeyExFn fSeedKey = NULL;
	//begin calculate key from seed------------------------------------------------------------
	switch (iSecurityLevel)
	{
	case 0x01://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x02://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x03:
		//for security access with Services 0x27 03 -> 0x27 04
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x05://for security access with Services 0x27 05 -> 0x27 06
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x09://for security access with Services 0x27 09 ->0x27 0A
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	default:break;
	}

	unsigned int calculatedKeySize = 8;

	//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
	//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
	//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
	//printf("ipVariant. Код %lu\n", *ipVariant);
	//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
	//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
	//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);

	//end calculate key from seed------------------------------------------------------------
	//Copy key to the output buffer 
	//Note: The first byte of the key array will be the first key byte of the bus message
	if (fSeedKey == NULL) {
		printf("Ошибка загрузки fSeedKey. Код ошибки: 2");
		return 2;
	}
	else {
		//bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, ipVariant, iopKeyArray, iMaxKeyArraySize, oActualKeyArraySize);
		bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, 0x00, iopKeyArray, iMaxKeyArraySize, calculatedKeySize);

		if (result) {
			printf("Ошибка рассчёта fSeedKey. Код ошибки: %lu\n", result);
			return 3;
		}
		//printf("Успешный рассчёт fSeedKey. Код %lu\n", result);
		//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
		//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
		//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
		//printf("ipVariant. Код %lu\n", *ipVariant);
		//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
		//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
		//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);
	}



	//setting length of key
	oActualKeyArraySize = 4;
	return KGRE_Ok;
}
KEYGEN_API GenerateKeyEx_PROG(
	const unsigned char* ipSeedArray,              /* Array for the seed [in] */
	unsigned int iSeedArraySize,                   /* Length of the array for the seed [in] */
	const unsigned int iSecurityLevel,             /* Security level [in] */
	const char* ipVariant,                         /* Name of the active variant [in] */
	unsigned char* iopKeyArray,                    /* Array for the key [in, out] */
	unsigned int iMaxKeyArraySize,                 /* Maximum length of the array for the key [in] */
	unsigned int& oActualKeyArraySize)             /* Length of the key [out] */
{
	//HMODULE hDll = LoadLibrary(TEXT("SA_EOl.dll"));

	//if (!hDll) {
	//	DWORD error = GetLastError();
	//	printf("Ошибка загрузки SA_EOL.dll. Код ошибки: %lu\n", error);
	//	return 1;
	//}
	//else {
	//	printf("Успешная загрузка SA_EOL.dll");
	//}
	HMODULE hDll = LoadLibrary(TEXT("SA_EOL.dll"));

	if (!hDll) {
		DWORD error = GetLastError();
		printf("Ошибка загрузки SA_ALL.dll. Код ошибки: %lu\n", error);
		return 1;
	}
	else {
		printf("Успешная загрузка SA_ALL.dll");
	}

	typedef bool(__stdcall* GenerateKeyExFn)(
		const unsigned char* pSeedArray,
		unsigned int nSeedArraySize,
		const unsigned int nSecurityLevel,
		const char* pVariant,
		unsigned char* pKeyArray,
		unsigned int nKeyArraySize,
		unsigned int& nSize);
	GenerateKeyExFn fSeedKey = NULL;
	//begin calculate key from seed------------------------------------------------------------
	switch (iSecurityLevel)
	{
	case 0x00://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x02://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x03:
		//for security access with Services 0x27 03 -> 0x27 04
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x05://for security access with Services 0x27 05 -> 0x27 06
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x09://for security access with Services 0x27 09 ->0x27 0A
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	default:break;
	}

	unsigned int calculatedKeySize = 8;

	//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
	//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
	//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
	//printf("ipVariant. Код %lu\n", *ipVariant);
	//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
	//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
	//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);

	//end calculate key from seed------------------------------------------------------------
	//Copy key to the output buffer 
	//Note: The first byte of the key array will be the first key byte of the bus message
	if (fSeedKey == NULL) {
		printf("Ошибка загрузки fSeedKey. Код ошибки: 2");
		return 2;
	}
	else {
		//bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, ipVariant, iopKeyArray, iMaxKeyArraySize, oActualKeyArraySize);
		bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, 0x00, iopKeyArray, iMaxKeyArraySize, calculatedKeySize);

		if (result) {
			printf("Ошибка рассчёта fSeedKey. Код ошибки: %lu\n", result);
			return 3;
		}
		//printf("Успешный рассчёт fSeedKey. Код %lu\n", result);
		//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
		//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
		//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
		//printf("ipVariant. Код %lu\n", *ipVariant);
		//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
		//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
		//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);
	}



	//setting length of key
	oActualKeyArraySize = 4;
	return KGRE_Ok;
}
KEYGEN_API GenerateKeyEx_EOL(
	const unsigned char* ipSeedArray,              /* Array for the seed [in] */
	unsigned int iSeedArraySize,                   /* Length of the array for the seed [in] */
	const unsigned int iSecurityLevel,             /* Security level [in] */
	const char* ipVariant,                         /* Name of the active variant [in] */
	unsigned char* iopKeyArray,                    /* Array for the key [in, out] */
	unsigned int iMaxKeyArraySize,                 /* Maximum length of the array for the key [in] */
	unsigned int& oActualKeyArraySize)             /* Length of the key [out] */
{
	//HMODULE hDll = LoadLibrary(TEXT("SA_EOl.dll"));

	//if (!hDll) {
	//	DWORD error = GetLastError();
	//	printf("Ошибка загрузки SA_EOL.dll. Код ошибки: %lu\n", error);
	//	return 1;
	//}
	//else {
	//	printf("Успешная загрузка SA_EOL.dll");
	//}
	HMODULE hDll = LoadLibrary(TEXT("SA_EOL.dll"));

	if (!hDll) {
		DWORD error = GetLastError();
		printf("Ошибка загрузки SA_EOL.dll. Код ошибки: %lu\n", error);
		return 1;
	}
	else {
		printf("Успешная загрузка SA_EOL.dll");
	}

	typedef bool(__stdcall* GenerateKeyExFn)(
		const unsigned char* pSeedArray,
		unsigned int nSeedArraySize,
		const unsigned int nSecurityLevel,
		const char* pVariant,
		unsigned char* pKeyArray,
		unsigned int nKeyArraySize,
		unsigned int& nSize);
	GenerateKeyExFn fSeedKey = NULL;
	//begin calculate key from seed------------------------------------------------------------
	switch (iSecurityLevel)
	{
	case 0x01://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x02://for security access with Services 0x27 01 ->0x27 02
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x03:
		//for security access with Services 0x27 03 -> 0x27 04
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x05://for security access with Services 0x27 05 -> 0x27 06
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	case 0x09://for security access with Services 0x27 09 ->0x27 0A
		fSeedKey = (GenerateKeyExFn)GetProcAddress(hDll, "GenerateKeyEx");
		break;
	default:break;
	}

	unsigned int calculatedKeySize = 8;

	//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
	//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
	//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
	//printf("ipVariant. Код %lu\n", *ipVariant);
	//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
	//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
	//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);

	//end calculate key from seed------------------------------------------------------------
	//Copy key to the output buffer 
	//Note: The first byte of the key array will be the first key byte of the bus message
	if (fSeedKey == NULL) {
		printf("Ошибка загрузки fSeedKey eol. Код ошибки: 2");
		return 2;
	}
	else {
		//bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, ipVariant, iopKeyArray, iMaxKeyArraySize, oActualKeyArraySize);
		bool result = fSeedKey(ipSeedArray, iSeedArraySize, iSecurityLevel, 0x00, iopKeyArray, iMaxKeyArraySize, calculatedKeySize);

		if (result) {
			printf("Ошибка рассчёта fSeedKey. eol Код ошибки: %lu\n", result);
			return 3;
		}
		//printf("Успешный рассчёт fSeedKey. Код %lu\n", result);
		//printf("ipSeedArray %x %x %x %x\n", ipSeedArray[0], ipSeedArray[1], ipSeedArray[2], ipSeedArray[3]);
		//printf("iSeedArraySize. Код %lu\n", iSeedArraySize);
		//printf("iSecurityLevel. Код %lu\n", iSecurityLevel);
		//printf("ipVariant. Код %lu\n", *ipVariant);
		//printf("iopKeyArray %x %x %x %x\n", iopKeyArray[0], iopKeyArray[1], iopKeyArray[2], iopKeyArray[3]);
		//printf("iMaxKeyArraySize. Код %lu\n", iMaxKeyArraySize);
		//printf("oActualKeyArraySize. Код %lu\n", calculatedKeySize);
	}



	//setting length of key
	oActualKeyArraySize = 4;
	return KGRE_Ok;
}
KEYGEN_API ASAP1A_CCP_ComputeKeyFromSeed(unsigned char* ipSeedArray,          /* Array for the seed [in] */
	UINT16 iSeedArraySize,               /* Length of the array for the seed [in] */
	unsigned char* opKey,                /* Array for the key [in, out] */
	UINT16 maxSizeKey,                   /* Maximum length of the array for the key [in] */
	UINT16* opSizeKey)                    /* Length of the key [out] */
{
	*opSizeKey = 4;
	return KGRE_Ok;
}

KEYGEN_API SeedAndKey_Type2(UINT32 ASeed, UINT32* AKey)
{
	return KGRE_Ok;
}

KEYGEN_API SeedAndKey_Type3(void* ASeed, void* AKey)
{
	return KGRE_Ok;
}
