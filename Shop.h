#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

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

	Shop(int, int, int, int);
	
	string showGenres();
	string showAlbums(string);
	string minusPrice(string, int);

};