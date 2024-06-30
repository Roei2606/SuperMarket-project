#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "General.h"



char* getStrExactLength(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	getsStrFixSize(temp, MAX_STR_LEN, msg);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char*	getsStrFixSize(char* buffer, int size, const char* msg)
{
	puts(msg);
	return myGets(buffer, size,stdin);
}

char*  myGets(char* buffer, int size, FILE* source)
{
	if (buffer != NULL && size > 0)
	{
		if (fgets(buffer, size, source))
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char**	splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength)
{
	char* word;
	int count = 0;
	char* temp = _strdup(str);
	*pTotalLength = 0;
	char** wordsArray = NULL;

	word = strtok(temp, del);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, del);
	}
	*pCount = count;
	free(temp);
	return wordsArray;
}


float	getPositiveFloat(const char* msg)
{
	float val;
	do {
		puts(msg);
		scanf("%f", &val);
	} while (val < 0);
	return val;
}

int		getPositiveInt(const char* msg)
{
	int val;
	do {
		puts(msg);
		scanf("%d", &val);
	} while (val < 0);
	return val;
}

int		countCharInString(const char* str, char tav)
{
	int count = 0;
	while (*str)
	{
		if (*str == tav)
			count++;
		str++;
	}
	return count;
}

void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element))
{
	for (int i = 0; i < size; i++)
		func((char*)(arr)+i * typeSize);

}

int		checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}
	return 1;
}

int getTwoDigOfFloat(float num)
{
	return (int)((num - ((int)(num))) * 100);
	
}


int translateCharINTOint(char ch) { //help writing to file

	int value = 0;

	if (ch >= 'A'&&ch <= 'Z') {

		value = (int)((ch + 10) - 'A');// A=65....Z=90  ->   A=10....Z=35

	}

	if (ch >= '0'&&ch <= '9') {

		value = (int)(ch - '0');

	}

	return value;
}




char translateBYTEtoChar(BYTE data) {//help reading from file

	int value = 0;
	char ch = ' ';
	value = (int)data;
	if (value >= 10 && value <= 35)
		ch = (char)((data + 'A') - 10);
	if (value >= 0 && value <= 9)
		ch = (char)(data + '0');

	return ch;


}


int chooseIFReadFromCompressed() {

	int choose = -1;

	printf("\nIf You Want To READ FROM Compressed File ENTER : 1 , ELSE ENTER 0:\n");
	scanf("%d", &choose);
	while (choose != 1 && choose != 0) {

		printf("\nINVALID VALUE //> TRY AGAIN");
		printf("\nIf You Want To READ FROM Compressed File ENTER : 1 , ELSE ENTER 0");
		scanf("%d", &choose);
	}

	return choose;

}
int chooseIFWriteToCompressed() {

	int choose = -1;

	printf("\nIf You Want To Save To Compressed File ENTER : 1 , ELSE ENTER 0:\n");
	scanf("%d", &choose);
	while (choose != 1 && choose != 0) {

		printf("\nINVALID VALUE //> TRY AGAIN");
		printf("\nIf You Want To Save To Compressed File ENTER : 1 , ELSE ENTER 0");
		scanf("%d", &choose);
	}

	return choose;

}

void convertIntegerToChar(char* num)
{
	if ((int)(*num) >= 0 && (int)(*num) <= 9) {
		*num = (char)((int)(*num) + 48);
	}
	else {
		*num = (char)((int)(*num) + 55);
	}
}

void printMessage(const char* str, ...)
{
	va_list allWords;
	char* currentWord;
	va_start(allWords, str);
	currentWord = str;
	while (currentWord != NULL)
	{
		printf("%s ", currentWord);
		currentWord = va_arg(allWords, char*);
	}
	printf("\n");
	va_end(allWords);
}