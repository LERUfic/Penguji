#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <math.h>

int gameOver;
int *foodStock;
char inputTemp;


struct termios old={0};
struct termios new={0};
char getch(){
    char buf=0;
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    new = old;
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }

void RestoreTerminal(){
    tcsetattr(0, TCSANOW, &new);
    tcsetattr(0, TCSADRAIN, &new);
 }

void *renderFunction(){
    while(1){
        printf("FoodShop\n");
        printf("Food stock : %d\n", *foodStock);
        printf("Choices\n1. Restock\n2. Exit\n");
        sleep(1);
        system("clear");
    }
}

 int main(){
    key_t key = 4242;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0777);
    foodStock = shmat(shmid, NULL, 0);

    *foodStock = 0;
    gameOver = 0;
    pthread_t renderthread;

    if(pthread_create(&renderthread,NULL,renderFunction,NULL)){
        printf("failed\n");
    }

    system("clear");

    while(gameOver != 1){
        inputTemp = getch();
        if(inputTemp == '1'){
            system("clear");
            printf("added one food to the stock!\n");
            *foodStock += 1;
        }
        else if(inputTemp == '2'){
            gameOver = 1;
        }
    }

    shmdt(foodStock);
    shmctl(shmid, IPC_RMID, NULL);
    RestoreTerminal();
    exit(EXIT_SUCCESS);
 }