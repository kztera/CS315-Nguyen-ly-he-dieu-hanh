#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<sys/wait.h> 
#include<signal.h> 
#include<unistd.h>

#define PORT 8080

void handle_client(int new_socket){
	int valread;
	char buffer[1024] = { 0 };
	
	while((valread=read(new_socket , buffer, 1024))>0){
		
		int number1,number2,sum;

        sscanf(buffer,"%d %d",&number1,&number2);
        sum=number1+number2;

        char message[1024];
        sprintf(message,"%d",sum);

        send(new_socket , message , strlen(message) , 0 );
        printf("Sum sent\n");
		
		memset(buffer,'\0',sizeof(buffer));
		
	}
	
	close(new_socket);
	exit(0);
}

int main(int argc,char const *argv[]){
	int server_fd,new_socket,valread,opt=1;
	struct sockaddr_in address;
	int addrlen=sizeof(address);

	if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0){
	perror("socket failed");
	exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd,SOL_SOCKET,
    SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,3)<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        if((new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        pid_t pid=fork();
        if(pid==0){
            close(server_fd);
            handle_client(new_socket);
        }else{
            close(new_socket);
        }
    }
    return 0;
}