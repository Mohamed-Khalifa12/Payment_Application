#include <stdio.h>
#include <string.h>
#include "server.h"
#include "../Card/card.h"

ST_accountsDB_t accountsDB[255] = {
    {2000.0, RUNNING, "8989374615436851"} ,
    {100000.0, BLOCKED, "5807007076043875"} ,
    {30000.0, RUNNING, "7546985214563205"} ,
    {7850.0, RUNNING, "6582015630478503"},
    {300.0, RUNNING, "9856014752049530"},
    {22798.0, BLOCKED, "7569852310641035"},
    {36250.0, RUNNING, "8569854123470264"}
};

int transCounter = -1, dbIndex;
ST_transaction_t transaction[255];

//CHEACK THE VALIDATION OF THE PAN VALUE
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	int count = 0;
	while (accountsDB[count].primaryAccountNumber[0])
	{
		if (!strcmp(accountsDB[count].primaryAccountNumber, cardData->primaryAccountNumber))
		{
			dbIndex = count;
			*accountRefrence = accountsDB[count];
			return SERVER_OK;
		}
		count++;
	}
	accountRefrence =  NULL;
	return ACCOUNT_NOT_FOUND;
}

//CHECK IF THE ACCOUNT IS BLOCKED OR NOT
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state)
		return BLOCKED_ACCOUNT;
	return SERVER_OK;
}

//CHECK IS THE TRASACTION AMOUNT IS AVAIL IN THE BALANCE OR NOT
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
		return LOW_BALANCE;
	return SERVER_OK;
}

void listSavedTransactions(void)
{
	for(int i = 0; i<= transCounter; i++)
	{
		printf("\n#########################\n");
		printf("Transaction Sequence Number: %d\n", transaction[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transaction[i].terminalData.transactionDate );
		printf("Transaction Amount: %9.2f\n", transaction[i].terminalData.transAmount);
		printf("Transaction State: %d\n",transaction[i].transState );
		printf("Terminal Max Amount: %9.2f\n", transaction[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transaction[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transaction[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transaction[i].cardHolderData.cardExpirationDate);
		printf("#########################\n\n");
	}
}

//SAVING TRANSACTION
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_transState_t result = recieveTransactionData(transData);
	transData->transState = result;
	transData->transactionSequenceNumber = ++transCounter;
	transaction[transCounter] = *transData;
	if(!result)
		accountsDB[dbIndex].balance -= transData->terminalData.transAmount;
	listSavedTransactions();
	return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t refrence;

	ST_cardData_t cardData;
	EN_cardError_t isValidName = getCardHolderName(&cardData);
	if(isValidName)
	{
		printf("INVALID USER NAME !!\n");
		return SAVING_FAILED;
	}
	EN_cardError_t isValidExpiryDate = getCardExpiryDate(&cardData);
	if(isValidExpiryDate)
	{
		printf("WRONG FORMAT OF EXPIRY DATE !!\n");
		return SAVING_FAILED;
	}
	EN_cardError_t isValidPan = getCardPAN(&cardData);
	if(isValidExpiryDate)
	{
		printf("INVALID PIN NUMBER !!\n");
		return SAVING_FAILED;
	}	
	transData->cardHolderData = cardData;

	ST_terminalData_t terminalData;
	EN_terminalError_t isValidTranDate = getTransactionDate(&terminalData);
	if(isValidTranDate)
	{
		printf("INVALID FORMAT OF TRANSACTION DATE !!\n");
		return TRANSACTION_NOT_FOUND;
	}	
	EN_terminalError_t isExpiredCard = isCardExpired(&cardData, &terminalData);
	if(isExpiredCard)
	{
		printf("EXPIRED CARD!!\n");
		return TRANSACTION_NOT_FOUND;
	}	
	EN_terminalError_t isValidAmount = getTransactionAmount(&terminalData);
	if(isValidAmount)
	{
		printf("INVALID NUMBER !!\n");
		return TRANSACTION_NOT_FOUND;
	}	
	EN_terminalError_t isValidMax = setMaxAmount(&terminalData, 5000);
	if(isValidMax)
	{
		printf("INVALID NUMBER !!\n");
		return TRANSACTION_NOT_FOUND;
	}	
	EN_terminalError_t isBelowMax = isBelowMaxAmount(&terminalData);
	if(isBelowMax)
	{
		printf("ECXEED THE MAX VALUE !!\n");
		return TRANSACTION_NOT_FOUND;
	}	
	transData->terminalData = terminalData;

	EN_serverError_t isNotValid = isValidAccount(&cardData, &refrence);
	if (isNotValid)
		return FRAUD_CARD;

	EN_serverError_t isNotBlocked = isBlockedAccount(&refrence);
	if (isNotBlocked)
		return DECLINED_STOLEN_CARD;

	EN_serverError_t isNotSufficientAmount= isAmountAvailable(&terminalData, &refrence);
	if (isNotSufficientAmount)
		return DECLINED_INSUFFECIENT_FUND;

	return APPROVED;
}

void isValidAccountTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : isValidAccount\n\n");
	ST_cardData_t input1;
	getCardPAN(&input1);
	ST_accountsDB_t ref1;
	EN_cardError_t output1 = isValidAccount(&input1, &ref1);
	printf("Test Case 1:\nInput Data : 4563218950321795\nExpected Result : 3\nActual Result :%d\n", output1);
	printf("The PAN = %s\n\n", &ref1 ? ref1.primaryAccountNumber: NULL);
	ST_cardData_t input2;
	getCardPAN(&input2);
	ST_accountsDB_t ref2;
	EN_cardError_t output2 = isValidAccount(&input2, &ref2);
	printf("Test Case 2:\nInput Data : 7546985214563205\nExpected Result : 0\nActual Result :%d\n", output2);
	printf("The PAN = %s\n\n", ref2.primaryAccountNumber);
	ST_cardData_t input3;
	getCardPAN(&input3);
	ST_accountsDB_t ref3;
	EN_cardError_t output3 = isValidAccount(&input3, &ref3);
	printf("Test Case 3:\nInput Data : 8569854123470264\nExpected Result : 0\nActual Result :%d\n", output3);
	printf("The PAN = %s\n\n", ref3.primaryAccountNumber);
}

void isBlockedAccountTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : isBlockedAccount\n\n");
	ST_cardData_t cardData1;
	getCardPAN(&cardData1);
	ST_accountsDB_t ref1;
	isValidAccount(&cardData1, &ref1);
	EN_serverError_t output1 = isBlockedAccount(&ref1);
	printf("Test Case 1:\nInput Data : 8989374615436851\nExpected Result : 0\nActual Result :%d\n\n", output1);
	ST_cardData_t cardData2;
	getCardPAN(&cardData2);
	ST_accountsDB_t ref2;
	isValidAccount(&cardData2, &ref2);
	EN_serverError_t output2 = isBlockedAccount(&ref2);
	printf("Test Case 2:\nInput Data : 6582015630478503\nExpected Result : 0\nActual Result :%d\n\n", output2);
	ST_cardData_t cardData3;
	getCardPAN(&cardData3);
	ST_accountsDB_t ref3;
	isValidAccount(&cardData3, &ref3);
	EN_serverError_t output3 = isBlockedAccount(&ref3);
	printf("Test Case 3:\nInput Data : 7569852310641035\nExpected Result : 5\nActual Result :%d\n\n", output3);
	ST_cardData_t cardData4;
	getCardPAN(&cardData4);
	ST_accountsDB_t ref4;
	isValidAccount(&cardData4, &ref4);
	EN_serverError_t output4 = isBlockedAccount(&ref4);
	printf("Test Case 4:\nInput Data : 5807007076043875\nExpected Result : 5\nActual Result :%d\n\n", output4);
}

void isAmountAvailableTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : isAmountAvailable\n\n");
	ST_cardData_t cardData1;
	getCardPAN(&cardData1);
	ST_terminalData_t transaction1;
	getTransactionAmount(&transaction1);
	ST_accountsDB_t ref1;
	isValidAccount(&cardData1, &ref1);
	EN_serverError_t output1 = isAmountAvailable(&transaction1, &ref1);
	printf("Test Case 1:\nInput Data : 8989374615436851 && 3000\nExpected Result : 4\nActual Result :%d\n\n", output1);
	ST_cardData_t cardData2;
	getCardPAN(&cardData2);
	ST_terminalData_t transaction2;
	getTransactionAmount(&transaction2);
	ST_accountsDB_t ref2;
	isValidAccount(&cardData2, &ref2);
	EN_serverError_t output2 = isAmountAvailable(&transaction2, &ref2);
	printf("Test Case 2:\nInput Data : 8989374615436851 && 1500\nExpected Result : 0\nActual Result :%d\n\n", output2);
	ST_cardData_t cardData3;
	getCardPAN(&cardData3);
	ST_terminalData_t transaction3;
	getTransactionAmount(&transaction3);
	ST_accountsDB_t ref3;
	isValidAccount(&cardData3, &ref3);
	EN_serverError_t output3 = isAmountAvailable(&transaction3, &ref3);
	printf("Test Case 3:\nInput Data : 6582015630478503 && 8000\nExpected Result : 4\nActual Result :%d\n\n", output3);
	ST_cardData_t cardData4;
	getCardPAN(&cardData4);
	ST_terminalData_t transaction4;
	getTransactionAmount(&transaction4);
	ST_accountsDB_t ref4;
	isValidAccount(&cardData4, &ref4);
	EN_serverError_t output4 = isAmountAvailable(&transaction4, &ref4);
	printf("Test Case 4:\nInput Data : 6582015630478503 && 7000\nExpected Result : 0\nActual Result :%d\n\n", output4);
}

void saveTransactionTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : SaveTransaction\n\n");
	ST_transaction_t trans1;
	EN_serverError_t result1  = saveTransaction(&trans1);
	printf("The balance after trans: %9.2f\n", accountsDB[dbIndex].balance);
	ST_transaction_t trans2;
	EN_serverError_t result2  = saveTransaction(&trans2);
	printf("The balance after trans: %9.2f\n\n\n", accountsDB[dbIndex].balance);
}

void recieveTransactionDataTest(void)
{
	printf("Tester Name: Mohamed Ahmed Ibrahim\nFunction Name : SaveTransaction\n\n");
	ST_transaction_t trans1;
	EN_transState_t result1 = recieveTransactionData(&trans1);
	printf("Test Case 1:\nInput Data : Mohamed Ahmed Ibrahim & 11/23 & 7546985214563205 & 2000\nExpected Result : 0\nActual Result :%d\n\n", result1);
	ST_transaction_t trans2;
	EN_transState_t result2 = recieveTransactionData(&trans2);
	printf("Test Case 1:\nInput Data : Mohamed Ahmed Ibrahim & 11/23 & 7546985214563114 & 2000\nExpected Result : 3\nActual Result :%d\n\n", result2);
	ST_transaction_t trans3;
	EN_transState_t result3 = recieveTransactionData(&trans3);
	printf("Test Case 1:\nInput Data : Mohamed Ahmed Ibrahim & 11/23 & 5807007076043875 & 2000\nExpected Result : 2\nActual Result :%d\n\n", result3);
	ST_transaction_t trans4;
	EN_transState_t result4 = recieveTransactionData(&trans4);
}
