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
#include <string>
#include "Customer.h"



#define SEMAPHORE_NAME "/my_named_semaphore"                         // name of the semaphore
#define BUFFSIZE 256                                                 // bufsize
#define PIPE_NAME "/home/rask0ne/CLIENT/fifofile.txt"
#define QUEUE_PIPE "/home/rask0ne/Logging/queuefile"
#define QUEUE_SEMAPHORE "/queue_semaphore"
#define QUEUE_FILE_SEMAPHORE "/queueFile_semaphore"// Pipe name
using namespace std;

void cleanString(char* str){

    for(int i = 0; i < strlen(str); i++)
        str[i] = '\0';

}

void clean_stdin(void)                                    // clean stdin
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

char *itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(labs(n)))+1;
    if (n<0) len++; // room for negative sign '-'

    char *buf = (char*)calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%ld", n);
    return   buf;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int signo;                                     // set the mask for getting signals
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);                  // add signals in mask
    sigaddset(&newmask, SIGUSR2);
    sigaddset(&newmask, SIGINT);
    sigprocmask(SIG_BLOCK, &newmask, NULL);

    int parentID = 0;
    parentID = atoi(argv[1]);                                        // get ppid from a command line

    sem_t *mutex, *queueMutex, *queueFileMutex;                                                     // semaphore
    mutex = sem_open(SEMAPHORE_NAME,0,0644,0);

    printf("command parent - %d\n",parentID);
    if(mutex == SEM_FAILED)
    {
        perror("reader:unable to execute semaphore");
        sem_close(mutex);
        exit(-1);
    }
    cout << endl << getpid() << endl;
    printf("Wait...\n");


    queueMutex = sem_open(QUEUE_SEMAPHORE,0,0644,0);                 // creating semaphore (mutex)
    if(queueMutex == SEM_FAILED)
    {
        perror("unable to open semaphore");
        sem_unlink(QUEUE_SEMAPHORE);                                  // destroying semaphore
        return -1;
    }

    queueFileMutex = sem_open(QUEUE_FILE_SEMAPHORE,0,0644,0);                 // creating semaphore (mutex)
    if(queueFileMutex == SEM_FAILED)
    {
        perror("unable to open semaphore");
        sem_unlink(QUEUE_FILE_SEMAPHORE);                                  // destroying semaphore
        return -1;
    }

    cout << "Parent ID " << parentID << endl;
    Customer* user = new Customer(5000);
    sem_wait(queueMutex);
    if (!user->connectToPipe(QUEUE_PIPE,"a"))
    {
        printf("cannot connect\n");
        sem_post(queueMutex);
        return -1;
    }
    user->logIn();
    user->closeConnectionToPipe();
    sem_post(queueMutex);
    sem_wait(mutex);

    sleep(1);
    user->connectToPipe(PIPE_NAME, "w");

    char choice = '1';                             // choice of continue
    char* str = new char[BUFFSIZE];
    while(choice == '1'){
        memset(str, '\0', sizeof(str));                                  // close 0 end
        printf("\nPrint 'buy' to open genres:\n");
        gets(str);
        if(!strcmp(str, "buy"))
            str = itoa(getpid());
        cout << endl << itoa(getpid()) << endl;
        fputs(str,user->pipe);// get inquery
        fflush (user->pipe);
        user->closeConnectionToPipe();
        /*fputs("\t",user->pipe);
        fputs(str, user->pipe);*/
        printf("is Send successfully\n");
        kill((pid_t)parentID,SIGUSR1);
        cout << "WAITING";// make signal for parent process
        sigwait(&newmask, &signo);
        if(signo == SIGUSR1){

            user->connectToPipe(PIPE_NAME, "r");
            char* copy = new char[BUFFSIZE];
            cleanString(str);
            memset(str, '\0', sizeof(str));
            fread(str,1,BUFFSIZE,user->pipe);// wait for
            fflush (user->pipe);

            cout << "!!!!From shop!!!!: " << str << endl;
            user->closeConnectionToPipe();
            user->connectToPipe(PIPE_NAME, "w");
            printf("\nPrint genre yOOOu want to buy...\n");
            cleanString(str);
            memset(str, '\0', sizeof(str));
            gets(str);
            string genre(str);// get inquery
            fputs(str, user->pipe);
            fflush (user->pipe);
            printf("is Send successfully\n");
            user->closeConnectionToPipe();
            kill((pid_t)parentID,SIGUSR1);                                           // write in pipe

            sigwait(&newmask, &signo);
            if(signo == SIGUSR1){
                cleanString(str);
                memset(str, '\0', sizeof(str));
                user->connectToPipe(PIPE_NAME, "r");
                fread(str,1,BUFFSIZE,user->pipe);// wait for
                fflush (user->pipe);

                cout << "!!!!From shop!!!!: " << str << endl;
                user->closeConnectionToPipe();
                cout << "Print amount of albums you want to buy..." << endl;
                memset(str, '\0', sizeof(str));
                gets(str);
                int alb = atoi(str);// get

                user->connectToPipe(PIPE_NAME, "w");
                fputs(str, user->pipe);
                fflush (user->pipe);
                printf("is Send successfully\n");
                user->closeConnectionToPipe();
                kill((pid_t)parentID,SIGUSR1);                  // make signal for parent process
                sigwait(&newmask, &signo);
                if(signo == SIGUSR1){
                    memset(str, '\0', sizeof(str));
                    cleanString(str);
                    user->connectToPipe(PIPE_NAME, "r");
                    fread(str,1,BUFFSIZE,user->pipe);// wait for
                    fflush (user->pipe);
                    user->closeConnectionToPipe();
                    cout << "!!!!From shop!!!!: " << str << endl;

                    user->takeMyMoney(atoi(str));
                    user->getCash();
                    if(!strcmp(genre.c_str(), "rock"))
                    {
                        user->setRock(alb);
                    }
                    if(!strcmp(genre.c_str(), "blues"))
                    {
                        user->setBlues(alb);
                    }
                    if(!strcmp(genre.c_str(), "jazz"))
                    {
                        user->setJazz(alb);
                    }
                    if(!strcmp(genre.c_str(), "trip-hop"))
                    {
                        user->setTrip(alb);
                    }
                    user->printAlbums();

                }
            }
        }

        cout << "\ninput 1 for continue\n" << endl;        // continue process
        scanf("%c", &choice);// scanf choice

        sem_wait(queueFileMutex);
        user->logOut(QUEUE_PIPE);
        sem_post(queueFileMutex);
        sem_close(queueMutex);
        sem_close(queueFileMutex);
        if(choice == '1') kill((pid_t)parentID,SIGUSR2);
        else kill((pid_t)parentID,SIGUSR1);
    }
    QCoreApplication::quit();
    cout << "HUEHUEHUE" << endl;
    user->closeConnectionToPipe();                             // close pipe
    printf("kill\n");
    sem_post(mutex);
    sem_close(mutex);                                                // close semaphore
    // send to parent process SIGUSR1
    // close semaphore
    sleep(4);
    return 0;


}

