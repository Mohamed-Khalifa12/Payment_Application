#include <stdio.h>
#include "app.h"

void appStart(void)
{
    while(1)
    {
        ST_transaction_t transData;

        ST_cardData_t cardData;
        EN_cardError_t isValidName = getCardHolderName(&cardData);
        if(isValidName)
        {
            printf("INVALID USER NAME !!\n");
            return;
        }
        EN_cardError_t isValidExpiryDate = getCardExpiryDate(&cardData);
        if(isValidExpiryDate)
        {
            printf("WRONG FORMAT OF EXPIRY DATE !!\n");
            return;
        }
        EN_cardError_t isValidPan = getCardPAN(&cardData);
        if(isValidExpiryDate)
        {
            printf("INVALID PIN NUMBER !!\n");
            return;
        }	
        transData.cardHolderData = cardData;

        ST_terminalData_t terminalData;
        EN_terminalError_t isValidTranDate = getTransactionDate(&terminalData);
        if(isValidTranDate)
        {
            printf("INVALID FORMAT OF TRANSACTION DATE !!\n");
            return;
        }	
        EN_terminalError_t isExpiredCard = isCardExpired(&cardData, &terminalData);
        if(isExpiredCard)
        {
            printf("EXPIRED CARD!!\n");
            return;
        }	
        EN_terminalError_t isValidAmount = getTransactionAmount(&terminalData);
        if(isValidAmount)
        {
            printf("INVALID NUMBER !!\n");
            return;
        }	
        EN_terminalError_t isValidMax = setMaxAmount(&terminalData, 5000);
        if(isValidMax)
        {
            printf("INVALID NUMBER !!\n");
            return;
        }	
        EN_terminalError_t isBelowMax = isBelowMaxAmount(&terminalData);
        if(isBelowMax)
        {
            printf("ECXEED THE MAX VALUE !!\n");
            return;
        }	
        transData.terminalData = terminalData;

        saveTransaction(&transData);
    }

}
