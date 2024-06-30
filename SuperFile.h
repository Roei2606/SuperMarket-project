#pragma once

#include <stdio.h>
#include "Supermarket.h"

int		saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
								const char* customersFileName, int isComp);
int		loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
								const char* customersFileName, int isComp);
int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName);


int		compressAndsaveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
											const char* customersFileName, int isComp);
int		readSuperMarketFromBinaryFileCompress(SuperMarket* pMarket, const char* compressFileNmae, const char* customersFileName);