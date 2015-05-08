#include <iostream>
#include <string>
#include "Order.h"
#include "Message.h"

using namespace std;

Message::Message() //Default Constructor
{
    currentOrder = new Order();
    security = currentOrder->getSecurity();
    side = currentOrder->getSide();
    if(side == "B")
    {
        side = "buy";
    }
    else if (side == "S")
    {
        side = "sell";
    }
    ordertype = currentOrder->getType();
    qty = currentOrder->getQuantity();
    limit = currentOrder->getLimit();
    counterparty = currentOrder->getCounterParty();
}

Message::Message(Order* order1) //constructor for encapsulation
{
    currentOrder = order1;
    security = currentOrder->getSecurity();
    side = currentOrder->getSide();
    if(side == "B")
    {
        side = "buy";
    }
    else if (side == "S")
    {
        side = "sell";
    }

    ordertype = currentOrder->getType();
    qty = currentOrder->getQuantity();
    limit = currentOrder->getLimit();
    counterparty = currentOrder->getCounterParty();
}

void Message::partialOrderFilled(double x)//x is the price of settlement
{
    cout<<endl;
    cout<<"Partially Filled Limit Order:"<<endl;
    cout<<"Security: "<<security<<endl;
    cout<<"Side: "<<side<<endl;
    cout<<"Quantity Remaining: "<<qty<<endl;
    cout<<"Limit Price: "<<limit<<endl;
    cout<<"Price of Execution: "<<x<<endl;
    cout<<"CounterParty: "<<counterparty<<endl;
    cout<<"The remaining "<<qty<<" units have been placed in the OrderBook."<<endl;
    cout<<endl;
    return;
}

void Message::marketOrderPartiallyFilled(double x)//x is the price of settlement
{
    cout<<endl;
    cout<<"Market Order Partial Fill: "<<endl;
    cout<<"Security: "<<security<<endl;
    cout<<"Side: "<<side<<endl;
    cout<<"Quantity Remaining: "<<qty<<endl;
    cout<<"Price of Execution: "<<x<<endl;
    cout<<"CounterParty: "<<counterparty<<endl;
    cout<<"The remaining "<<qty<<" units have been placed in the OrderBook."<<endl;
    cout<<endl;
    return;
}

void Message::orderUnfilled()
{
    cout<<endl;
    cout<<"Order UnFilled: "<<endl;
    cout<<"Security: "<<security<<endl;
    cout<<"Side: "<<side<<endl;
    cout<<"Quantity: "<<qty<<endl;
    cout<<"Limit Price: "<<limit<<endl;
    cout<<"CounterParty: "<<counterparty<<endl;
    cout<<"The Order has been placed in the OrderBook."<<endl;
    cout<<endl;
    return;
}


void Message::orderFilled(double x) //x is the price of settlement
{
    if (ordertype == "market")
    {
        cout<<endl;
        cout<<"Market Order Filled: "<<endl;
        cout<<"Security: "<<security<<endl;
        cout<<"Side: "<<side<<endl;
        cout<<"Quantity: "<<qty<<endl;
        cout<<"Price of Execution: "<<x<<endl;
        cout<<"CounterParty: "<<counterparty<<endl;
        cout<<endl;
    }
    else
    {
        cout<<endl;
        cout<<"Limit Order Filled: "<<endl;
        cout<<"Security: "<<security<<endl;
        cout<<"Side: "<<side<<endl;
        cout<<"Quantity: "<<qty<<endl;
        cout<<"Limit Price: "<<limit<<endl;
        cout<<"Price of Execution: "<<x<<endl;
        cout<<"CounterParty: "<<counterparty<<endl;
        cout<<endl;
    }
    return;
}

void Message::orderReceived()
{
    cout<<endl;
    cout<<"Order Received: "<<endl;
    cout<<"Security: "<<security<<endl;
    cout<<"Side: "<<side<<endl;
    cout<<"Type: "<<ordertype<<endl;
    if(ordertype == "limit")
    {
        cout<<"Limit Price: "<<limit<<endl;
    }
    cout<<"Quantity: "<<qty<<endl;
    cout<<"CounterParty: "<<counterparty<<endl;
    cout<<endl;
}

void Message::marketOrderUnfilled()
{
    cout<<endl;
    cout<<"Market Order UnFilled: "<<endl;
    cout<<"Security: "<<security<<endl;
    cout<<"Side: "<<side<<endl;
    cout<<"Quantity: "<<qty<<endl;
    cout<<"CounterParty: "<<counterparty<<endl;
    cout<<"There is no liquidity in this market. The order cannot be filled. "<<endl;
    cout<<endl;
}
