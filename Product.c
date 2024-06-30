#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"
#include "fileHelper.h"


#define MIN_DIG 3
#define BYTE_ARR_SIZE 3
#define MAX_DIG 5
#define amountAndPriceSIZEBYTES 3

void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	getBorcdeCode(pProduct->barcode);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH);
		myGets(pProduct->name, sizeof(pProduct->name), stdin);
	} while (checkEmptyString(pProduct->name));
}

void	printProduct(const Product* pProduct)
{
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %10d\n", typeStr[pProduct->type], pProduct->price, pProduct->count);
}

int		saveProductToFile(const Product* pProduct, FILE* fp)
{
	if (fwrite(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error saving product to file\n");
		return 0;
	}
	return 1;
}

int		loadProductFromFile(Product* pProduct, FILE* fp)
{
	if (fread(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error reading product from file\n");
		return 0;
	}
	return 1;
}


int	compressAndSaveProductToFile(const Product* pProduct, FILE* fp) {

	BYTE dataBarcodeFirstPart[BYTE_ARR_SIZE];
	BYTE dataBarcodeSecPartAndNameAndType[BYTE_ARR_SIZE];
	BYTE amountAndPrice[amountAndPriceSIZEBYTES];
	eProductType type = pProduct->type;
	const char* barcode = pProduct->barcode;
	int priceWithoutAgurot = (int)pProduct->price;
	int agurot = getTwoDigOfFloat(pProduct->price);
	int amount = pProduct->count;
	int nameSize = (int)strlen(pProduct->name);
	int one = translateCharINTOint(barcode[0]);
	int two = translateCharINTOint(barcode[1]);
	int three = translateCharINTOint(barcode[2]);
	int four = translateCharINTOint(barcode[3]);
	int five = translateCharINTOint(barcode[4]);
	int six = translateCharINTOint(barcode[5]);
	int seven = translateCharINTOint(barcode[6]);
	////first part barcode compress start:
	dataBarcodeFirstPart[0] = (one <<2) | (two >> 4);////
	dataBarcodeFirstPart[1] = (two << 4) | (three >> 2);
	dataBarcodeFirstPart[2] = (three << 6) | four;
	////second part barcode compress start:
	dataBarcodeSecPartAndNameAndType[0] = (five << 2) | (six >> 4);
	dataBarcodeSecPartAndNameAndType[1] = (six << 4) | (seven >> 2);
	dataBarcodeSecPartAndNameAndType[2] = (seven  << 6 )| (nameSize<< 2) | type;
	////compress amount and price:
	amountAndPrice[0] = (BYTE)amount;
	amountAndPrice[1] = ((agurot << 1) | (priceWithoutAgurot >> 8)) ;
	amountAndPrice[2] =priceWithoutAgurot ;
	

	if (!saveCompressedBytesToFile(fp, dataBarcodeFirstPart, dataBarcodeSecPartAndNameAndType, BYTE_ARR_SIZE)) {
		printf("\nERROR WRITING BYTES TO FILE\n");
		return 0;
	}

	if (fwrite(pProduct->name, sizeof(char), nameSize, fp) != nameSize)
	{
		printf("\nERROR WRITING PRODUCT NAME TO FILE\n");
		return 0;

	}

	if (fwrite(&amountAndPrice, sizeof(BYTE), amountAndPriceSIZEBYTES, fp) != amountAndPriceSIZEBYTES)
	{

		printf("\nERROR WRITING PRODUCT AMOUT AND PRICE TO FILE\n");
		return 0;
	}





	return 1;
}


int saveCompressedBytesToFile(FILE* fp, BYTE* dataA, BYTE* dataB, int size) {

	if (fwrite(dataA, sizeof(BYTE), size, fp) != size)
		return 0;
	if (fwrite(dataB, sizeof(BYTE), size, fp) != size) return 0;

	return 1;
}












void getBorcdeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg, "Code should be of %d length exactly\n"
		"UPPER CASE letter and digits\n"
		"Must have %d to %d digits\n"
		"First and last chars must be UPPER CASE letter\n"
		"For example A12B40C\n",
		BARCODE_LENGTH, MIN_DIG, MAX_DIG);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		printf("Enter product barcode ");
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts(msg);
			ok = 0;
		}
		else {
			//check and first upper letters
			if (!isupper(temp[0]) || !isupper(temp[BARCODE_LENGTH - 1]))
			{
				puts("First and last must be upper case letters\n");
				ok = 0;
			}
			else {
				for (int i = 1; i < BARCODE_LENGTH - 1; i++)
				{
					if (!isupper(temp[i]) && !isdigit(temp[i]))
					{
						puts("Only upper letters and digits\n");
						ok = 0;
						break;
					}
					if (isdigit(temp[i]))
						digCount++;
				}
				if (digCount < MIN_DIG || digCount > MAX_DIG)
				{
					puts("Incorrect number of digits\n");
					ok = 0;
				}
			}
		}

	} while (!ok);

	strcpy(code, temp);
}


eProductType getProductType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);
	getchar();
	return (eProductType)option;
}

const char* getProductTypeStr(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

int		isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

int		compareProductByBarcode(const void* var1, const void* var2)
{
	const Product* pProd1 = (const Product*)var1;
	const Product* pProd2 = (const Product*)var2;

	return strcmp(pProd1->barcode, pProd2->barcode);
}


void	updateProductCount(Product* pProduct)
{
	int count;
	do {
		printf("How many items to add to stock?");
		scanf("%d", &count);
	} while (count < 1);
	pProduct->count += count;
}





void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}

int		readProductCountAndNameFromCompressFile(FILE* compressFileNmae, int* productCount, int* nameLength)
{
	BYTE data[2];
	if (fread(data, sizeof(BYTE), 2, compressFileNmae) != 2) return 0;
	*productCount = (data[0] << 2) | (data[1] >> 6);
	*nameLength = (data[1] & 0x3F);
	return 1;
}

int		readProductFromCompressFile(FILE* compressFileNmae, Product* p)
{

	BYTE data1[3];
	BYTE data2[3];
	BYTE data3[3];
	int nameLen;

	if (fread(data1, sizeof(BYTE), 3, compressFileNmae) != 3)return 0;
	if (fread(data2, sizeof(BYTE), 3, compressFileNmae) != 3)return 0;

	p->barcode[0] = (data1[0] >> 2);
	p->barcode[1] = (data1[0] & 0x3) | (data1[1] >> 4);
	p->barcode[2] = (data1[1] & 0xF) | data1[2] >> 6;
	p->barcode[3] = (data1[2] & 0x3F);
	p->barcode[4] = (data2[0] >> 2);
	p->barcode[5] = (data2[0] & 0x3) | (data2[1] >> 4);
	p->barcode[6] = ((data2[1] << 2) & 0xF) | ((data2[2] >> 6) & 0x3);

	for (int i = 0; i < BARCODE_LENGTH; i++)
		convertIntegerToChar(&p->barcode[i]);

	nameLen = (data2[2] >> 2) & 0xF;
	p->type = data2[2] & 0x3;

	if (!readCharsFromFile(p->name, nameLen, compressFileNmae, "Error with reading name of product\n"))
		return 0;

	if (fread(data3, sizeof(BYTE), 3, compressFileNmae) != 3)return 0;

	p->count = data3[0];
	float agorot;
	int shkalim;
	agorot = (float)(data3[1] >> 1);
	agorot /= 100;
	shkalim = ((data3[1] & 0x1) << 8) | data3[2];
	p->price = (float)(shkalim + agorot);
	return 1;
}