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
#include<string.h>
#include"serverstrings.h"
#include"functions.h"

void newrequest(int cfd){
    int status,choice;
    status=write(cfd,MENU,sizeof(MENU));
    if(status==-1){
        perror("error while sending MENU");
    }
    status=read(cfd,&choice,sizeof(choice));
    if(status==-1){
        perror("error while reading choice");
    }
    switch(choice){
        case 1:
        adminfun(cfd);
        break;
        case 2:
        clientfun(cfd);
        break;
        // case 3:
        // createaccount(cfd);
        default:
        write(cfd,"Wrong choice\n*",15);
        // newrequest(cfd);
        break;
    }

}
void main(){
    int sfd,cfd,status,csize;
    struct sockaddr_in server,client;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Error in creating socket\n");
        exit(0);
    }
    server.sin_family=AF_INET;
    server.sin_port=htons(8088);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    status=bind(sfd,(struct sockaddr *)&server,sizeof(server));
    if(status==-1){
        perror("Error in binding...\n");
        exit(0);
    }
    status=listen(sfd,1);
    if(status==-1){
        perror("Error in listening...\n");
        exit(0);
    }
    printf("Server running and liesting for client\n");
    csize=(int)sizeof(client);
    while(1){
        cfd=accept(sfd,(struct sockaddr *)&client,&csize);
        if(cfd==-1){
            perror("Error in accepting connection...\n");
            exit(0);
        }
        if(!fork()){
            close(sfd);
            newrequest(cfd);
            close(cfd);
            exit(0);
        }
        else{
            close(cfd);
        }
    }
}
