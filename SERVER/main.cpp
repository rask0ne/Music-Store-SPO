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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "shop.h"
#include <stdlib.h>

#define PIPE_NAME "/home/rask0ne/CLIENT/fifofile.txt"
#define SEMAPHORE_NAME "/my_named_semaphore"

using namespace std;

#define BUFFSIZE 256
#define CLIENT_COUNT 3

void cleanString(char* str){

    for(int i = 0; i < strlen(str); i++)
        str[i] = '\0';

}

int getClientID(char *str)
{
    char c;
    int sum = 0;
    for (int i = 0; i < 4;i++)
    {
        c = str[i];
        if (c == '\0')
            break;
        sum += (int)c -'0';
        if(i + 1 < 4)
            sum *= 10;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    pid_t pid;
    char *str = new char[BUFFSIZE];
    int signo;                                     // set the mask for getting signals
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);                  // add signals in mask
    sigaddset(&newmask, SIGUSR2);
    sigaddset(&newmask, SIGINT);
    sigprocmask(SIG_BLOCK, &newmask, NULL);


    unlink(PIPE_NAME);
    mkfifo(PIPE_NAME, O_RDWR);                                       // сreating pipe for communicating between clients and server
    unlink(PIPE_NAME);                                      // сreating pipe for communicating between clients and server


    sem_t *mutex;                                                    // declare semaphore
    int value = 0;
    mutex = sem_open(SEMAPHORE_NAME,O_CREAT,0644,0);                 // creating semaphore (mutex)
    if(mutex == SEM_FAILED)
    {
         perror("unable to create semaphore");
         sem_unlink(SEMAPHORE_NAME);                                  // destroying semaphore
         return -1;
    }
    sem_post(mutex);                                                 // release semaphore
    sem_getvalue(mutex,&value);
    printf("semaphore - %d\n",value);
    char buf[100];
    for(int i = 0; i < CLIENT_COUNT; i++)
    {

         cout << "Client process has been created" << endl;
         strcpy(buf,"gnome-terminal -x sh -c 'exec /home/rask0ne/build-CLIENT-Desktop-Debug/CLIENT ");
         strcat(buf,std::to_string((int)getpid()).c_str());           // add pid to command argument
         strcat(buf,"'");
         system(buf);                                                 // create client
         printf("%d\n",i);
         printf("Server - %d\n", (int)getpid());                      // server pid
    }

    printf("after create\n");
    Shop* res = new Shop(10, 15, 20, 25);
    int clientClose = CLIENT_COUNT;
    while (clientClose > 0)                                          // while not all was closed
    {
        sigwait(&newmask, &signo);                 // wait child process
        if (signo == SIGUSR1)
        {

            if(!res->connectToPipe(PIPE_NAME, "r")){                 // try to connect to pipe
                  printf("shop can't connect\n");
                  return -1;
            }                                        // close 1 end of pipe
            sleep(1);
            printf("alive\n");
            fread(str,1,BUFFSIZE, res->pipe);        // read from pipe in buffer
            fflush (res->pipe);
            int clientID = getClientID(str);
            pid = clientID;

            cout << "!!!!From client!!!!: " << clientID << endl;


                str = res->showGenres();

               res->closeConnectionToPipe();
               res->connectToPipe(PIPE_NAME, "w");
                fputs(str,res->pipe);             // write in pipe
                fflush (res->pipe);
                res->closeConnectionToPipe();
                kill((pid_t)clientID, SIGUSR1);
                sigwait(&newmask, &signo);
                if(signo == SIGUSR1){
                    cleanString(str);
                    res->connectToPipe(PIPE_NAME, "r");
                    fflush (res->pipe);
                    memset(str, '\0', sizeof(str));
                    fread(str,1,BUFFSIZE, res->pipe);

                    res->closeConnectionToPipe();
                    cout << "!!!!From client!!!!: " << str << endl;       // if extension is supporte             // send 1 SIGNAL

                    if(!strcmp(str, "rock") || !strcmp(str, "blues") || !strcmp(str, "jazz") || !strcmp(str, "trip-hop"))
                    {

                        res->connectToPipe(PIPE_NAME, "w");
                        string genre(str);
                        cleanString(str);
                        memset(str, '\0', sizeof(str));
                        str = res->showAlbums(genre);
                        fputs(str,res->pipe);              // write in pipe
                        fflush (res->pipe);
                        res->closeConnectionToPipe();
                        kill((pid_t)clientID, SIGUSR1);
                         // make signal for parent process
                        sigwait(&newmask, &signo);
                        if(signo == SIGUSR1){
                            cleanString(str);

                            res->connectToPipe(PIPE_NAME, "r");
                            memset(str, '\0', sizeof(str));
                            fread(str,1,BUFFSIZE, res->pipe);

                            fflush (res->pipe);
                            res->closeConnectionToPipe();
                            cout << "!!!!From client!!!!: " << str << endl;

                            int price = atoi(str);

                            if (price > 0){
                                cleanString(str);
                                res->connectToPipe(PIPE_NAME, "w");
                                memset(str, '\0', sizeof(str));
                                str = res->minusPrice(genre, price);

                                fputs(str,res->pipe);              // write in pipe
                                fflush (res->pipe);
                                res->closeConnectionToPipe();
                                kill((pid_t)clientID, SIGUSR1);
                                sigwait(&newmask, &signo);
                                if(signo == SIGUSR1)
                                {
                                   sem_trywait(mutex);
                                   sem_post(mutex);
                                   clientClose--;
                                }

                             }// make signal for parent process

                        }
                    }
                }

        }
        else
        {
             break;
        }
    }
     unlink(PIPE_NAME);

    return a.exec();
}

