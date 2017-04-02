#include "Customer.h"


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
