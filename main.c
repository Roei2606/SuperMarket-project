#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"
#include "SuperFile.h"





int menu();


int main(int argc,char* argv[])
{
	
	if (argc != 4) {
		printf("Error in params\n");
		system("pause");
		return -1;
	}
	int opt;
	sscanf(argv[1], "%d", &opt);

	SuperMarket	market;

	if (opt == 0)
	{
		if (!initSuperMarket(&market, argv[3], CUSTOMER_FILE_NAME, opt))
		{
			printf("error init  Super Market from SUPER_FILE_NAME ");
			return 0;
		}
	}
	
	else {
		if (!initSuperMarket(&market, argv[2], CUSTOMER_FILE_NAME, opt))
		{
			printf("error init  Super Market SUPERMARKET_COMPRESS_check");
			return 0;
		}
	}

	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;


		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if (!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerPay:
			if (!doPayment(&market))
				printf("Error in payment\n");
			break;

		case eSortCustomer:
			sortCustomers(&market);
			break;

		case eSearchCustomer:
			findCustomer(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case EXIT:
			printMessage("Thank", "You", "For", "Shopping", "With", "Us", NULL);
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	handleCustomerStillShoppingAtExit(&market);
	
	if (opt != 0)
	{
		if (!compressAndsaveSuperMarketToFile(&market, SUPER_FILE_NAME_COMPRESSED, CUSTOMER_FILE_NAME, opt))
			printf("Error saving supermarket to file\n");
	}

	else
	{
		if (!saveSuperMarketToFile(&market, SUPER_FILE_NAME, CUSTOMER_FILE_NAME, opt))
			printf("Error saving supermarket to file\n");
	}


	freeMarket(&market);
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;

}

