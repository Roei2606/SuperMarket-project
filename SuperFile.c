#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myMacros.h"
#include "Address.h"
#include "General.h"
#include "fileHelper.h"
#include "SuperFile.h"



int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName,int isComp)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp);
	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n", isComp))
	{
		fclose(fp);
		return 0;
	}

	if (!saveAddressToFile(&pMarket->location, fp, isComp))
	{
		fclose(fp);
		return 0;
	}
	int count = getNumOfProductsInList(pMarket);

	if (!writeIntToFile(count, fp, "Error write product count\n"))
	{
		fclose(fp);
		return 0;
	}

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!saveProductToFile(pTemp, fp))
		{
			fclose(fp);
			return 0;
		}
		pN = pN->next;
	}

	fclose(fp);

	saveCustomerToTextFile(pMarket->customerArr, pMarket->customerCount, customersFileName);

	return 1;
}
#define BIN
#ifdef BIN
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, int isComp)
{
	
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp);
	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n", isComp);
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp, isComp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	}

	int count;
	if (!readIntFromFile(&count, fp, "Error reading product count\n"))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	}

	if (count > 0)
	{
		Product* pTemp;
		for (int i = 0; i < count; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			if (!loadProductFromFile(pTemp, fp))
			{
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
		}
	}


	fclose(fp);

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
return 0;

return	1;

}
#else
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, isComp)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	//L_init(&pMarket->productList);


	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n", isComp);
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp, isComp))
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	fclose(fp);

	loadProductFromTextFile(pMarket, "Products.txt");


	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;

}
#endif

int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	int count;
	fscanf(fp, "%d\n", &count);

	Product* pTemp;
	for (int i = 0; i < count; i++)
	{
		pTemp = (Product*)calloc(1, sizeof(Product));
		myGets(pTemp->name, sizeof(pTemp->name), fp);
		myGets(pTemp->barcode, sizeof(pTemp->barcode), fp);
		fscanf(fp, "%d %f %d\n", &pTemp->type, &pTemp->price, &pTemp->count);
		insertNewProductToList(&pMarket->productList, pTemp);
	}

	fclose(fp);
	return 1;
}



int	compressAndsaveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName, const char* customersFileName, int isComp)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
	{
		printf("Error open supermarket file to write\n");
		return 0;
	}

	int productCount = getNumOfProductsInList(pMarket);
	int compNameSize = (int)strlen(pMarket->name);
	BYTE dataOnAmountAndCompName[2];
	dataOnAmountAndCompName[0] = productCount >> 2;
	dataOnAmountAndCompName[1] = productCount<<6|compNameSize;
	
	if (fwrite(dataOnAmountAndCompName, sizeof(BYTE), 2, fp) != 2)
	{

		printf("Error writing supermarket name and product amount\n");
		CLOSE_RETURN_0(fp);
	}
	if(fwrite(pMarket->name, sizeof(char), compNameSize, fp) != compNameSize)
	{
		printf("\nERROR WRITING MARKET NAME TO FILE\n");
		CLOSE_RETURN_0(fp);
	}

	compressAndSaveAddressToFile(&pMarket->location, fp, 1);

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!compressAndSaveProductToFile(pTemp, fp))
		{
			printf("\nERROR WRITING PRODUCTS TO FILE\n");
			CLOSE_RETURN_0(fp);
		}
		pN = pN->next;
	}

	saveCustomerToTextFile(pMarket->customerArr, pMarket->customerCount, customersFileName);
	return 1;
}


int		readSuperMarketFromBinaryFileCompress(SuperMarket* pMarket, const char* compressFileNmae, const char* customersFileName)
{
	FILE* fp = fopen(compressFileNmae, "rb");
	if (!fp) {
		printf("Error with open compress file\n");
		return 0;
	}
	int productCount = 0;
	int nameLen = 0;
	if (readProductCountAndNameFromCompressFile(fp, &productCount, &nameLen) == 0)
	{
		printf("Error with reading Product Count And NameLen\n");
		CLOSE_RETURN_0(fp);
	}
	pMarket->name = (char*)calloc(nameLen + 1, sizeof(char));
	if (!pMarket->name)
	{
		printf("ALLOCATION ERROR\n");
		CLOSE_RETURN_0(fp);
	}
	if (!readCharsFromFile(pMarket->name, nameLen, fp, "Can't read name supermarket"))
	{
		printf("Error with reading Name\n");
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	if (loadAddressFromCompressFile(&pMarket->location, fp) == 0)
	{
		printf("Error with reading Address\n");
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	if (productCount > 0)
	{
		Product* pTemp;
		for (int i = 0; i < productCount; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("ALLOCATION ERROR\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (readProductFromCompressFile(fp, pTemp) == 0)
			{
				printf("Error with reading product\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
		}
	}

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
	{
		fclose(fp);
		return 0;
	}

	fclose(fp);
	return 1;
}

