
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

void main(){
    int fd,fd2,fd3;
    struct details customer;
    struct account acc;
    int t;
    char buf[20];
    fd=open("customer.dat",O_RDONLY);
    if(fd==-1) perror("fd");
    fd2=open("acounts.dat",O_RDONLY);
    if(fd2==-1) perror("fd2");
    fd3=open("id.dat",O_RDONLY);
    if(fd3==-1) perror("fd3");
    while(read(fd,&customer,sizeof(customer))!=0){
    //lseek(fd,3*sizeof(customer),SEEK_SET) 
    printf("%d ",customer.accno);
    printf("%s ",customer.name);
    printf("%c ",customer.gender);
    printf("%s ",customer.dob);
    printf("%d ",customer.userid);
    printf("%s \n",customer.password);
    }

    while(read(fd2,&acc,sizeof(acc))!=0){
    // lseek(fd2,2*sizeof(acc),SEEK_SET);
    
    printf("accno:%d ",acc.accno);
    printf("bal:%ld ",acc.balance);
    printf("isactive:%d ",acc.isactive);
    printf("isjoint:%d ",acc.isjoint);
    printf("user1:%d ",acc.owneruserid[0]);
    printf("user2:%d \n",acc.owneruserid[1]);
    }

    read(fd3,&t,sizeof(t));
    printf("%d \n",t);
}