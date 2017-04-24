

#include "Customer.h"
#include <unistd.h>
#define TEMP_PIPE "/home/rask0ne/Logging/queuefile_temp"
#define BUFFSIZE 256
Customer::Customer(int cash) {

    this->cash = cash;

}

void Customer::takeMyMoney(int cash) {

    if (this->cash - cash < 0) {

        cout << "You can not buy what you want, because you don't have money for that" << endl;

    }
    else {

        this->cash -= cash;
        cout << cash << endl;
        cout << "Transaction complete!" << endl;

    }

}

void Customer::getCash()
{
    cout << "Current cash: " << this->cash << endl;
}

void Customer::setRock(int rock){

    this->rock = rock;

}

int Customer::getRock(){

    return this->rock;

}

void Customer::setBlues(int blues){

    this->blues = blues;

}

int Customer::getBlues(){

    return this->blues;

}

void Customer::setJazz(int jazz){

    this->jazz = jazz;

}

int Customer::getJazz(){

    return this->jazz;

}

void Customer::setTrip(int trip){

    this->triphop = trip;

}

int Customer::getTrip(){

    return this->triphop;

}

void Customer::printAlbums(){

    cout << "Amount of rock albums: " << this->getRock() << endl;
    cout << "Amount of blues albums: " << this->getBlues() << endl;
    cout << "Amount of jazz albums: " << this->getJazz() << endl;
    cout << "Amount of trip-hop albums: " << this->getTrip() << endl;
}

int Customer::getCashInt(){

    return this->cash;

}

bool Customer::connectToPipe(char* pipeName, char* mode)
{
    printf("try connect to pipe\n");
    sleep(1);
    this->pipe = fopen(pipeName, mode);
    printf("connecting to pipe\n");
    sleep(1);
    if(this->pipe == NULL)
    {
        printf("Error! Client cannot connect to pipe!\n");
        return false;
    }
    printf("Client is connected to pipe.\n");
    sleep(1);
    return true;
}

void Customer::closeConnectionToPipe()
{
    fclose(this->pipe);
    printf("close\n");
}

void Customer::inputData(char* str)
{
    fputs(str, this->pipe);
}

void Customer::logIn()
{
    printf("logIn - %d\n",(int)getpid());
    fputs(std::to_string((int)getpid()).c_str(),this->pipe);
    fputs("\n",this->pipe);
    printf("logIn succesfully\n");
}

void Customer::logOut(char* pipeName)
{
    if (!this->connectToPipe(pipeName,"r"))
    {
        printf("can't connection to temp Pipe \n");
        sleep(1);
        return;
    }
    printf("logOut\n");
    FILE *tempFile = fopen(TEMP_PIPE, "w+");
    if (tempFile == NULL)
    {
        printf("can't create file\n");
        this->closeConnectionToPipe();
        sleep(1);
        return;
    }
    int idBuffer;
    char tempBuffer[BUFFSIZE];
    rewind(this->pipe);
    fseek(this->pipe,0,0);
    do
    {
        fgets(tempBuffer,BUFFSIZE,this->pipe);
        if (feof(this->pipe))
        {
            break;
        }
        //fseek(this->pipe,1,1);
        if (tempBuffer == NULL)
        {
            printf("pidec\n");
        }
        idBuffer = atoi(tempBuffer);
        printf("read from pipe - %ld\n",ftell(this->pipe));
        puts(tempBuffer);
        if (idBuffer != getpid())
        {
            fputs(std::to_string(idBuffer).c_str(),tempFile);
            fputs("\n",tempFile);
        }
    }while(1);

    printf("pos in file - %ld",ftell(this->pipe));
    rewind(this->pipe);
    fclose(tempFile);
    fclose(this->pipe);
    remove(pipeName);
    rename(TEMP_PIPE,pipeName);
    printf("remove and rename\n");
    sleep(1);
}
