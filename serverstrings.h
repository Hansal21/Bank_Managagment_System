#ifndef SERVER_STRINGS
#define SERVER_STRINGS

#define EMBUF bzero(buf, sizeof(buf));
#define EMWBUF bzero(wbuf, sizeof(wbuf));
#define MENU "\n\nWelcome to Dhandha's Bank\n\n1. Admin Login\n\n2. Customer Login\n\nPlease enter choice:"
#define ADMIN_WELCOME "\n========Admin Login========\n\nHi Admin, Please Enter your userid:"
#define CLIENT_WELCOME "\n========Customer Login========\n\nHi Client, Please enter your userid:"
#define ENTER_PASSWORD "\nPlease enter your password:#"
#define LOGIN_SUCCESS "\nLogin successfull\n^"
#define WRONG_PASS "wrong password*"
#define WRONG_USERID "wrong userid"
#define ADMIN_FUN_MENU "\n========Admin functions========\n\n1. Create new account\n2. Delete account\n3. Modify account\n4. Search account\n5. Log out\n\nPease enter choice:"
#define JOINT_OR_NOT "\n========Create Account========\n\n1. Normal account\n2. Joint account\n\nPlease enter your choice:"
#define BASE 2022

#define CUSTOMER_FUN_MENU "\n========Customer functions========\n\n1. Deposite\n2. Withdraw\n3. Balance Check\n4. Password change\n5. View details\n6. View Transactions\n7. Exit\n\nPlease Enter your choice:"
#define MODIFY_ADMIN "What do you want to change?\n1. Name\n2. DOB\n3. Gender\nPease enter your choice:"

#endif