#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS


#include<stdio.h> 
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/ip.h>
#include<string.h>  
#include<stdbool.h> 
#include<string.h>
#include<errno.h> 
#include<time.h>


#include "./customer.h"
#include "./customerfun.h"
#include "./account.h"
#include "./transaction.h"
#include "./id.h"
#include "./serverstrings.h"


void deposit(int cfd,struct details customer){
    // printf("deposit\n");
    // system("clear");
    //write(cfd,"clear!",6);
    int accnumber,fd1,fd2,amount;
    char buf[1000];
    struct details temp;
    struct account temp2;
    struct transaction trans; 
    // struct time_t ct;
    struct flock lock,lock2;

    // lock.l_type=F_WRLCK;
    // lock.l_whence=SEEK_SET;
    // lock.l_start=(customer.userid-1)*sizeof(customer);
    // lock.l_len=sizeof(customer);
    // lock.l_pid=getpid();
    
    

    fd1=open("customer.dat",O_RDONLY);
    if(fd1==-1) perror("fd");
    // fcntl(fd1,F_SETLKW,&lock);
    while(read(fd1,&temp,sizeof(temp))!=0){
    if(customer.userid==temp.userid){
        break;
    }
    }
    fd2=open("acounts.dat",O_RDWR);
    if(fd2==-1) perror("fd");
    lock2.l_type=F_WRLCK;
    lock2.l_whence=SEEK_SET;
    lock2.l_start=(customer.accno-2022)*sizeof(temp2);
    lock2.l_len=sizeof(temp2);
    lock2.l_pid=getpid();

    fcntl(fd2,F_SETLKW,&lock2);
    while(read(fd2,&temp2,sizeof(temp2))!=0){
    if(customer.accno==temp2.accno){
        break;
    }
    }
    write(cfd,"\n========Deposite========\n\nEnter amount:",41);
    EMBUF
    read(cfd,buf,sizeof(buf));
    amount=atoi(buf);
    if(amount<1){
    write(cfd,"Transaction not valid\n*",24);
    }
    else{
    temp2.balance+=amount;
    write(cfd,"Transaction successfull\n*",26);
    lseek(fd2,(-1)*sizeof(temp2),SEEK_CUR);
    write(fd2,&temp2,sizeof(temp2));
    // lock.l_type=F_UNLCK;
    lock2.l_type=F_UNLCK;
    fcntl(fd2,F_SETLK,&lock2);
    // fcntl(fd1,F_SETLK,&lock);
    close(fd1);
    close(fd2);

    trans.accno=customer.accno;
    trans.amount=amount;
    trans.ttime=time(NULL);
    trans.balance=temp2.balance;
    strcpy(trans.type,"deposite");

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();

    fd1=open("transaction.dat",O_RDWR | O_CREAT,0744);
    fcntl(fd1,F_SETLKW,&lock);
    lseek(fd1,0,SEEK_END);
    write(fd1,&trans,sizeof(trans));
    lock.l_type=F_UNLCK;
    fcntl(fd1,F_SETLK,&lock);
    close(fd1);
    }
}
void withdraw(int cfd,struct details customer){
    // printf("withdraw\n");
    // system("clear");
    //write(cfd,"clear!",6);
    int accnumber,fd1,fd2,amount;
    char buf[1000];
    struct details temp;
    struct account temp2;
    struct transaction trans; 
    struct flock lock;


    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(customer.accno-2022)*sizeof(temp2);
    lock.l_len=sizeof(temp2);
    lock.l_pid=getpid();

    fd1=open("customer.dat",O_RDONLY);
    if(fd1==-1) perror("fd");
    while(read(fd1,&temp,sizeof(temp))!=0){
    if(customer.userid==temp.userid){
        break;
    }
    }
    fd2=open("acounts.dat",O_RDWR);
    fcntl(fd2,F_SETLKW,&lock);
    if(fd2==-1) perror("fd");
    while(read(fd2,&temp2,sizeof(temp2))!=0){
    if(customer.accno==temp2.accno){
        break;
    
    }
    }
     write(cfd,"\n========Withdraw========\n\nEnter amount:",41);
    EMBUF
    read(cfd,buf,sizeof(buf));
    amount=atoi(buf);
    if(amount<1){
    write(cfd,"Transaction not valid\n*",24);
    }
    else{
    if(amount>temp2.balance){
    write(cfd,"Transaction Failed,Insuffient balance\n*",40);
    }
    else{
    temp2.balance-=amount;
    write(cfd,"Transaction successfull\n*",26);
    lseek(fd2,(-1)*sizeof(temp2),SEEK_CUR);
    write(fd2,&temp2,sizeof(temp2));
    }
    lock.l_type=F_UNLCK;
    fcntl(fd2,F_SETLK,&lock);
    close(fd1);
    close(fd2);


    trans.accno=customer.accno;
    trans.amount=amount;
    trans.ttime=time(NULL);
    trans.balance=temp2.balance;
    strcpy(trans.type,"withdraw");

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();

    fd1=open("transaction.dat",O_RDWR | O_CREAT,0744);
    fcntl(fd1,F_SETLKW,&lock);
    lseek(fd1,0,SEEK_END);
    write(fd1,&trans,sizeof(trans));
    lock.l_type=F_UNLCK;
    fcntl(fd1,F_SETLK,&lock);
    close(fd1);
    }

}
void balancecheck(int cfd,struct details customer){
    // printf("balancecheck\n");
    // system("clear");
    //write(cfd,"clear!",6);
    int accnumber,fd1,fd2;
    char buf[1000];
    struct details temp;

    struct account temp2;

    fd1=open("customer.dat",O_RDONLY);
    if(fd1==-1) perror("fd");
    while(read(fd1,&temp,sizeof(temp))!=0){
    if(customer.userid==temp.userid){
        break;
    }
    }
    fd2=open("acounts.dat",O_RDWR);
    if(fd2==-1) perror("fd");
    while(read(fd2,&temp2,sizeof(temp2))!=0){
    if(customer.accno==temp2.accno){
        break;
    
    }
    }
    // printf("%ld",temp2.balance);
    EMBUF
    snprintf(buf,sizeof(buf),"\n========Balance check========\n\nYour balance:%ld\n*",temp2.balance);
    write(cfd,buf,sizeof(buf));
    close(fd1);
    close(fd2);
}
void passwordchange(int cfd,struct details customer){
    // system("clear");
    //write(cfd,"clear!",6);
    // printf("passwordchange\n");
    char buf[1000],temp2[100];
    int fd1;
    struct details temp;
    fd1=open("customer.dat",O_RDWR);
    if(fd1==-1) perror("fd");
    while(read(fd1,&temp,sizeof(temp))!=0){
    if(customer.userid==temp.userid){
        break;
    }
    }
    
    while(strcmp(buf,customer.password)!=0){
    write(cfd,"\n========Password Change========\n\nEnter your current password:#",64);
    EMBUF
    read(cfd,buf,sizeof(buf));
    if(strcmp(buf,customer.password)!=0)
     write(cfd,"Wrong password!\n^",17);
    }
    

    if(strcmp(buf,customer.password)==0){
        
    while(1){
    write(cfd,"Enter new password:#",20);
    EMBUF
    read(cfd,buf,sizeof(buf));
    
    write(cfd,"Confirm new password:#",22);
    bzero(temp2, sizeof(temp2));
    read(cfd,temp2,sizeof(temp2));

    if(strcmp(buf,temp2)==0) {
        strcpy(customer.password,buf);
        lseek(fd1,(-1)*sizeof(customer),SEEK_CUR);
        write(fd1,&customer,sizeof(customer));
        write(cfd,"password change successfully\n*",30);
        break;
    }
    else{
    write(cfd,"Password didn't match please try again:\n@",40);
    read(cfd,buf,sizeof(buf));
    EMBUF
    }
    }
    }
    else{
    write(cfd,WRONG_PASS,sizeof(WRONG_PASS));
    exit(0);
    }
    // lseek(fd1,(-1)*sizeof(temp),SEEK_CUR);
    // write(fd1,&temp,sizeof(temp));
    close(fd1);
    close(cfd);

}
void viewdetails(int cfd,struct details customer){
    // system("clear");
    //write(cfd,"clear!",6);
    char buf[1000];
    // printf("viewdetails\n");
    int accnumber,fd1,fd2;
    struct details temp;
    
    fd1=open("customer.dat",O_RDONLY);
    if(fd1==-1) perror("fd");
    // while(read(fd1,&temp,sizeof(temp))!=0){
    // if(customer.userid==temp.userid){
    //     break;
    // }
    // }
    lseek(fd1,(customer.userid-1)*sizeof(customer),SEEK_SET);
    snprintf(buf,sizeof(buf),"\n========View Details========\n\nName: %s\nAcc. no.:%d\nDOB:%s\nGender:%c\n*",customer.name,customer.accno,customer.dob,customer.gender);
    write(cfd,buf,sizeof(buf));
    close(fd1);
    close(cfd);
    

}
void viewtransaction(int cfd,struct details customer){
    struct transaction trans;
    // struct details customer2;
    // struct account acc;
    char buf[200];

    int fd;
    fd=open("transaction.dat",O_RDONLY);
    // write(cfd,"\n========Transactions========\n\n^",33);
    while(read(fd,&trans,sizeof(trans))!=0){

        if(trans.accno==customer.accno){
            snprintf(buf,sizeof(buf),"\nAccountno:%d    Amount:%ld    Type:%s     Balance:%ld     Time:%s^",trans.accno,trans.amount,trans.type,trans.balance,ctime(&trans.ttime));
            write(cfd,buf,sizeof(buf));
            // printf("Accountno:%d ",trans.accno);
            // printf("Amount:%ld ",trans.amount);
            // printf("Type:%s ",trans.type);
            // printf("Time:%s \n",ctime(&trans.ttime));
        }
    }
    write(cfd,"*",2);
}
#endif