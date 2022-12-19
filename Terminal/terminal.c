#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "terminal.h"
#include "../Card/card.h"

//GET THE DATE FROM THE SYSTEM AND CHECK THE AVILABILITY
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char date[11]; int count = 0;
	sprintf(date, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	
	if (strlen(date) != 11)	WRONG_DATE;

	while (date[count])
	{
		if (count == 2 || count == 5) { count++; continue; }
		if (!(date[count] >= '0' || date[count] <= '9')) return WRONG_DATE;
		count++;
	}
	strcpy(termData->transactionDate, date);
	return TERMINAL_OK;
}

//CHECK IF THE TRANSACTION DATE EXCEED THE EXPIRY DATE
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	char Exp_date[6], Tran_date[11];
	int Year_degit_identical = 0;
	strcpy(Exp_date, cardData->cardExpirationDate);
	strcpy(Tran_date, termData->transactionDate);

	for (int year_count = 3; year_count <= 4; year_count++)
	{
		if (Exp_date[year_count] < Tran_date[year_count + 5])
			return EXPIRED_CARD;
		if (Exp_date[year_count] == Tran_date[year_count + 5]) Year_degit_identical++;
	}

	if (Year_degit_identical == 2)
	{
		int exp_month = atoi(Exp_date);
		int tran_month = atoi(&Tran_date[3]);
		if(tran_month > exp_month)
			return EXPIRED_CARD;
	}

	return TERMINAL_OK;
}

//GET THE TRANSACTION AMOUNT FROM THE USER
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount;
	printf("Please Enter the Transaction amount : ");
	if (!scanf(" %f", &amount))
		return INVALID_AMOUNT;
	if(amount <= 0)
		return INVALID_AMOUNT;

	termData->transAmount= amount;
	return TERMINAL_OK;
}

//SET A MAX AMOUNT FOR ONE TRANSACTION
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;

	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

//CHEK IF THE INPUT TRANSACTION AMOUNT BELOW THE ALLAWED MAX AMOUT 
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}


void isCardExpriedTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : isCardExpriedTest\n\n");
	ST_terminalData_t input1_t;
	getTransactionDate(&input1_t);
	ST_cardData_t input1_c;
	getCardExpiryDate(&input1_c);
	EN_terminalError_t output1 = isCardExpired(&input1_c, &input1_t);
	printf("Test Case 1:\nInput Data : 12/20\nExpected Result : 2\nActual Result :%d\n\n", output1);
	ST_terminalData_t input2_t;
	getTransactionDate(&input2_t);
	ST_cardData_t input2_c;
	getCardExpiryDate(&input2_c);
	EN_terminalError_t output2 = isCardExpired(&input2_c, &input2_t);
	printf("Test Case 2:\nInput Data : 12/21\nExpected Result : 2\nActual Result :%d\n\n", output2);
	ST_terminalData_t input3_t;
	getTransactionDate(&input3_t);
	ST_cardData_t input3_c;
	getCardExpiryDate(&input3_c);
	EN_terminalError_t output3 = isCardExpired(&input3_c, &input3_t);
	printf("Test Case 3:\nInput Data : 10/22 \nExpected Result : 2\nActual Result :%d\n\n", output3);
	ST_terminalData_t input4_t;
	getTransactionDate(&input2_t);
	ST_cardData_t input4_c;
	getCardExpiryDate(&input4_c);
	EN_terminalError_t output4 = isCardExpired(&input4_c, &input4_t);
	printf("Test Case 4:\nInput Data : 10/23 \nExpected Result : 0\nActual Result :%d\n\n", output4);
	ST_terminalData_t input5_t;
	getTransactionDate(&input2_t);
	ST_cardData_t input5_c;
	getCardExpiryDate(&input5_c);
	EN_terminalError_t output5 = isCardExpired(&input5_c, &input5_t);
	printf("Test Case 5:\nInput Data : 12/22 \nExpected Result : 0\nActual Result :%d\n\n", output5);
}

void getTransactionAmountTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : getTransactionAmount\n\n");
	ST_terminalData_t input1;
	EN_terminalError_t output1 = getTransactionAmount(&input1);
	printf("Test Case 1:\nInput Data : 0\nExpected Result : 4\nActual Result :%d\n\n", output1);
	ST_terminalData_t input2;
	EN_terminalError_t output2 = getTransactionAmount(&input2);
	printf("Test Case 2:\nInput Data : -12.5\nExpected Result : 4\nActual Result :%d\n\n", output2);
	ST_terminalData_t input3;
	EN_terminalError_t output3 = getTransactionAmount(&input3);
	printf("Test Case 3:\nInput Data : 256.236 \nExpected Result : 0\nActual Result :%d\n\n", output3);
}

void setMaxAmountTest(void) {
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : setMaxAmountTest\n\n");
	ST_terminalData_t input1;
	EN_terminalError_t output1 = setMaxAmount(&input1,0);
	printf("Test Case 1:\nInput Data : 0\nExpected Result : 6\nActual Result :%d\n\n", output1);
	ST_terminalData_t input2;
	EN_terminalError_t output2 = setMaxAmount(&input2,-12.5);
	printf("Test Case 2:\nInput Data : -12.5\nExpected Result : 6\nActual Result :%d\n\n", output2);
	ST_terminalData_t input3;
	EN_terminalError_t output3 = setMaxAmount(&input3,256.236);
	printf("Test Case 3:\nInput Data : 256.236 \nExpected Result : 0\nActual Result :%d\n\n", output3);
}

void isBelowMaxAmountTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : isBelowMaxAmountTest\n\n");
	ST_terminalData_t input1;
	setMaxAmount(&input1, 3000);
	getTransactionAmount(&input1);
	EN_terminalError_t output1 = isBelowMaxAmount(&input1);
	printf("Test Case 1:\nInput Data : 4000\nExpected Result : 5\nActual Result :%d\n\n", output1);
	ST_terminalData_t input2;
	setMaxAmount(&input2, 3000);
	getTransactionAmount(&input2);
	EN_terminalError_t output2 = isBelowMaxAmount(&input2);
	printf("Test Case 2:\nInput Data : 3000\nExpected Result : 0\nActual Result :%d\n\n", output2);
	ST_terminalData_t input3;
	setMaxAmount(&input3, 3000);
	getTransactionAmount(&input3);
	EN_terminalError_t output3 = isBelowMaxAmount(&input3);
	printf("Test Case 3:\nInput Data : 1000\nExpected Result : 0\nActual Result :%d\n\n", output3);
}