
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

#include "./customer.h"
#include "./account.h"
#include "./serverstrings.h"
#include "./transaction.h"


void main(){
    int fd;
    struct transaction trans;
    
    int t;
    char buf[20];
    fd=open("transaction.dat",O_RDONLY);
    if(fd==-1) perror("fd");
    while(read(fd,&trans,sizeof(trans))!=0){
    //lseek(fd,3*sizeof(customer),SEEK_SET) 
    printf("Accountno:%d ",trans.accno);
    printf("Amount:%ld ",trans.amount);
    printf("Type:%s ",trans.type);
    printf("Time:%s \n",ctime(&trans.ttime));
   
    // }

    // while(read(fd2,&acc,sizeof(acc))!=0){
    // // lseek(fd2,2*sizeof(acc),SEEK_SET);
    
    // printf("accno:%d ",acc.accno);
    // printf("bal:%ld ",acc.balance);
    // printf("isactive:%d ",acc.isactive);
    // printf("isjoint:%d ",acc.isjoint);
    // printf("user1:%d ",acc.owneruserid[0]);
    // printf("user2:%d \n",acc.owneruserid[1]);
    // }

    // read(fd3,&t,sizeof(t));
    // printf("%d \n",t);
}
}