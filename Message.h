#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include <iostream>
#include <string>
#include "Order.h"

using namespace std;

class Message
{
public:
    Message();//default constructor
    Message(Order* order);//constructor for encapsulation
    void orderReceived();
    void orderFilled(double x);
    void partialOrderFilled(double x);
    void orderUnfilled();
    void marketOrderUnfilled();
    void marketOrderPartiallyFilled(double x);
private:
    Order* currentOrder;
    string security;
    string side;
    string ordertype;
    int qty;
    double limit;
    string counterparty;
};

#endif
