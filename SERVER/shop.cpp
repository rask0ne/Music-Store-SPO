
#include "shop.h"
#define PIPE_NAME "./fifofile"
#define BUFFSIZE 256


using namespace std;

Shop::Shop(int rock, int blues, int jazz, int triphop) {

    this->rock = rock;
    this->blues = blues;
    this->jazz = jazz;
    this->triphop = triphop;

}

char* Shop::showGenres() {

    string szBuf = "Genres Avaliable: \n";


    if (this->rock > 0)
        szBuf += "rock\n";
    if (this->blues > 0)
        szBuf += "blues\n";
    if (this->jazz > 0)
        szBuf += "jazz\n";
    if (this->triphop > 0)
        szBuf += "trip-hop\n";

    char* buff = new char[256];

    //buff = &szBuf[0u];
    //memcpy(buff, szBuf.c_str(), szBuf.size() + 1);
    //buff = strdup(szBuf.c_str());
    strcpy(buff,szBuf.c_str());
   // buff = szBuf.c_str();

    return buff;

}

char* Shop::showAlbums(string genre) {

    if (!strcmp(genre.c_str(), "rock")) {

        string str = "\nAmount of albums this genre: ";
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", this->rock);
        str += buffer;
        str += "\n";
        str += "\nPrice of one album: ";
        snprintf(buffer, sizeof(buffer), "%d", this->rockPrice);
        str += buffer;
        str += "\nHow many albums you want to buy: ";

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }

    if (!strcmp(genre.c_str(), "blues")) {

        string str = "\nAmount of albums this genre: ";
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", this->blues);
        str += buffer;
        str += "\n";
        str += "\nPrice of one album: ";
        snprintf(buffer, sizeof(buffer), "%d", this->bluesPrice);
        str += buffer;
        str += "\nHow many albums you want to buy: ";

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }

    if (!strcmp(genre.c_str(), "jazz")) {

        string str = "\nAmount of albums this genre: ";
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", this->jazz);
        str += buffer;
        str += "\n";
        str += "\nPrice of one album: ";
        snprintf(buffer, sizeof(buffer), "%d", this->jazzPrice);
        str += buffer;
        str += "\nHow many albums you want to buy: ";

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }

    if (!strcmp(genre.c_str(), "trip-hop")) {

        string str = "\nAmount of albums this genre: ";
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", this->triphop);
        str += buffer;
        str += "\n";
        str += "\nPrice of one album: ";
        snprintf(buffer, sizeof(buffer), "%d", this->triphopPrice);
        str += buffer;
        str += "\nHow many albums you want to buy: ";

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }

}

char* Shop::minusPrice(string genre, int amount)
{

    if (!strcmp(genre.c_str(), "rock")) {

        if (this->rock > 0)
            this->rock -= amount;
        int price = this->rockPrice * amount;

        string str;
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", price);
        str += buffer;

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }
    if (!strcmp(genre.c_str(), "blues")) {

        if (this->blues > 0)
            this->blues -= amount;
        int price = this->bluesPrice * amount;

        string str;
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", price);
        str += buffer;

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }
    if (!strcmp(genre.c_str(), "jazz")) {

        if (this->jazz > 0)
            this->jazz -= amount;
        int price = this->jazzPrice * amount;

        string str;
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", price);
        str += buffer;

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }
    if (!strcmp(genre.c_str(), "trip-hop")) {

        if (this->triphop > 0)
            this->triphop -= amount;
        int price = this->triphopPrice * amount;

        string str;
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", price);
        str += buffer;

        char* buff = new char[str.length() + 1];
        strcpy(buff,str.c_str());

        return buff;

    }
}

bool Shop::connectToPipe(char* pipePath, char* mode)
{
    printf("start Connecting\n");
    this->pipe = fopen(pipePath, mode);
    if(this->pipe == NULL)
    {
        printf("Error! Compiler cannot connect to pipe!\n");
        return false;
    }
    printf("Compiler has been connected to pipe.\n");
    return true;
}

void Shop::closeConnectionToPipe()
{
    fclose(this->pipe);
    unlink(PIPE_NAME);
}

char* Shop::getData()
{
    char *str = new char[BUFFSIZE];
    fread(str,1,BUFFSIZE, this->pipe);
    return str;
}
