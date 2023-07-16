#include <stdio.h>
#include <iostream>
#include <limits.h>
#include "IntervalTree.h"

using namespace std;

IntervalTree::IntervalTree()
{
    this->root=NULL;    
}

Node* IntervalTree::createNode(Interval i)
{
    Node* n = new Node(i,i.get_high());
    return n;
}

Node* IntervalTree::add(Node* r ,Interval i)
{
    if(r == NULL)
        return createNode(i);
    
    int l = r->get_i()->get_low();

    if(i.get_low() < l) //if added node low value is smaller than r's low value go left subtree
        r->set_left(add(r->get_left(),i));
    
    else //else added node low value is greater than r's low value go right subtree
        r->set_right(add(r->get_right(),i));

    //Update the max value of this ancestor if needed
    if(r->get_max() < i.get_high())
        r->set_max(i.get_high());

    return r;
}

void IntervalTree::add(Interval i)
{
    if(this->root == NULL)
    {
        root = createNode(i);
        return;
    }
    Node* created_node = add(root,i);
}


bool IntervalTree::doOverlap(Interval i1, Interval i2)
{
    if (i1.get_low() <= i2.get_high() && i2.get_low() <= i1.get_high())
        return true;
    return false;
}


Interval* IntervalTree::overlapSearch(Node* r, Interval i)
{
    if(r == NULL)
        return NULL;

    if(doOverlap(*(r->get_i()), i)) //if given interval overlap with r
        return r->get_i();
    
    //if left subtree max is >= given interval low then go left subtree
    if(r->get_left()!=NULL && r->get_left()->get_max() >= i.get_low())
        return overlapSearch(r->get_left(),i);

    //else go right subtree
    return overlapSearch(r->get_right(),i);


}
Interval* IntervalTree::overlapSearch(Interval i)
{
    return overlapSearch(this->root, i);
}

void IntervalTree::inorder_traversal(Node* r)
{
    if(r == NULL)
        return;
    
    inorder_traversal(r->get_left());
    cout << "[" << r->get_i()->get_low() << ", " << r->get_i()->get_high() << "]"
    << " max = " << r->get_max() << endl;
    inorder_traversal(r->get_right());
}

void IntervalTree::inorder_traversal()
{
    inorder_traversal(this->root);
}

Node* IntervalTree::minlowinterval(Node* r)
{
	while(r->get_left())
	    r = r->get_left();
	
	return r;
}

int IntervalTree::findmax(int a,int b,int c)
{
	return a>b?(a>c?a:c):(b>c?b:c);
}

Node* IntervalTree::remove(Node* r,Interval i)
{
    if(r == NULL)
        return NULL;
    
    Node* temp;

    if(i.get_low() < r->get_i()->get_low())
        r->set_left(remove(r->get_left(),i));

    else if(i.get_low() > r->get_i()->get_low())
        r->set_right(remove(r->get_right(),i));

    else if(i.get_low() == r->get_i()->get_low())
    {
        if(i.get_high() == r->get_i()->get_high())
        {
            if(r->get_left() == NULL)
            {
                temp = r->get_right();
                free(r);
                return temp;
            }
            if(r->get_right() == NULL)
            {
                temp = r->get_left();
                free(r);
                return temp;
            }
            temp = minlowinterval(r->get_right());
            r->set_i(temp->get_i()->get_low(), temp->get_i()->get_high());
            r->set_right(remove(r->get_right(),*(temp->get_i())));
        }
        else
            r->set_right(remove(r->get_right(),i));
    }
    r->set_max(findmax(r->get_i()->get_high(),((r->get_left())?r->get_left()->get_max():INT_MIN),
    ((r->get_right())?r->get_right()->get_max():INT_MIN)));
    return r;
}

void IntervalTree::remove(Interval i)
{
    Node* n_root = remove(this->root,i);
}
