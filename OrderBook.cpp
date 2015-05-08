#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <utility>
#include "Order.h"
#include "OrderBook.h"
#include "Message.h"
#include <sstream>
#include <fstream>
#include <cstdlib>

OrderBook::OrderBook()
{
    //deque of strings to import different components of an order
    deque <string> price;
    deque <string> quantity;
    deque <string> counterparty;
    deque <string> order;
    deque <string> ticker;
    deque <string> orderType;
    deque <Order*> Orders;
    char* filename = new char[100];
    cout<<"Please import an Order Book(enter nothing if no import): ";
    cin.getline(filename, 100);//Get file name
    cout<<endl;
    ifstream inFile;
    inFile.open(filename);
    if (inFile.fail())
    {
        cout<<"Failed to find file. Sorry."<<endl;
    }
    string line;
    string item;
    int linenum = 0;
    getline(inFile, line);
    while (getline (inFile, line))//While the line still exists
    {
        linenum++;
        istringstream linestream(line);
        int itemnum = 0;
        while (getline (linestream, item, ','))//import data from the file
        {
            itemnum++;
            if (itemnum == 1)
            {
                ticker.push_back(item.c_str());
            }
            if (itemnum == 2)
            {
                order.push_back(item.c_str());
            }
            if (itemnum == 3)
            {
                orderType.push_back(item.c_str());
            }
            if (itemnum == 4)
            {
                quantity.push_back(item.c_str());
            }
            if (itemnum == 5)
            {
                price.push_back(item.c_str());
            }
            if (itemnum == 6)
            {
                counterparty.push_back(item.c_str());
            }
        }
    }
    
    double pricetemp;
    int qtytemp;
    Message* msg;
    Order* x1;
    
    for(int i =0; i<price.size(); i++)
    {
        x1=new Order();
        pricetemp= strtod(price[i].c_str(),NULL);
        qtytemp= atoi(quantity[i].c_str());
        x1->setCounterParty(counterparty[i]);
        x1->setSecurity(ticker[i]);
        x1->setLimit(pricetemp);
        x1->setQuantity(qtytemp);
        x1->setSide(order[i]);
        x1->setType(orderType[i]);
        msg = new Message(x1);
        msg->orderReceived();//order received message
        Orders.push_back(x1);//Storing inside a deque of orders
    }
    
    Order* temporder = new Order();//creating new order object
    
    for(deque<Order*>::iterator it = Orders.begin(); it!=Orders.end(); it++)
    {
        temporder = *it;
        OrderEntry(temporder);//prossing each order in the import file
    }
}

void OrderBook::OrderEntry(Order* x) //To get an order entry and create a new object of class order
{
    if (x->getSide() == "B")//Bid Entry
    {
        bidOrder(x);
    }
    if (x->getSide() == "S")//Ask Entry
    {
        askOrder(x);
    }
    return;
}

void OrderBook::bidOrder(Order* x)
{
    map<double, deque<Order*> >::iterator it;
    it = AskBook.begin();
    if(AskBook.empty()&&x->getType()=="limit")//If the Askbook is empty and its a limit order
    {
        unfilledBid(x);
        return;
    }
    if(x->getType()=="limit") //if Order is limit order
    {
        if (x->getLimit()<it->first && !it->second.empty())
        {
            unfilledBid(x);
        }
        else if (x->getLimit()>=it->first)//If bid price is higher than lowest ask then fill it
        {
            fillBid(x);
            return;
        }
    }
    else if(x->getType() == "market")//if the order is market order
    {
        fillBidMarket(x);
        return;
    }
    return;
}

void OrderBook::askOrder(Order* x)
{
    map<double, deque<Order*> >::iterator it;
    it = BidBook.begin();
    if(BidBook.empty()&&x->getType()=="limit")
    {
        unfilledAsk(x);
        return;
    }
    if(x->getType()=="limit") //if Order is limit order
    {
        if (x->getLimit()>it->first && !it->second.empty())
        {
            unfilledAsk(x);
            return;
        }
        else
        {
            fillAsk(x);
            return;
        }
    }
    else if(x->getType() == "market")//if the order is market order
    {
        fillAskMarket(x);
        return;
    }
    return;
}

