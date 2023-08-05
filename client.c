#include<stdio.h> 
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/ip.h>
#include<string.h>  
#include<stdbool.h> 
#include<errno.h> 
#include"serverstrings.h"
#define EMBUF bzero(buf, sizeof(buf));
void sendrequest(int sfd){
    int status,choice,i;
    char buf[200];
    status=read(sfd,buf,sizeof(buf));
    if(status==-1){
        perror("error reading msg from server...\n");
    }
    printf("%s",buf);
    scanf("%d",&choice); //entering choice
    if(choice!=1 && choice!=2){
    printf("\nInvalid choice\n");
        exit(0);
    }
    status=write(sfd,&choice,sizeof(choice));
    if(status==-1){
        perror("error sending choice to server...\n");
    }
    // system("clear");
    if(choice!=3){
    EMBUF
    status=read(sfd,buf,sizeof(buf));
    if(status==-1){
        perror("error after sending choice..\n");
    }
    printf("%s",buf);
    scanf(" %[^\n]",buf); //user id
    write(sfd,buf,sizeof(buf));
    EMBUF
    status=read(sfd,buf,sizeof(buf));
    if(status==-1){
        perror("error while reading from server..\n");
    }
   
        
    if(strcmp(buf,WRONG_USERID)==0) {
        for(i=0;i<strlen(buf);i++)
        printf("%c",buf[i]);
        printf("\n");
        close(sfd);
        exit(0);
    }
    else{
        for(i=0;i<strlen(buf)-1;i++)
        printf("%c",buf[i]);
    }
    EMBUF
    strcpy(buf,getpass(buf)); //pass
    write(sfd,buf,sizeof(buf));
    EMBUF
    status=read(sfd,buf,sizeof(buf));
    if(status==-1){
        perror("error while reading from server..\n");
    }
    // system("clear");
    if(strcmp(buf,WRONG_PASS)==0) {
        for(i=0;i<strlen(buf)-1;i++)
        printf("%c",buf[i]);
        printf("\n");
        close(sfd);
        exit(0);
    }else
    printf("%s",buf); //login success+admin menu
    EMBUF
    scanf(" %[^\n]",buf);
    write(sfd,buf,sizeof(buf));
    }
    EMBUF
    while(read(sfd,buf,sizeof(buf))!=0){
        // printf("hhhhs\n");
        // char clear[10];
        // strcpy(clear,buf);
        // if(strcmp("clear",clear)==0){
        // //   system("clear");  
        // // read(sfd,buf,sizeof(buf));
        // printf("hhhh\n");
        // EMBUF
        // }
        if(strchr(buf,'@')!=NULL){
        // printf("elsebuf 1");
            printf("%s",buf);
            write(sfd,"a",1);
            EMBUF
        }
        else if(strchr(buf,'#')!=NULL){
        // printf("elsebuf 2");
            for(i=0;i<strlen(buf)-1;i++)
            printf("%c",buf[i]);
            EMBUF
            strcpy(buf,getpass(buf));
            // printf("aa mlyu %s",buf);
            write(sfd,buf,sizeof(buf));
            EMBUF
        }
        else if(strchr(buf,'^')!=NULL){
        // printf("elsebuf 3");
            for(i=0;i<strlen(buf)-1;i++)
            printf("%c",buf[i]);
            
        }
        else if(strchr(buf,'*')!=NULL){
        // printf("elsebuf 3");
            for(i=0;i<strlen(buf)-1;i++)
            printf("%c",buf[i]);
            exit(0);
        }
        else{
        // printf("elsebuf 4\n");
        printf("%s",buf);
        scanf(" %[^\n]",buf);
        write(sfd,buf,sizeof(buf));
        
        
        EMBUF
        }
        EMBUF
    }


}
void main(){
    int sfd,status;
    struct sockaddr_in server;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Error in creating socket\n");
        exit(0);
    }
    server.sin_family=AF_INET;
    server.sin_port=htons(8088);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    status=connect(sfd,(struct sockaddr *)&server,sizeof(server));
    if(status==-1){
        perror("Error while connecting to server\n");
        close(sfd);
        exit(0);
    }
    system("clear");
    sendrequest(sfd);
}
