#include <iostream>
#include <string>
#include "Order.h"
#include "Message.h"

using namespace std;

Order::Order()
{
    security = "ABC";
    side = "B";
    ordertype ="market";
    qty = 0;
    limit = 0.0;
    counterparty = "Default";
}

Order::Order(string security, string type, string side, double price, int quantity, string counterparty)//constructor for encapsulation
{
    this->security = security;
    this->side = side;
    this->ordertype = type;
    this->qty = quantity;
    this->limit = price;
    this->counterparty = counterparty;
}

string Order::getSecurity()
{
    return security;
}

void Order::setSecurity(string security)
{
    this->security = security;
}

string Order::getSide()
{
    return side;
}

void Order::setSide(string side)
{
    this->side = side;
}

string Order::getType()
{
    return ordertype;
}

void Order::setType(string type)
{
    this->ordertype = type;
}

int Order::getQuantity()
{
    return qty;
}

void Order::setQuantity(int qty)
{
    this->qty = qty;
}

double Order::getLimit()
{
    return limit;
}

void Order::setLimit(double limit)
{
    this->limit = limit;
}

string Order::getCounterParty()
{
    return counterparty;
}

void Order::setCounterParty(string counterparty)
{
    this->counterparty = counterparty;
}
