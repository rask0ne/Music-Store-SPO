
#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>

using namespace std;

class Customer {

private:

    int rock = 0;
    int blues = 0;
    int jazz = 0;
    int triphop = 0;
    int cash;



public:

    FILE* pipe;
    Customer(int);

    void takeMyMoney(int);
    void getCash();

    void setRock(int);
    int getRock();
    void setBlues(int);
    int getBlues();
    void setJazz(int);
    int getJazz();
    void setTrip(int);
    int getTrip();
    void printAlbums();
    int getCashInt();
    bool connectToPipe(char* pipePath, char* mode);
    void closeConnectionToPipe();
    void inputData(char*);
    void logIn();
    void logOut(char* str);
};

#endif // CLIENT_H
