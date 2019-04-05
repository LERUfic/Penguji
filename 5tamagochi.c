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

struct monsterData
{
    char monsterName[100];
    int monsterHunger;
    int monsterHealth;
    int monsterHygiene;
    int monsterFoodStock;
    int monsterBathTimer;
};

struct monsterData *monstat;

int monsterMaxHunger = 200;
int monsterMaxHygiene = 100;
int monsterMaxHealth = 300;
int enemyHealth = 100;

int gameState;
int gameOver;
int *foodStock;
char inputTemp;


void *renderFunction();

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


void *hungerFunction(){
    while(1){
        sleep(10);
        if(gameState != 1){
            monstat->monsterHunger -= 5;
            if(monstat->monsterHunger <= 0){
                system("clear");
                printf("%s died of hunger\n", monstat->monsterName);
                gameOver = 1;
            }
        }
    }
}

void *hygieneFunction(){
    while(1){
        sleep(30);
        if(gameState != 1){
            monstat->monsterHygiene -= 10;
            if(monstat->monsterHygiene <= 0){
                system("clear");
                printf("%s died of dirtiness\n", monstat->monsterName);
                gameOver = 1;
            }
        }
    }
}

void *bathFunction(){
    while(1){
        while(monstat->monsterBathTimer > 0){
            sleep(1);
            monstat->monsterBathTimer -=1;
        }
    }
}

void *regenFunction(){
    while(1){
        sleep(10);
        if(gameState == 0){
            monstat->monsterHealth += 5;
            if(monstat->monsterHealth > monsterMaxHealth){
                monstat->monsterHealth = monsterMaxHealth;
            }
        }
    }
}


int main(){
    key_t key = 4242;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0777);
    foodStock = shmat(shmid, NULL, 0);

    gameState = 0;
    gameOver = 0;

    pthread_t hungerthread, regenthread, hygienethread, baththread, inputthread,  renderthread, checkthread;
    monstat = malloc(sizeof(struct monsterData));

    monstat->monsterHealth = monsterMaxHealth;
    monstat->monsterHunger = monsterMaxHunger;
    monstat->monsterHygiene = monsterMaxHygiene;


    system("clear");
    printf("Enter your desired name for your monster (1 word pls) : \n");
    scanf("%s", monstat->monsterName);

    printf("Great! your monster's name is %s \n", monstat->monsterName);
    sleep(3);

    system("clear");


    if(pthread_create(&hungerthread,NULL,hungerFunction,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&regenthread,NULL,regenFunction,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&hygienethread,NULL,hygieneFunction,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&baththread,NULL,bathFunction,NULL)){
        printf("failed\n");
    }

    //if(pthread_create(&inputthread,NULL,inputFunction,NULL)){
        //printf("failed\n");
    //}
    if(pthread_create(&renderthread,NULL,renderFunction,NULL)){
        printf("failed\n");
    }
    //if(pthread_create(&checkthread,NULL,checkFunction,NULL)){
        //printf("failed\n");
    //}

    //main input thread

    inputTemp = 'a';

    while(gameOver != 1){
        inputTemp = getch();
        if(gameState == 0){
            if(inputTemp == '1'){
                if(monstat->monsterFoodStock > 0){
                    monstat->monsterFoodStock -= 1;
                    monstat->monsterHunger += 15;
                    if(monstat->monsterHunger > monsterMaxHunger){
                        monstat->monsterHunger = monsterMaxHunger;
                        system("clear");
                        printf("Your monster is really full!\n");
                    }
                }
                else{
                    system("clear");
                    printf("Stock makanan anda habis!\n");
                }
            }
            else if(inputTemp == '2'){
                if(monstat->monsterBathTimer == 0){
                    monstat->monsterBathTimer = 20;
                    monstat->monsterHygiene += 30;
                    if(monstat->monsterHygiene > monsterMaxHygiene){
                        monstat->monsterHygiene = monsterMaxHygiene;
                        system("clear");
                        printf("Your monster is as clean as it can be!\n");
                    }
                }
                else{
                    system("clear");
                    printf("Monster masih belum ingin mandi!\n");
                }
            }
            else if(inputTemp == '3'){
                system("clear");
                printf("Loading battle...\n");
                gameState = 1;
            }
            else if(inputTemp == '4'){
                system("clear");
                printf("Loading shop...\n");
                gameState = 2;
            }
            else if(inputTemp == '5'){
                system("clear");
                printf("Exiting...\n");
                gameOver = 1;
            }
        }
        else if(gameState == 1){
            printf("still here yo\n");
            if(inputTemp == '1'){
                enemyHealth -= 20;
                if(enemyHealth<=0){
                    system("clear");
                    printf("Your Monster Wins!\n");
                    enemyHealth = 100;
                    gameState = 0;
                }
                monstat->monsterHealth -= 20;  
                if(monstat->monsterHealth <= 0){
                    system("clear");
                    printf("%s was killed in battle\n", monstat->monsterName);
                    gameOver = 1;
                }
            }
            else if(inputTemp == '2'){
                system("clear");
                printf("Running from battle...\n");
                enemyHealth = 100;
                gameState = 0;
            }
        }
        else if(gameState == 2){
            printf("still here yo\n");
            if(inputTemp == '1'){
                if(*foodStock > 0){
                    *foodStock -= 1;
                    monstat->monsterFoodStock += 1;
                }
            }
            else if(inputTemp == '2'){
                system("clear");
                printf("Exiting shop...\n");
                gameState = 0;
            }
            
        }
    }

    shmdt(foodStock);
    shmctl(shmid, IPC_RMID, NULL);
    RestoreTerminal();
    exit(EXIT_SUCCESS);

}

void *renderFunction(){
    while(gameOver != 1){
        //printf("state: %d\n", gameState);
        if(gameState == 0){
            //printf("state: %d\n", gameState);
            printf("StanyMode\n");
            printf("Health : %d\n", monstat->monsterHealth);
            printf("Hunger : %d\n", monstat->monsterHunger);
            printf("Hygiene : %d\n", monstat->monsterHygiene);
            printf("Food in stock : %d\n", monstat->monsterFoodStock);;
            if(monstat->monsterBathTimer > 0){
                printf("Bath will be ready in %d s\n", monstat->monsterBathTimer );
            }
            else{
                printf("Bath is ready\n");
            }
            printf("Choices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
        }
        else if (gameState == 1){
            printf("BattleMode\n");
            printf("%s's Health : %d\n", monstat->monsterName, monstat->monsterHealth);
            printf("Enemy's Health : %d\n", enemyHealth);
            printf("Choices\n1. Attack\n2. Run\n");
        }
        else {
            printf("ShopMode\n");
            printf("Shop food stock: %d\n", *foodStock);
            printf("Your food stock: %d\n", monstat->monsterFoodStock);
            printf("Choices\n1. Buy\n2. Back\n");
        }
        sleep(1);
        system("clear");
     }
 }