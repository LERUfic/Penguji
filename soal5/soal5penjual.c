#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>

int status=0;
int *value;
char a = '\0';

struct termios old, new;
/* Read 1 character - echo defines echo mode */
char getch() 
{
    char ch;
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    // new.c_lflag |= ECHO; /* set echo mode */
    new.c_lflag &= ~ECHO; /* set no echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

void* stok()
{
    while(1)
    {
        printf("Shop\nFood Stock : %d\nChoices\n1.Restock\n2.Exit\n",*value);
        // printf("%d\n",a);
        sleep(1);
        system("clear");
    }
}

int main(int argc, char const *argv[]) {

    key_t key = 1234;
    int mem = shmget(key,sizeof(int),IPC_CREAT|0666);
    value = shmat(mem,NULL,0);
    pthread_t t;
    pthread_create(&t,NULL,stok,NULL);
    while ((a = getch())) {
        if(a == '1') *value = *value + 1;
        if(a == '2') break;
    }
    if(a=='2') exit;
    return 0;
}