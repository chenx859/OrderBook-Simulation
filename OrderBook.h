#ifndef ORDERBOOK_H_INCLUDED
#define ORDERBOOK_H_INCLUDED

#include <iostream>
#include <map>
#include <deque>
#include <utility>
#include "Order.h"
#include "Message.h"

class OrderBook
{

public:
    OrderBook(); //default constructor
    void OrderEntry(Order* x);//function to input an order
    void bidOrder(Order* x);//function to process a bid Order
    void askOrder(Order* x);//function to process an ask Order
    void fillBid(Order* x);//function to completely fill a Bid Order
    void fillBidMarket(Order* x);//function to fill Market Bid Order
    void unfilledBid(Order* x);//function to add an unfilled Bid Order to the orderbook
    void fillAsk(Order* x);//function to completely fill an ask order
    void fillAskMarket(Order* x);//function to fill Market Ask Order
    void unfilledAsk(Order* x);//function to add an unfilled ask order to the orderbook
    void displayBids(); //function to display bid orders
    void displayAsks(); //function to display ask orders

private:
    map<double, deque<Order*>, greater<double> > BidBook; //map to store bidbook
    map<double, deque<Order*> > AskBook; //map to store askbook
};


#endif