void OrderBook::fillBid(Order* x)//Fill Bid Limit orders
{
    map<double, deque<Order*> >::iterator it;//iterator through ask book
    it = AskBook.begin();
    int qtemp;
    int qx;
    Order* temp;//Temp order variable to store modified orders
    deque<Order*> x1 = it->second;
    temp = x1.front();
    
    if(x1.empty() || x->getLimit() < it->first)
    {
        unfilledBid(x);
        return;
    }
    if(x->getQuantity()<temp->getQuantity())//if x has a smaller quantity than first order
    {
        qx = x->getQuantity();//store x's quantity
        qtemp = temp->getQuantity();//store temps quantity
        temp->setQuantity(qtemp - qx);
        Message* msg = new Message(x);
        msg->orderFilled(temp->getLimit());
        msg = new Message(temp);
        msg->partialOrderFilled(temp->getLimit());
        x1.pop_front();//modify the deque
        x1.push_front(temp);
        it->second = x1;//place the new deque at that price in the map
        return;
    }
    else if(x->getQuantity()==temp->getQuantity())//if x quantity is equal first order quantity
    {
        Message* msg = new Message(x);
        msg->orderFilled(temp->getLimit());
        msg = new Message(temp);
        msg->orderFilled(temp->getLimit());
        x1.pop_front();
        if(!x1.empty())//if after poping the deque is empty then delete the element from the map
        {
            it->second = x1;
        }
        else if(x1.empty() && !AskBook.empty())
        {
            AskBook.erase(it);
        }
        return;
    }
    else if (x->getQuantity()>temp->getQuantity() && x->getLimit() >= it->first)//If temp has a smaller quantity than x
    {
        qx = x->getQuantity();
        qtemp = temp->getQuantity();
        x->setQuantity(qx - qtemp);//change x
        Message* msg = new Message(temp);
        msg->orderFilled(temp->getLimit());//fill temp
        msg = new Message(x);
        msg->partialOrderFilled(temp->getLimit());//partially fill x, then modify it
        x1.pop_front();//pop temp and delete the element if the deque is empty
        if(!x1.empty())
        {
            it->second = x1;
        }
        else if(x1.empty() && !AskBook.empty())
        {
            AskBook.erase(it);
        }
        fillBid(x);//recall this function with the modified x
    }
}

void OrderBook::fillBidMarket(Order* x)
{
    map<double, deque<Order*> >::iterator it;//Iterator through ask book
    Message* msg = new Message();
    it = AskBook.begin();
    if(AskBook.empty())
    {
        msg = new Message(x);//if ask book is empty
        msg->marketOrderUnfilled();
        return;
    }
    Order* temp;
    deque<Order*> x1 = it->second;
    int q;//temp variable to store new quantity of order if its partially filled
    while(x->getQuantity()!=0 && !AskBook.empty() && it!=AskBook.end())
    {
        x1 = it->second;
        temp = x1.front();
        if (x->getQuantity()>temp->getQuantity())
        {
            msg = new Message(temp);
            msg->orderFilled(temp->getLimit());
            q =x->getQuantity()-temp->getQuantity();
            x->setQuantity(q);
            msg = new Message(x);
            msg->marketOrderPartiallyFilled(temp->getLimit());
            x1.pop_front();
            if(!x1.empty())
            {
                it->second = x1;
            }
            else if(x1.empty() && !AskBook.empty())
            {
                AskBook.erase(it);
            }
            fillBidMarket(x);
        }
        else if (x->getQuantity()==temp->getQuantity())
        {
            msg = new Message(x);
            msg->orderFilled(temp->getLimit());
            msg = new Message(temp);
            msg->orderFilled(temp->getLimit());
            x->setQuantity(0);
            x1.pop_front();
            it->second = x1;
            return;
        }
        else if (x->getQuantity()<temp->getQuantity())
        {
            msg = new Message(x);
            msg->orderFilled(temp->getLimit());
            q = temp->getQuantity()-x->getQuantity();
            temp->setQuantity(q);
            msg = new Message(temp);
            msg->partialOrderFilled(temp->getLimit());
            x->setQuantity(0);
            x1.pop_front();
            x1.push_front(temp);
            it->second = x1;
        }
    }
    return;
}

