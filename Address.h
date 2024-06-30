#pragma once

typedef struct
{
	int		num;
	char*	street;
	char*	city;
}Address;

int		initAddress(Address* pAd);
void	printAddress(const Address* pAd);
void	freeAddress(Address* pAd);
int		checkElements(char**  elements, int count);
char*	fixAddressParam(char* param);
void	freeElements(char**  elements, int count);


int		saveAddressToFile(const Address* pAdd, FILE* fp, int isComp);
int		compressAndSaveAddressToFile(const Address* pAdd, FILE* fp, int isComp);
int		loadAddressFromFile(Address* pAdd, FILE* fp, int isComp);
int		loadAddressFromCompressFile(Address* pAdd, FILE* fp);