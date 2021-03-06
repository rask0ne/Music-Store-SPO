#define CRT_SECURE_NO_WARNINGS
#include <QCoreApplication>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <string>
#include <cstdlib>


using namespace std;


#define PIPE_NAME "/home/rask0ne/CLIENT/fifofile.txt"
#define BUFFSIZE 256                                                 // bufsize
#define QUEUE_PIPE "/home/rask0ne/Logging/queuefile"
#define QUEUE_FILE_SEMAPHORE "/queueFile_semaphore"
using namespace std;

bool logInFile()
{
    FILE *queueFile = fopen(QUEUE_PIPE,"r");
    if (queueFile == NULL)
    {
        printf("can't connection to queue Pipe \n");
        return false;
    }
    printf("READ LOG\n");

    char tempBuffer[BUFFSIZE];
    rewind(queueFile);
    fseek(queueFile,0,0);

    do
    {
        fgets(tempBuffer,BUFFSIZE,queueFile);
        if (feof(queueFile))
        {
            break;
        }

        if (tempBuffer == NULL)
        {
            printf("pizdec\n");
            break;
        }

        puts(tempBuffer);
    }while(1);

    rewind(queueFile);
    fclose(queueFile);
    return true;
}

char *itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(labs(n)))+1;
    if (n<0) len++; // room for negative sign '-'

    char *buf = (char*)calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%ld", n);
    return   buf;
}

void sendInt(int send,pid_t adress)
{
    sleep(1);
    while(send > 0)
    {

        if (send % 2 == 1)
        {
            kill(adress,SIGUSR1);                  // SIGUSR1 - "1"
        }
        else
        {
            kill(adress,SIGUSR2);                  // SIGUSR2 - "0"
        }
        send /= 2;
        sleep(1);
    }
    kill(adress,SIGINT);                           // SIGINT - END
    return;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // ----------------------LOGGER PROCESS------------------------------------


    FILE* pipe;
    int signo;
    sigset_t newmask;                                                // mask for signals
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);

    char* str = new char[BUFFSIZE];

    int parentID = 0;
    parentID = atoi(argv[1]);                                        // get ppid from a command line
    printf("command - %d\nnatural parent - %d\n",parentID,(int)getppid());

    sem_t *queueFileMutex;
    // semaphore
    queueFileMutex = sem_open(QUEUE_FILE_SEMAPHORE,0,0644,0);                 // creating semaphore (mutex)
    if(queueFileMutex == SEM_FAILED)
    {
        perror("unable to open semaphore");
        sem_unlink(QUEUE_FILE_SEMAPHORE);                                  // destroying semaphore
        return -1;
    }
    printf("sending - %d\n",(int)getpid());
    bool flag = true;

    pipe = fopen(PIPE_NAME, "w");
    str = itoa(getpid());
    cout << endl << itoa(getpid()) << endl;
    fputs(str, pipe);// get inquery
    fflush (pipe);
    fclose(pipe);
    printf("close\n");
    kill(parentID,SIGUSR1);
    //sendInt((int)getpid(),parentID);
    sigwait(&newmask,&signo);
    if (signo == SIGUSR1)
    {
        while(flag)
        {
            sem_wait(queueFileMutex);
            flag = logInFile();
            sem_post(queueFileMutex);
            printf("\n\n");
            sleep(5);
        }
    }
    sem_wait(queueFileMutex);
    sem_trywait(queueFileMutex);
    sem_close(queueFileMutex);
    sem_destroy(queueFileMutex);
    unlink(QUEUE_FILE_SEMAPHORE);
}
