#pragma once
#include <string>
using namespace std;

class Node{
    private:
        string from;
        string to;
        int transfer;
        Node* next;
    
    public:
        Node(string, string, int);
        ~Node();
        Node* getnext();
        void setnext(Node*);
        void print();
        string toString();
        string getfrom();
        string getto();
        int getamount();

};