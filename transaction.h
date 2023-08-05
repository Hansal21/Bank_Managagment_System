#ifndef TRANSACTION_DETAILS
#define TRANSACTION_DETAILS
#include<time.h>
#include <bits/types.h>

struct transaction
{
    int accno;
    char type[20];
    long int amount;
    time_t ttime;
    long int balance;
};

#endif