#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/shm.h>

char buffer[1024] = {0};
int sock = 0, valread;

void *processFunction(void *argv){
    char item[100];
    while(1){
        scanf("%s", item);
        send(sock , item , strlen(item) , 0 );
    }
}

void *outputFunction(){
    while(1){
        memset(buffer, '\0', sizeof(buffer));
        valread = recv(sock, buffer, 1024, 0);
        if(valread == 0)
            exit(EXIT_SUCCESS);
        printf("%s\n",buffer );
    }
}


int main(){

    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5151);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    pthread_t processthread, outputthread;

    if(pthread_create(&processthread,NULL, processFunction,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&outputthread,NULL,outputFunction,NULL)){
        printf("failed\n");
    }

    pthread_join(processthread, NULL);
    return 0;
}