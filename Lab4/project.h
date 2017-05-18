#ifndef PROJECT_H
#define PROJECT_H


#include<iostream>
using namespace std;

#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>


#include <string>

/*main class*/
class Project
{
public:
    Project(int writePause = 1000, int emersionPause = 5000); //default constructor

    static void* ThreadRoutine(void *arg);                    //main fun


    void START();                                             //program init

private:
    struct ThreadData                                         //struct with thread info
    {
        int ThreadId;
        int SleepTime;
    };

    int writePause;                                           //output pauses
    int emersionPause;                                        //pause before new thread
};

#endif // PROJECT_H
