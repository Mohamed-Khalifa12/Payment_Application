#include <stdio.h>
#include <string.h>
#include "card.h"

// GET THE CARD HOLDER NAME AND TEST ALL INPUTS
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t name[100] = "";
	printf("Please Enter Card Holder Name : ");
	if(!scanf(" %[^\n]", name))
		return WRONG_NAME;
	int name_length = strlen(name), count = 0;

	if (name_length < 20 || name_length >24)
		return WRONG_NAME;

	while (name[count])
	{
		if(!((name[count] >= 'a' && name[count] <= 'z') || (name[count] >= 'A' && name[count] <= 'Z') || name[count] == ' '))
			return WRONG_NAME;
		count++;
	}
	strcpy(cardData->cardHolderName, name);
	return CARD_OK;
}

//GET THE EXPIRY DATE FROM THE USER
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	int count = 0;
	uint8_t date[20] = "";
	printf("Please Enter The Card Expiry Date (NOTE MUST BE IN MM/YY FORMAT) : ");
	if(!scanf(" %s", date))
		return WRONG_EXP_DATE;
	if (strlen(date) != 5 || date[2] != '/')
		return WRONG_EXP_DATE;

	while (count <= 4)
	{
		if (count == 2) { count++; continue;}
		if (!(date[count] >= '0' && date[count] <= '9')) return WRONG_EXP_DATE;
		count++;
	}
	strcpy(cardData->cardExpirationDate, date);
	return CARD_OK;
}
//GET THE PAN FROM THE USER
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	uint8_t num[30];

	printf("Please Enter Primary Account Number (PAN) : ");
	if (!scanf(" %s",num))
		return WRONG_PAN;
	int numLength = strlen(num), count = 0;

	if (numLength < 16 || numLength >19)
		return WRONG_PAN;

	while (count < numLength)
	{
		if (!(num[count] >= '0' && num[count] <= '9')) return WRONG_PAN;
		count++;
	}

	strcpy(cardData->primaryAccountNumber, num);
	return CARD_OK;
}

//TESTING THE CARD HOLDER NAME
void getCardHolderNameTest(void) {
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : getCardHolderName\n\n");
	ST_cardData_t input1;
	EN_cardError_t output1 = getCardHolderName(&input1);
	printf("Test Case 1:\nInput Data : Mohamed Ahmed\nExpected Result : 1\nActual Result :%d\n\n", output1);
	ST_cardData_t input2;
	EN_cardError_t output2 = getCardHolderName(&input2);
	printf("Test Case 2:\nInput Data Mohamed Ahmed Ibrahim4:\nExpected Result : 1\nActual Result :%d\n\n", output2);
	ST_cardData_t input3;
	EN_cardError_t output3 = getCardHolderName(&input3);
	printf("Test Case 3:\nInput Data : Mohamed Ahmed Ibrahim Khalifa Ibrahim \nExpected Result : 1\nActual Result :%d\n\n", output3);
	ST_cardData_t input4;
	EN_cardError_t output4 = getCardHolderName(&input4);
	printf("Test Case 4:\nInput Data : Mohamed Ahmed Ibrahim \nExpected Result : 0\nActual Result :%d\n\n", output4);
}

//TESTING THE EXPIRY DATE
void getCardExpiryDateTest(void) 
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : getCardExpiryDateTest\n\n");
	ST_cardData_t input1;
	EN_cardError_t output1 = getCardExpiryDate(&input1);
	printf("Test Case 1:\nInput Data : 10/2022\nExpected Result : 2\nActual Result :%d\n\n", output1);
	ST_cardData_t input2;
	EN_cardError_t output2 = getCardExpiryDate(&input2);
	printf("Test Case 2:\nInput Data : 1/22\nExpected Result : 2\nActual Result :%d\n\n", output2);
	ST_cardData_t input3;
	EN_cardError_t output3 = getCardExpiryDate(&input3);
	printf("Test Case 3:\nInput Data : 10\\22\nExpected Result : 2\nActual Result :%d\n\n", output3);
	ST_cardData_t input4;
	EN_cardError_t output4 = getCardExpiryDate(&input4);
	printf("Test Case 4:\nInput Data : 10/2a\nExpected Result : 2\nActual Result :%d\n\n", output4);
	ST_cardData_t input5;
	EN_cardError_t output5 = getCardExpiryDate(&input5);
	printf("Test Case 5:\nInput Data : 10/22\nExpected Result : 0\nActual Result :%d\n\n", output5);

}
//TESTING THE PAN
void getCardPANTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : getCardPANTest\n\n");
	ST_cardData_t input1;
	EN_cardError_t output1 = getCardPAN(&input1);
	printf("Test Case 1:\nInput Data : 123456789\nExpected Result : 3\nActual Result :%d\n\n", output1);
	ST_cardData_t input2;
	EN_cardError_t output2 = getCardPAN(&input2);
	printf("Test Case 2:\nInput Data : 12345678912345678912\nExpected Result : 3\nActual Result :%d\n\n", output2);
	ST_cardData_t input3;
	EN_cardError_t output3 = getCardPAN(&input3);
	printf("Test Case 3:\nInput Data : 123a4567891234567\nExpected Result : 3\nActual Result :%d\n\n", output3);
	ST_cardData_t input4;
	EN_cardError_t output4 = getCardPAN(&input4);
	printf("Test Case 4:\nInput Data : 12345678912345678\nExpected Result : 0\nActual Result :%d\n\n", output4);
}