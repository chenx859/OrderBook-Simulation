#include <iostream>
#include "Order.h"
#include <string.h>
#include "Message.h"
#include <vector>
#include <map>
#include <utility>
#include <deque>
#include "OrderBook.h"


using namespace std;

int main()
{
    cout<<"Welcome to the Trading OrderBook Simulation"<<endl;
    OrderBook* x1= new OrderBook();
    int choice;
    Message* msg = new Message();
    while(choice!=9) //Run a loop till the user asks to exit using choice = 9
    {
        cout<<"Enter 1 to enter an order"<<endl;
        cout<<"Enter 2 to Display Bid Orders"<<endl;
        cout<<"Enter 3 to Display Ask Orders"<<endl;
        cout<<"Enter 9 to Exit"<<endl;
        cin>>choice; //inputting choice from user
        switch(choice)
        {
            case 1://Enter an order information
            {
                string security;
                string side;
                string type;
                double price;
                int quantity;
                string counterparty;
                cout<<"Enter Security Name: ";
                cin>>security;
                cout<<"Enter Side(B or S): ";
                cin>>side;
                cout<<"Enter Type of Order(market or limit): ";
                cin>>type;
                cout<<"Enter Price($): ";
                cin>>price;
                cout<<"Enter Quantity: ";
                cin>>quantity;
                cout<<"Enter CounterParty Name: ";
                cin>>counterparty;
                cout<<endl;
                Order* x = new Order(security, type, side, price, quantity, counterparty);//Creating new order
                msg = new Message(x);
                msg->orderReceived();//Order received message
                x1->OrderEntry(x);//Entering order and processing
                break;
            }
            case 2:
            {
                x1->displayBids();
                break;
            }
            case 3:
            {
                x1->displayAsks();
                break;
            }
            case 9:
            {
                cout<<"Thank you for using this trade simulation."<<endl;//Exit
                break;
            }
            default:
                cout<<"That is not a valid entry, please try again."<<endl;
        }
    }
}
