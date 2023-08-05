
#ifndef FUNCTIONS
#define FUNCTIONS

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
#include "./adminfun.h"
#include "./customerfun.h"
#include "./account.h"
#include "./id.h"
#include "./serverstrings.h"



void fun2(){
    
}
void createaccount(int cfd){
    struct details customer,pcustomer;
    struct account acc;
    struct id idvar;
    struct flock lock,lock2,lock3;
    bool flag=false;

    int choice,fd1,fd2,fd3,tempid,i,c;
    char buf[200],temp[200];

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();

    
    fd1=open("id.dat",O_RDWR | O_CREAT,0744);
    
    fcntl(fd1,F_SETLKW,&lock);

    if(read(fd1,&idvar,sizeof(idvar))==0){
        tempid=0;
    }
    else{
        tempid=idvar.idno;
    }

    lock.l_type=F_UNLCK;
    fcntl(fd1,F_SETLK,&lock);
    
    
    
    
    fd2=open("customer.dat",O_RDWR | O_CREAT,0744);
    if(read(fd2,buf,1)==0){
        customer.accno=BASE+0;
    }
    else{
    fcntl(fd2,F_SETLKW,&lock);
    lseek(fd2,(-1)*sizeof(customer),SEEK_END);
    read(fd2,&pcustomer,sizeof(pcustomer));
    customer.accno=pcustomer.accno+1;
    }
    fd3=open("acounts.dat",O_RDWR | O_CREAT,0744);
    fcntl(fd3,F_SETLKW,&lock3);
    
    acc.accno=customer.accno;
    acc.isactive=true;
    acc.balance=0;
    acc.isjoint=false;

    write(cfd,JOINT_OR_NOT,sizeof(JOINT_OR_NOT));
    read(cfd,buf,sizeof(buf));
    choice=atoi(buf);
    if(choice!=1 && choice!=2){
        write(cfd,"Wrong choice\n*",15);
        close(cfd);
        return;
    }
    if(choice==2){
        tempid+=2;
    }
    else{
        tempid++;
    }
    acc.owneruserid[1]=-1;
    do{
    
    if(choice==2){
    write(cfd,"Enter details about 1st account holder\nEnter your name:",56);
    acc.isjoint=true;
    customer.userid=tempid-1;
    acc.owneruserid[1]=customer.userid;
    }
    else{
    if(acc.isjoint==true)
    write(cfd,"Enter details about 2nd account holder\nEnter your name:",56);
    else
    write(cfd,"Enter your name:",16);

    // acc.isjoint=false;
    customer.userid=tempid;
    acc.owneruserid[0]=customer.userid;
    }
    EMBUF
    read(cfd,buf,sizeof(buf));
    strcpy(customer.name,buf);

    write(cfd,"Enter your DOB (ddmmyyyy):",26);
    EMBUF
    read(cfd,buf,sizeof(buf));
    strcpy(customer.dob,buf);

    write(cfd,"Enter your gender (M for male, F for female, O for others):",60);
    EMBUF
    read(cfd,buf,sizeof(buf));
    customer.gender=buf[0];
    
    while(1){
    write(cfd,"Enter new password:#",20);
    EMBUF
    read(cfd,buf,sizeof(buf));
    
    write(cfd,"Confirm new password:#",22);
    bzero(temp, sizeof(temp));
    read(cfd,temp,sizeof(temp));

    if(strcmp(buf,temp)==0) {
        strcpy(customer.password,buf);
        lseek(fd2,0,SEEK_END);
        write(fd2,&customer,sizeof(customer));
        break;
    }
    else{
    write(cfd,"Password didn't match please try again:\n@",40);
    read(cfd,buf,sizeof(buf));
    EMBUF
    }

    }
   
    choice--;
    }while(choice>0);

    lseek(fd1,0,SEEK_SET);
    write(fd1,&tempid,sizeof(tempid));

    lseek(fd3,0,SEEK_END);
    write(fd3,&acc,sizeof(acc));
    
    // lseek(fd2,0,SEEK_END);
    // write(fd2,&customer,sizeof(customer));

    lock.l_type=F_UNLCK;
    fcntl(fd2,F_SETLK,&lock);
    lock3.l_type=F_UNLCK;
    fcntl(fd3,F_SETLK,&lock3);

    write(cfd,"Account created successfully\n^",31);
    


}
void adminfun(int cfd){
    // system("clear");
    int status,choice;
    char buf[200];
    char userid[]="a";
    char password[]="a";
    // //write(cfd,"clear!",6);
    status=write(cfd,ADMIN_WELCOME,sizeof(ADMIN_WELCOME));
    if(status==-1) perror("error in admin fun");
    read(cfd,buf,sizeof(buf));
    if(strcmp(userid,buf)==0){
        write(cfd,ENTER_PASSWORD,sizeof(ENTER_PASSWORD));
        EMBUF
        read(cfd,buf,sizeof(buf));
        if(strcmp(buf,password)==0){
            // write(cfd,LOGIN_SUCCESS,sizeof(LOGIN_SUCCESS));
            // system("clear");
            // //write(cfd,"clear!",6);
            write(cfd,ADMIN_FUN_MENU,sizeof(ADMIN_FUN_MENU));
            EMBUF
            read(cfd,buf,sizeof(buf));
            choice=atoi(buf);
            switch(choice){
                case 1:
                addaccount(cfd);
                break;
                case 2:
                delete(cfd);
                break;
                case 3:
                modify(cfd);
                break;
                case 4:
                search(cfd);
                break;
                case 5:
                
                break;
                case 6:
                break;
            }
            
        }
        else
        write(cfd,WRONG_PASS,sizeof(WRONG_PASS));


    }
    else{
        write(cfd,WRONG_USERID,sizeof(WRONG_USERID));
    }
    
}
void clientfun(int cfd){
    int status,fd,choice;
    char buf[200];
    struct details customer;
    bool found=false;
    write(cfd,CLIENT_WELCOME,sizeof(CLIENT_WELCOME));
    EMBUF
    read(cfd,buf,sizeof(buf));
    choice=atoi(buf);
    fd=open("customer.dat",O_RDONLY);
    if(fd==-1) perror("fd");
    while(read(fd,&customer,sizeof(customer))!=0){
    if(customer.userid==choice){
        found=true;
        break;
    }
    }

    if(found){
        write(cfd,ENTER_PASSWORD,sizeof(ENTER_PASSWORD));
        EMBUF
        read(cfd,buf,sizeof(buf));
        // printf("%s %s %ld %ld",buf,customer.password,sizeof(buf),sizeof(customer.password));
        if(strcmp(buf,customer.password)==0){

            //write(cfd,LOGIN_SUCCESS,sizeof(LOGIN_SUCCESS));
            write(cfd,CUSTOMER_FUN_MENU,sizeof(CUSTOMER_FUN_MENU));
            EMBUF
            read(cfd,buf,sizeof(buf));
            choice=atoi(buf);
            switch(choice){
                case 1:
                deposit(cfd,customer);
                break;
                case 2:
                withdraw(cfd,customer);
                break;
                case 3:
                balancecheck(cfd,customer);
                break;
                case 4:
                passwordchange(cfd,customer);
                break;
                case 5:
                viewdetails(cfd,customer);
                break;
                case 6:
                viewtransaction(cfd,customer);
                break;
                case 7:
                break;
            }
        }
        else
        write(cfd,WRONG_PASS,sizeof(WRONG_PASS));


    }
    else{
        write(cfd,WRONG_USERID,sizeof(WRONG_USERID));
    }
}

#endif