#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    int number1, number2;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

	while(1){
		printf("Enter two numbers (enter 'q' to quit): ");
		if(scanf("%d %d", &number1,&number2)!=2){
			break;
		}

		char message[1024];
		sprintf(message,"%d %d",number1,number2);

		send(client_fd , message , strlen(message) , 0 );
		
		valread = read( client_fd , buffer, 1024);
		printf("Sum: %s\n", buffer);
	}
	
	close(client_fd);
	return 0;
}