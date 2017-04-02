#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;


class Customer {

private:


	int cash;

public:
	Customer(int);

	void takeMyMoney(int);
	void getCash();
};