void OrderBook::unfilledBid(Order* x)
{
    Message* msg = new Message(x);
    msg->orderUnfilled();//receive unfilled message
    map<double, deque<Order*> >::iterator it;
    deque<Order*> temp;
    double findx = x->getLimit();
    it = BidBook.find(findx); //Using iterator to see if there is already a queue at that price
    if(it!=BidBook.end())//if the BidBook is not empty then push it to the back of the current deque
    {
        it->second.push_back(x);
        return;
    }
    else
    {
        temp.push_back(x);//else push it into a new deque which is made the element at this price
        BidBook[x->getLimit()] = temp;
    }
    return;
}

void OrderBook::fillAsk(Order* x)//same principles as fillBid for Ask limit orders
{
    Message* msg = new Message();
    map<double, deque<Order*> >::iterator it,ittemp;
    it = BidBook.begin();
    int qtemp;
    int qx;
    if(BidBook.empty() || x->getLimit() > it->first)
    {
        unfilledAsk(x);
        return;
    }
    Order* temp;
    deque<Order*> x1 = it->second;// store deque at price
    temp = x1.front();
    if(x->getQuantity()<temp->getQuantity())
    {
        msg = new Message(x);
        msg->orderFilled(temp->getLimit());
        qx = x->getQuantity();
        qtemp = temp->getQuantity();
        temp->setQuantity(qtemp - qx);
        msg = new Message(temp);
        msg->partialOrderFilled(temp->getLimit());
        x1.pop_front();
        x1.push_front(temp);
        it->second = x1;
        return;
    }
    else if(x->getQuantity()==temp->getQuantity())
    {
        msg = new Message(x);
        msg->orderFilled(temp->getLimit());
        msg = new Message(temp);
        msg->orderFilled(temp->getLimit());
        x1.pop_front();
        if(!x1.empty())
        {
            it->second = x1;
        }
        else if(x1.empty())
        {
            BidBook.erase(it);
        }
        return;
    }
    else if (x->getQuantity()>temp->getQuantity() && x->getLimit() <= it->first)
    {
        qx = x->getQuantity();
        qtemp = temp->getQuantity();
        x->setQuantity(qx - qtemp);
        msg = new Message(temp);
        msg->orderFilled(temp->getLimit());
        msg = new Message(x);
        msg->partialOrderFilled(temp->getLimit());
        x1.pop_front();
        if(!x1.empty())
        {
            it->second = x1;
        }
        else if(x1.empty())
        {
            BidBook.erase(it);
        }
        fillAsk(x);//recursively try to fill partial orders
    }
    return;
}

