
#include "project.h"

Project::Project(int writePause, int emersionPause)
{
    this->writePause = writePause;       //pause of printing on screen (default = 1 sec)
    this->emersionPause = emersionPause; //pause between creating new thread (default = 5 sec)
}

void* Project::ThreadRoutine(void *arg)                  //function executable by thread
{

    struct ThreadData Data = *((struct ThreadData*)arg); //getting info about current threads
    sem_t* hSemaphore = sem_open("Semaphore",0);         //creating semaphore for output on screen

    while(true)                                          //while threads end
    {
        sem_wait(hSemaphore);                            //getting semaphore for 1 thread

        if(Data.ThreadId == 1)
        {
            napms(Data.SleepTime);                       //pausing programm for time to output threads
            printw("\nThread[%d] ",Data.ThreadId);
        }
        else
            printw("Thread[%d] ",Data.ThreadId);

        sem_post(hSemaphore);                            //setting semaphore for next thread
        napms(1);
    }
}

void Project::START()
{

    printf ("\[\e]0;Threads\a\]");

    initscr();                                    // initialization
    cbreak();                                     // setting only one symbol to input in terminal
    noecho();                                     // no vision for input symbol
    scrollok(stdscr, TRUE);                       // blocking terminal scroll
    nodelay(stdscr, TRUE);                        // nonblocking getch()
    curs_set(0);                                  // hiding cursor

    sem_unlink ("Semaphore");
    sem_open ("Semaphore", O_CREAT , 0777, 1);

    int threadCounter = 1;                        //counting number of threads

    struct ThreadData *threadData;                //initializing struct of threads

    pthread_t *ThreadHandles;                     //thread descriptor (POSIX standart)

    threadData = new ThreadData;
    threadData[threadCounter - 1].ThreadId = threadCounter;  //setting thread id
    threadData[threadCounter - 1].SleepTime = writePause;    //setting its sleeptime before output

    ThreadHandles = new pthread_t;
    /*creating new thread, first one*/
    pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);

    char temp;

    long long int start = clock();

    while (true) {                  //main cycle of adding or deleting threads


            temp = getch();
            if(temp == '+')
            {
                threadCounter++;

                /*reallocing memory in struct for new thread*/
                threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
                threadData[threadCounter - 1].ThreadId = threadCounter;
                threadData[threadCounter - 1].SleepTime = writePause;

                ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));
                pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);
            }
            if(temp == '-' && threadCounter > 1)
            {

                /*deleting thread*/
                pthread_cancel(ThreadHandles[threadCounter - 1]);

                threadCounter--;

                ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));

                threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
            }
            if(temp == 'q')
            {
                /*ending program*/
                endwin();
                exit(0);
            }

        /*every 5 (or other) seconds adding new thread by default*/
        if((clock() - start) / 1000 > emersionPause)
        {
            start = clock();

            threadCounter++;

            threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
            threadData[threadCounter - 1].ThreadId = threadCounter;
            threadData[threadCounter - 1].SleepTime = writePause;

            ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));
            pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);
        }
    }
}
