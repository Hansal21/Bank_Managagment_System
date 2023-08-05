#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS


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
#include "./customerfun.h"
#include "./account.h"
#include "./id.h"
#include "./serverstrings.h"

void delete(int cfd){
    // system("clear");
    // char clear[10];
    // strcpy(clear,"clear");
    // write(cfd,clear,sizeof(clear));
    // printf("delete\n");
    int tempaccno,fd;
    struct account acc;
    struct details customer1,customer2;
    struct flock lock;
    char buf[1000];
    // printf("Su che\n");
    write(cfd,"========Delete Account========\n\nEnter account number to be deleted:",68);
    // printf("Su che\n");
    EMBUF
    read(cfd,buf,sizeof(buf));
    // printf("Su che\n");
    tempaccno=atoi(buf);
    printf("%d",tempaccno);
    if(tempaccno<2022){
        write(cfd,"Account number is not valid\n*",30);
    }
    else{
    fd=open("acounts.dat",O_RDWR);

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(tempaccno-2022)*sizeof(acc);
    lock.l_len=sizeof(acc);
    lock.l_pid=getpid();
    
    // printf("Su che\n");
    fcntl(fd,F_SETLKW,&lock);
    // printf("Su che\n");
    // sleep(100);
    lseek(fd,(tempaccno-2022)*sizeof(acc),SEEK_SET);
    read(fd,&acc,sizeof(acc));
    
    if(acc.balance!=0){
        write(cfd,"account balance is not zero ,can not delete account\n*",54);
        close(fd);
    }else{
    acc.isactive=false;
    lseek(fd,(-1)*sizeof(acc),SEEK_CUR);
    write(fd,&acc,sizeof(acc));
    write(cfd,"\nAccount Deleted successfully\n^",31);
    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    fd=open("customer.dat",O_RDWR);

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(acc.owneruserid[1]-1)*sizeof(customer1);
    lock.l_len=sizeof(customer1);
    lock.l_pid=getpid();
    fcntl(fd,F_SETLKW,&lock);

    lseek(fd,(acc.owneruserid[1]-1)*sizeof(customer1),SEEK_SET);
    read(fd,&customer1,sizeof(customer1));
    customer1.userid=-1;
    strcpy(customer1.password,"a");
    lseek(fd,(-1)*sizeof(customer1),SEEK_CUR);
    write(fd,&customer1,sizeof(customer1));
    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    if(acc.isjoint){
    
    lock.l_start=(acc.owneruserid[0]-1)*sizeof(customer2);
    lock.l_len=sizeof(customer2);
    fcntl(fd,F_SETLKW,&lock);

    read(fd,&customer2,sizeof(customer2));
    customer2.userid=-1;
    strcpy(customer2.password,"a");
    lseek(fd,(-1)*sizeof(customer2),SEEK_CUR);
    write(fd,&customer2,sizeof(customer2));
    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    }


    }
    close(fd);
    }
}