void OrderBook::fillAskMarket(Order* x)//same principles as fill bid market
{
    Message* msg = new Message();
    if(BidBook.empty())
    {
        msg = new Message(x);
        msg->marketOrderUnfilled();
        return;
    }
    map<double, deque<Order*> >::iterator it;
    it = BidBook.begin();
    if(it->second.empty())
    {
        msg = new Message(x);
        msg->marketOrderUnfilled();
        return;
    }
    Order* temp;
    deque<Order*> x1;
    int q;//temp variable to store new quantity of order if its partially filled
    while(x->getQuantity()!=0 && !BidBook.empty() && it!=BidBook.end())
    {
        x1 = it->second;
        temp = x1.front();
        if (x->getQuantity()>temp->getQuantity())
        {
            msg = new Message(temp);
            msg->orderFilled(temp->getLimit());
            q =x->getQuantity()-temp->getQuantity();
            x->setQuantity(q);
            msg = new Message(x);
            msg->marketOrderPartiallyFilled(temp->getLimit());
            x1.pop_front();
            if(!x1.empty())
            {
                it->second = x1;
            }
            else if(x1.empty() && !BidBook.empty())
            {
                BidBook.erase(it);
            }
            fillAskMarket(x);
        }
        else if (x->getQuantity()==temp->getQuantity())
        {
            msg = new Message(x);
            msg->orderFilled(temp->getLimit());
            msg = new Message(temp);
            msg->orderFilled(temp->getLimit());
            x->setQuantity(0);
            x1.pop_front();
            it->second = x1;
            return;
        }
        else if (x->getQuantity()<temp->getQuantity())
        {
            msg = new Message(x);
            msg->orderFilled(temp->getLimit());
            q = temp->getQuantity()-x->getQuantity();
            temp->setQuantity(q);
            msg= new Message(temp);
            msg->partialOrderFilled(temp->getLimit());
            x->setQuantity(0);
            x1.pop_front();
            x1.push_front(temp);
            it->second = x1;
            return;
        }
    }
    return;
}

void OrderBook::unfilledAsk(Order* x)//unfilled ask works the same way as unfilled bid
{
    Message* msg = new Message(x);
    msg->orderUnfilled();
    map<double, deque<Order*> >::iterator it;
    deque<Order*> temp; //to modify queue at a position
    it = AskBook.find(x->getLimit()); //Using iterator to see if there is already a queue at that price
    if(it!=AskBook.end())
    {
        it->second.push_back(x);
        return;
    }
    else
    {
        temp.push_back(x);
        AskBook[x->getLimit()] = temp;
    }
    return;
}

void OrderBook::displayBids() //Function to display Bids
{
    deque<Order*> x1;
    Order* x = new Order();
    if(BidBook.empty())
    {
        cout<<"There are no Bid Orders. The book is empty"<<endl<<endl<<endl;
        return;
    }
    for(map<double,deque<Order*> >::iterator it=BidBook.begin(); it!=BidBook.end(); it++)
    {

        x1 = it->second; //Get Queue from that particular price and iterate over it
        for (deque<Order*>::iterator it2 = x1.begin(); it2!=x1.end(); it2++)
        {
            x = *it2;
            cout<<"Name:     "<<x->getCounterParty()<<endl;
            cout<<"Security: "<<x->getSecurity()<<endl;
            cout<<"Type:     "<<x->getType()<<endl;
            cout<<"Quantity: "<<x->getQuantity()<<endl;
            cout<<"Price:    "<<x->getLimit()<<endl;
        }

    }
    return;
}

void OrderBook::displayAsks() //Function to display Asks
{
    deque<Order*> x1;
    Order* x = new Order();
    if(AskBook.empty())//if its empty there is a special message
    {
        cout<<"There are no Ask Orders."<<endl<<endl<<endl;
        return;
    }
    //Using an iterator to run over the map and print out the corresponding object
    for(map<double,deque<Order*> >::iterator it=AskBook.begin(); it!=AskBook.end(); it++)
    {
        x1 = it->second;
        for (deque<Order*>::iterator it2 = x1.begin(); it2!=x1.end(); it2++)//run through entire deque and price elements
        {
            x = *it2;//store the order itself
            cout<<"Name:     "<<x->getCounterParty()<<endl;
            cout<<"Security: "<<x->getSecurity()<<endl;
            cout<<"Type:     "<<x->getType()<<endl;
            cout<<"Quantity: "<<x->getQuantity()<<endl;
            cout<<"Price:    "<<x->getLimit()<<endl;
        }
    }
    return;
}
