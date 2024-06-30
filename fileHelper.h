#pragma once


int		writeCharsToFile(const char* arr,int size, FILE* fp, const char* msg);
int		writeIntToFile(int val, FILE* fp, const char* msg);

char*	readDynStringFromTextFile(FILE* fp);
int		readIntFromFile(int* val, FILE* fp, const char* msg);

int		readCharsFromFile(char* arr, int length, FILE* fp, const char* msg);
int		readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg);
char*	readStringFromFile(FILE* fp, const char* msg, int isComp);
int		writeStringToFile(const char* str, FILE* fp, const char* msg, int isComp);