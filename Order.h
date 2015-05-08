#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#include <iostream>
#include <string.h>

using namespace std;

class Order
{
public:
    Order();//Default constructor
    Order(string, string, string, double, int quantitytemp, string);//constructor for encapsulation 
    string getSecurity();
    void setSecurity(string s);
    string getSide();
    void setSide(string s);
    string getType();
    void setType(string t);
    int getQuantity();
    void setQuantity(int q);
    double getLimit();
    void setLimit(double s);
    string getCounterParty();
    void setCounterParty(string s);

private:
    string security;
    string side;
    string ordertype;
    int qty;
    double limit;
    string counterparty;
};


#endif
