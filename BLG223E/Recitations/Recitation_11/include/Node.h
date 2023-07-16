#include "Interval.h"

class Node
{
    private:
        Interval* i;
        int max;
        Node* left;
        Node* right;

    public:
        Node(Interval,int);
        void set_left(Node*);
        void set_right(Node*);
        void set_i(int,int);
        void set_max(int);

        Node* get_left();
        Node* get_right();
        Interval* get_i();
        int get_max();
};