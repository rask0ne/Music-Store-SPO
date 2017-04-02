#include "Shop.h"
#define _CRT_SECURE_NO_WARNINGS

Shop::Shop(int rock, int blues, int jazz, int triphop) {

	this->rock = rock;
	this->blues = blues;
	this->jazz = jazz;
	this->triphop = triphop;

}

string Shop::showGenres() {

	string szBuf = "Genres Avaliable: \n";
	
	if (this->rock > 0)
		szBuf += "rock\n";
	if (this->blues > 0)
		szBuf += "blues\n";
	if (this->jazz > 0)
		szBuf += "jazz\n";
	if (this->triphop > 0)
		szBuf += "trip-hop\n";

	
	return szBuf;

}

string Shop::showAlbums(string genre) {

	if (!strcmp(genre.c_str(), "rock")) {

		string str = "\nAmount of albums this genre: ";
		char temp[10];
		_itoa(this->rock, temp, 10);
		str += temp;
		str += "\n";
		str += "\nPrice of 1 album: ";
		_itoa(this->rockPrice, temp, 10);
		str += temp;
		str += "\nHow many albums you want to buy: ";

		return str;

	}

	if (!strcmp(genre.c_str(), "blues")) {

		string str = "\nAmount of albums this genre: ";
		char temp[10];
		_itoa(this->blues, temp, 10);
		str += temp;
		str += "\n";
		str += "\nPrice of 1 album: ";
		_itoa(this->bluesPrice, temp, 10);
		str += temp;
		str += "\nHow many albums you want to buy: ";

		return str;

	}

	if (!strcmp(genre.c_str(), "jazz")) {

		string str = "\nAmount of albums this genre: ";
		char temp[10];
		_itoa(this->jazz, temp, 10);
		str += temp;
		str += "\n";
		str += "\nPrice of 1 album: ";
		_itoa(this->jazzPrice, temp, 10);
		str += temp;
		str += "\nHow many albums you want to buy: ";

		return str;

	}

	if (!strcmp(genre.c_str(), "trip-hop")) {

		string str = "\nAmount of albums this genre: ";
		char temp[10];
		_itoa(this->triphop, temp, 10);
		str += temp;
		str += "\n";
		str += "\nPrice of 1 album: ";
		_itoa(this->triphopPrice, temp, 10);
		str += temp;
		str += "\nHow many albums you want to buy: ";

		return str;

	}
	
}

string Shop::minusPrice(string genre, int amount)
{
	if (!strcmp(genre.c_str(), "rock")) {

		if (this->rock > 0)
			this->rock -= amount;
		int price = this->rockPrice * amount;

		string str;
		char temp[10];
		_itoa(price, temp, 10);
		str += temp;

		return str;

	}
	if (!strcmp(genre.c_str(), "blues")) {

		if (this->blues > 0)
			this->blues -= amount;
		int price = this->bluesPrice * amount;

		string str;
		char temp[10];
		_itoa(price, temp, 10);
		str += temp;

		return str;

	}
	if (!strcmp(genre.c_str(), "jazz")) {

		if (this->jazz > 0)
			this->jazz -= amount;
		int price = this->jazzPrice * amount;

		string str;
		char temp[10];
		_itoa(price, temp, 10);
		str += temp;

		return str;

	}
	if (!strcmp(genre.c_str(), "trip-hop")) {

		if (this->triphop > 0)
			this->triphop -= amount;
		int price = this->triphopPrice * amount;

		string str;
		char temp[10];
		_itoa(price, temp, 10);
		str += temp;

		return str;

	}
}
