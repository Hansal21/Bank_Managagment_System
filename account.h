#ifndef ACCOUNTS_DETAILS
#define ACCOUNTS_DETAILS
struct account{
    int accno;
    bool isjoint;
    bool isactive;
    long int balance;
    int owneruserid[2];
};
#endif