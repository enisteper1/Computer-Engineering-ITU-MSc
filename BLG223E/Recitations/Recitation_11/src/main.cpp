//Reference:
//https://www.geeksforgeeks.org/interval-tree/

#include <stdio.h>
#include <iostream>
#include "IntervalTree.h"

using namespace std;

int main()
{
    Interval ints[] = {{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}};
    IntervalTree* it = new IntervalTree();

    for(int i=0;i<6;i++)
        it->add(ints[i]);

    cout << "Inorder traversal of constructed Interval Tree is\n";
    it->inorder_traversal();

    Interval x = {6, 7};

    cout << "\nSearching for interval [" << x.get_low() << "," << x.get_high() << "]";
    Interval *res = it->overlapSearch(x);
    if (res == NULL)
        cout << "\nNo Overlapping Interval" << endl;
    else
        cout << "\nOverlaps with [" << res->get_low() << ", " << res->get_high() << "]" << endl;
    
    //Interval z={15,20};
    Interval z={30,40};
	it->remove(z);
	cout << "Traversal after deletion of node [" 
    << z.get_low() << "," << z.get_high() << "]" << endl;
	it->inorder_traversal();
    return EXIT_SUCCESS;
}