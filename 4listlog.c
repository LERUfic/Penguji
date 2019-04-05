#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


int syncer;
void *firstProcess(void *argv){
    syncer+=1;
    printf("in 1st process, creating file..., %d\n", syncer);
    while(syncer!=2) {}

    system("ps -aux | head -n 11 > /home/bolt/Documents/FolderProses1/SimpanProses1.txt");

    syncer+=1;
    printf("in 1st process pt 2, zipping..., %d\n", syncer);
    while(syncer!=4){}

    system("cd /home/bolt/Documents/FolderProses1/; zip KompresProses1.zip SimpanProses1.txt");

    syncer+=1;
    printf("in 1st process pt 3, removing..., %d\n", syncer);
    while(syncer!=6){}

    system("cd /home/bolt/Documents/FolderProses1/; rm SimpanProses1.txt");

    syncer+=1;
    printf("in 1st process pt 4, waiting 15 sec..., %d\n", syncer);
    while(syncer!=8){}
    sleep(15);
    printf("process 1, unzipping...\n");

    system("cd /home/bolt/Documents/FolderProses1/; unzip KompresProses1.zip");

}
void *secondProcess(void *argv){
    syncer+=1;
    printf("in 2nd process, creating file..., %d\n", syncer);
    while(syncer!=2) {}

    system("ps -aux | head -n 11 > /home/bolt/Documents/FolderProses2/SimpanProses2.txt");

    syncer+=1;
    printf("in 2nd process pt 2, zipping..., %d\n", syncer);
    while(syncer!=4){}

    system("cd /home/bolt/Documents/FolderProses2/; zip KompresProses2.zip SimpanProses2.txt");

    syncer+=1;
    printf("in 2nd process pt 3, removing..., %d\n", syncer);
    while(syncer!=6){}

    system("cd /home/bolt/Documents/FolderProses2/; rm SimpanProses2.txt");

    syncer+=1;
    printf("in 2nd process pt 4, waiting 15 sec..., %d\n", syncer);
    while(syncer!=8){}
    sleep(15);
    printf("process 2, unzipping...\n");

    system("cd /home/bolt/Documents/FolderProses2/; unzip KompresProses2.zip");
}

int main(){
    pthread_t process1, process2;
    syncer = 0;
    pthread_create(&process1, NULL, firstProcess, NULL);
    pthread_create(&process2, NULL, secondProcess, NULL);
    pthread_join(process1, NULL);
    pthread_join(process2, NULL);
    printf("done\n");
}