void addaccount(int cfd){
    // system("clear");
    // write(cfd,"clear!",6);
    struct details customer,pcustomer,customer2;
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

    lock2.l_type=F_WRLCK;
    lock2.l_whence=SEEK_SET;
    lock2.l_start=0;
    lock2.l_len=0;
    lock2.l_pid=getpid();

    
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
    fcntl(fd2,F_SETLKW,&lock2);
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
    // system("clear");
    // write(cfd,"clear!",6);
    if(choice==2){
    EMBUF
    strcpy(buf,"\nEnter details about 1st account holder\n\nEnter Customer name:");
    write(cfd,buf,sizeof(buf));
    acc.isjoint=true;
    customer.userid=tempid-1;
    acc.owneruserid[1]=customer.userid;
    }
    else{
    if(acc.isjoint==true){
    EMBUF
    strcpy(buf,"\nEnter details about 2nd account holder\n\nEnter Customer name:");
    write(cfd,buf,sizeof(buf)); 
    }
    else{
    EMBUF
    strcpy(buf,"\nEnter Customer name:");
    write(cfd,buf,sizeof(buf));
    }
    // acc.isjoint=false;
    customer.userid=tempid;
    acc.owneruserid[0]=customer.userid;
    }
    EMBUF
    read(cfd,buf,sizeof(buf));
    strcpy(customer.name,buf);

    write(cfd,"\nEnter Customer DOB (ddmmyyyy):",32);
    EMBUF
    read(cfd,buf,sizeof(buf));
    strcpy(customer.dob,buf);

    write(cfd,"\nEnter Customer gender (M for male, F for female, O for others):",65);
    EMBUF
    read(cfd,buf,sizeof(buf));
    customer.gender=buf[0];
    
    while(1){
    write(cfd,"\nEnter Customer password:#",27);
    EMBUF
    read(cfd,buf,sizeof(buf));
    
    write(cfd,"\nConfirm Customer password:#",29);
    bzero(temp, sizeof(temp));
    read(cfd,temp,sizeof(temp));

    if(strcmp(buf,temp)==0) {
        strcpy(customer.password,buf);
        // lseek(fd2,0,SEEK_END);
        // write(fd2,&customer,sizeof(customer));
        if(choice==2)
        customer2=customer;
        break;
    }
    else{
    EMBUF
    strcpy(buf,"\nPassword didn't match please try again:\n@");
    write(cfd,buf,sizeof(buf));
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
    // system("clear");
    // write(cfd,"clear!",6);
    if(acc.isjoint==true){
    lseek(fd2,0,SEEK_END);
    write(fd2,&customer2,sizeof(customer2));
    }

    lseek(fd2,0,SEEK_END);
    write(fd2,&customer,sizeof(customer));

    EMBUF
    snprintf(buf,sizeof(buf),"\nAccount created successfully\n\nCustomer account no:%d\n\nCustomer Name:%s\nCustomer userid:%d\n^",customer.accno,customer.name,customer.userid);
    write(cfd,buf,sizeof(buf));
    if(acc.isjoint==true){
        lseek(fd2,(-1)*sizeof(customer),SEEK_CUR);
        
        read(fd2,&customer,sizeof(customer));
        EMBUF
    snprintf(buf,sizeof(buf),"\nCustomer Name:%s\nCustomer userid:%d\n^",customer2.name,customer2.userid);
    write(cfd,buf,sizeof(buf));
    }

    lock.l_type=F_UNLCK;
    fcntl(fd2,F_SETLK,&lock);
    lock2.l_type=F_UNLCK;
    fcntl(fd2,F_SETLK,&lock2);
    lock3.l_type=F_UNLCK;
    fcntl(fd3,F_SETLK,&lock3);

    

}

void modify(int cfd){
    // system("clear");
    // write(cfd,"clear!",6);
    int tempuserid,fd,choice;
    struct details customer,temp;
    struct flock lock;
    char buf[1000];
    write(cfd,"========Modify Account========\n\nEnter userid of customer to modify:",68);
    EMBUF
    read(cfd,buf,sizeof(buf));
    tempuserid=atoi(buf);
    if(tempuserid<1){
        write(cfd,"Account number is not valid\n*",30);
    }
    else{
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(tempuserid-1)*sizeof(customer);
    lock.l_len=sizeof(customer);
    lock.l_pid=getpid();
    
    fd=open("customer.dat",O_RDWR);
    if(fd==-1) perror("");
    fcntl(fd,F_SETLKW,&lock);
    
    lseek(fd,(-1)*sizeof(customer),SEEK_END);
    read(fd,&customer,sizeof(customer));
    printf("%d\n",customer.userid);
    if(customer.userid<tempuserid){
        write(cfd,"user id not found\n*",20);
    }
    else{
    lseek(fd,(tempuserid-1)*sizeof(customer),SEEK_SET);
    // while(read(fd,&customer,sizeof(customer)))
    read(fd,&customer,sizeof(customer));
    
    write(cfd,MODIFY_ADMIN,sizeof(MODIFY_ADMIN));
    EMBUF
    read(cfd,buf,sizeof(buf));
    choice=atoi(buf);
    switch(choice){
        case 1:
        write(cfd,"Enter new Name:",15);
        EMBUF
        read(cfd,buf,sizeof(buf));
        strcpy(customer.name,buf);
        break;
        case 2:
        write(cfd,"Enter new DOB:",14);
        EMBUF
        read(cfd,buf,sizeof(buf));
        strcpy(customer.dob,buf);
        break;
        case 3:
        write(cfd,"Enter new gender:",17);
        EMBUF
        read(cfd,buf,sizeof(buf));
        customer.gender=buf[0];
        
        break;
        default:
        write(cfd,"Wrong choice\n*",15);

    }
    lseek(fd,(-1)*sizeof(customer),SEEK_CUR);
    write(fd,&customer,sizeof(customer));
    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    }
    }
}

void search(int cfd){
    // printf("search");
    // system("clear");
    // write(cfd,"clear!",6);
    int tempaccno,fd,fd2;
    struct account acc;
    struct details customer;
    char buf[1000],buf2[500],buf3[200],joint[4],active[4];

    write(cfd,"========Search Account========\n\nEnter account number to view details:",70);
    EMBUF
    read(cfd,buf,sizeof(buf));
    tempaccno=atoi(buf);


    fd=open("acounts.dat",O_RDONLY);
    lseek(fd,(tempaccno-2022)*sizeof(acc),SEEK_SET);
    read(fd,&acc,sizeof(acc));
    if(acc.accno!=tempaccno){
        write(cfd,"Account not found\n*",20);
    }
    else{
    if(acc.isactive==true) strcpy(active,"Yes");
    else strcpy(active,"No");
    if(acc.isjoint==true) strcpy(joint,"Yes");
    else strcpy(joint,"No");
    EMBUF
    snprintf(buf3,sizeof(buf3),"Account no.:%d\nBalance:%ld\nJoint account:%s\nIs account active?%s\n",acc.accno,acc.balance,joint,active);
    // write(cfd,buf3,sizeof(buf3));
    close(fd);


    fd2=open("customer.dat",O_RDONLY);
    while(read(fd2,&customer,sizeof(customer))!=0){
    if(customer.accno==acc.accno){
        break;
    }
    }

    if(acc.isjoint==false){
    EMBUF
    snprintf(buf,sizeof(buf),"\n%sAccountholder name:%s\nAccount holder userid:%d\n*",buf3,customer.name,customer.userid);
    write(cfd,buf,sizeof(buf));
    }

    if(acc.isjoint==true){
    snprintf(buf2,sizeof(buf2),"\n%s\n1st Account holder name:%s\n1st Account holder userid:%d\n",buf3,customer.name,customer.userid);
    // write(cfd,buf,sizeof(buf));
        read(fd2,&customer,sizeof(customer));
    snprintf(buf,sizeof(buf),"%s\n2nd Account holder name:%s\n2nd Account holder userid:%d\n*",buf2,customer.name,customer.userid);
    write(cfd,buf,sizeof(buf));
    }
    close(fd2);
    }

}
#endif
