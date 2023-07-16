#include <iostream>
#include <stdio.h>
#include "BinSTree.h"
using namespace std;

Node* build(int* arr,int lower,int upper)
{
    int size = upper-lower+1;
    if(upper<lower)
        return NULL;
    int mid = lower+size/2;
    Node* n = new Node(arr[mid]);
    n->set_left(build(arr,lower,mid-1));
    n->set_right(build(arr,mid+1,upper));
    return n;
}

void make_balanced(int* arr, int size, BinSTree* &t)
{
    t->root = build(arr,0,size-1);
}





int main(int argc, char *argv[]){
    /*
    list<int> v;
    v.assign({10,5,15,3,8,11,20,1,4,6,9,13});
    BinSTree* t = new BinSTree(v);
    */

    BinSTree* t = new BinSTree();

    t->add(10);
    t->add(5);
    t->add(15);
    t->add(3);
    t->add(7);
    t->add(11);
    t->add(20);
    t->add(1);
    t->add(4);
    t->add(6);
    t->add(9);

    t->leaf_count();
    t->depths();
    t->heights();

    int arr[8] = {10,11,17,19,30,31,37,38};
    BinSTree* t2 = new BinSTree();
    make_balanced(arr,8,t2);



    return EXIT_SUCCESS;
}
