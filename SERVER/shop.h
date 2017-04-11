
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;
#ifndef RESTORAN_H
#define RESTORAN_H


class Shop {

private:



    int rock;
    int blues;
    int jazz;
    int triphop;

    int rockPrice = 100;
    int bluesPrice = 120;
    int jazzPrice = 110;
    int triphopPrice = 125;

public:

    FILE* pipe;

    Shop(int, int, int, int);

    char* showGenres();
    char* showAlbums(string);
    char* minusPrice(string, int);
    bool connectToPipe(char* pipePath, char* mode);
    void closeConnectionToPipe();
    char* getStringFromPipe();
    char* getData();

};

#endif // COMPILER_H
