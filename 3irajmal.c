#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

struct personData{
    int spiritSirazu;
    int wakeyAkumaru;
    int summonSirazu;
    int summonAkumaru;
    int countSirazu;
    int countAkumaru;
    int sleepSirazu;
    int sleepAkumaru;
    int isOver;
};

void *prosesSirazu(void *personData){
    struct personData *attribute = (struct personData*)personData;
    while(attribute->spiritSirazu > 0){
        //Iraj Ayo Tidur
        if(attribute->summonSirazu == 1){
            attribute->spiritSirazu -= 20;
            attribute->countSirazu += 1;
            attribute->summonSirazu = 0;
        }
        //3 times
        if(attribute->countSirazu == 3){
            attribute->countSirazu = 0;
            attribute->sleepAkumaru = 10;
            printf("Agmal Ayo Bangun disabled 10 s\n");
        }
        //Sirazu program sleep
        while(attribute->sleepSirazu > 0){
            attribute->sleepSirazu -= 1;
            attribute->summonSirazu = 0;
            printf("Fitur Iraj Ayo Tidur disabled %d s\n", attribute->sleepSirazu);
            sleep(1);
        }
    }
    printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
    attribute->isOver = 1;
}

void *prosesAkumaru(void *personData){
    struct personData *attribute = (struct personData*)personData;
    while(attribute->wakeyAkumaru < 100){
        //Agmal Ayo Tidur
        if(attribute->summonAkumaru == 1){
            attribute->wakeyAkumaru += 15;
            attribute->countAkumaru += 1;
            attribute->summonAkumaru = 0;
        }
        //3 times
        if(attribute->countAkumaru == 3){
            attribute->countAkumaru = 0;
            attribute->sleepSirazu = 10;
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
        }
        //Akumaru program sleep
        while(attribute->sleepAkumaru > 0){
            attribute->sleepAkumaru -= 1;
            attribute->summonAkumaru = 0;
            sleep(1);
            printf("Agmal Ayo Bangun disabled %d s\n", attribute->sleepAkumaru);
        }
    }
    printf("Agmal terbangun, mereka bangun pagi dan berolahraga\n");
    attribute->isOver = 1;
}

void *inputHandler(void *personData){
    struct personData *attribute = (struct personData*)personData;
    char input[99];
    while(1){
        fgets(input, 100, stdin);
        if(strcmp(input, "All Status\n") == 0){
            printf("Agmal WakeUp_Status = %d\n", attribute->wakeyAkumaru);
            printf("Iraj Spirit_Status = %d\n", attribute->spiritSirazu);
        }
        else if(strcmp(input, "Iraj Ayo Tidur\n") == 0){
            attribute->summonSirazu = 1;
        }
        else if(strcmp(input, "Agmal Ayo Bangun\n") == 0){
            attribute->summonAkumaru = 1;
        }
        
    }
}

int main(){
    pthread_t sirazu, akumaru, inputhandler;
    struct personData *data;
    data = malloc(sizeof(struct personData));
    data->wakeyAkumaru = 0;
    data->countAkumaru = 0;
    data->summonAkumaru = 0;
    data->sleepAkumaru = 0;
    data->spiritSirazu = 100;
    data->summonSirazu = 0;
    data->countSirazu = 0;
    data->sleepSirazu = 0;
    data->isOver = 0;
    pthread_create(&inputhandler, NULL, inputHandler, (void*)data);
    pthread_create(&sirazu, NULL, prosesSirazu, (void*)data);
    pthread_create(&akumaru, NULL, prosesAkumaru, (void*)data);
    while(1){
        if(data->isOver == 1){
            exit(EXIT_SUCCESS);
        }
    }
    pthread_join(inputhandler, NULL);
    pthread_join(sirazu, NULL);
    pthread_join(akumaru, NULL);

}