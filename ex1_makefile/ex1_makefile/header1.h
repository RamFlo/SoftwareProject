/*
* header1.h
*
*  Created on: Nov 9, 2017
*      Author: Guy Keller
*/

#ifndef HEADER1_H_
#define HEADER1_H_

unsigned int baseToDecimal(unsigned int intBase, char* number, int arraySize);
void decimalToNewBase(unsigned int decimal, unsigned int base, char* arrayLocation, int arrayLength);
int isBaseLegal(int base);
int doLog(int num, int base);
int doPow(int num, int myPow);
int isLegalDigit(char digit, int base);
#endif /* HEADER1_H_ */
