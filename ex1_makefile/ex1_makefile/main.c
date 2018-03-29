/*
* main.c
*
*  Created on: Nov 9, 2017
*      Author: Guy Keller and Ram Shimon the kingsmen
*/
//#include "header1.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int doPow(int num, int myPow) {
	int newNum = 1;
	for (int i = 0; i < myPow; i++) {
		newNum *= num;
	}
	return newNum;
}

unsigned int baseToDecimal(unsigned int intBase, char *number, int arraySize) {
	unsigned int decimalNum = 0;
	unsigned int curDigit = 0;
	for (int i = arraySize - 1; i >= 0; i--) {
		if (isalpha(number[i]))
			curDigit = number[i] - 'A' + 10;
		else
			curDigit = number[i] - '0';
		decimalNum += curDigit * doPow(intBase, arraySize - 1 - i);
	}
	return decimalNum;
}

void decimalToNewBase(unsigned int decimal, unsigned int base, char* arrayLocation, int arrayLength) {
	unsigned int curDec = decimal;
	int curIndex = arrayLength - 1;
	while (curDec > 0) {
		if (curDec % base <= 9)
			arrayLocation[curIndex] = curDec % base + '0';
		else
			arrayLocation[curIndex] = curDec % base - 10 + 'A';
		curDec /= base;
		curIndex--;
	}
}

int isBaseLegal(int base) {
	if (base <= 16 && base >= 2)
		return 1;
	return 0;
}

int doLog(unsigned int num, unsigned int base) {
	if (num == 0)
		return 1;
	int counter = 0;
	while (num > 0) {
		num /= base;
		counter++;
	}
	return counter;
}

int isLegalDigit(char digit, int base) {
	if (base <= 10) {
		if (digit - '0' < base && digit - '0' >= 0)
			return 1;
	}
	else {
		if (digit - '0' < 10 && digit - '0' >= 0)
			return 1;
		if (0 <= digit - 'A' && digit - 'A' < base - 10)
			return 1;
	}
	return 0;
}

int main() {
	char c = '0';
	int check = 0;
	int a = 0;
	int b = 0;
	unsigned int a2 = 1;
	unsigned int b2 = 1;
	int curIndex = 0;
	unsigned int decimalNum = 0;
	int newLength = 0;
	char number[1024] = "";
	char newBaseNum[1024] = "";
	printf("Please enter the number's base:\n");
	check = scanf("%d", &a);
	if (check == 0) {
		printf("An error occurred!\n");
		exit(1);
	}
	while ((c = getchar()) != '\n' && c != EOF) {}
	if (isBaseLegal(a) == 0) {
		printf("Invalid input base\n");
		exit(1);
	}
	printf("Please enter the desired base:\n");
	check = scanf("%d", &b);
	if (check == 0) {
		printf("An error occurred!\n");
		exit(1);
	}
	while ((c = getchar()) != '\n' && c != EOF) {}
	if (isBaseLegal(b) == 0) {
		printf("Invalid desired base\n");
		exit(2);
	}
	a2 = a;
	b2 = b;
	printf("Please enter a number in base %d:\n", a);
	//c = getchar();
	//c = getchar();
	while (c != EOF && c != '\n') {
		if (isLegalDigit(c, a) == 0) {
			printf("Invalid number!\n");
			exit(3);
		}
		number[curIndex] = c;
		curIndex++;
		c = getchar();
	}
	decimalNum = baseToDecimal(a2, number, curIndex);
	newLength = doLog(decimalNum, b2);
	decimalToNewBase(decimalNum, b2, newBaseNum, newLength);
	printf("The result is : %.*s\n", newLength, newBaseNum);
	return 0;
}












