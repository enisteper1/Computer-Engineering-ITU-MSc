#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include "md5.h"
#include "Node.h"
#include "BlockNode.h"
#include "BlockList.h"

using namespace std;

int main(int argc, char *argv[]){

    string line;
    
    fstream infile;
    map<string,int> balances;

    if(argc < 2)
        cout<<"Provide a file name"<<endl;

    BlockNode* bn;
    BlockList* bl=new BlockList();

    infile.open(argv[1],ios::in);
    if (infile.is_open()){   
        int lineno = 0;
        while(getline(infile, line)){ 
            stringstream ss(line);
            lineno++;
            if(lineno==1){
                string name;
                int bal;

                while(ss>>name>>bal)
                    balances[name]=bal;

            }
            else{
                string word; 
                ss >> word;
                if(word.compare("BLOCK")==0){
                    string from;
                    string to;
                    int trans;

                    bn = new BlockNode();
                    while(ss>>from>>to>>trans) //for multiple transactions
                        bn->addNode(new Node(from, to, trans));
                
                    bl->addBlock(bn);
                }
                else if(word.compare("CLEAR")==0){
                    delete bl;
                    bl=new BlockList();
                }
                else if(word.compare("BALANCE")==0){
                    bl->printBalance(balances);
                }
                else if(word.compare("PRINT")==0){
                    bl->print();
                }
                else{
                    cout<<"WRONG COMMAND IN LINE "<<lineno<<endl;
                }
            }
        }
        infile.close();
        delete bl;
    }
    else
        cout<<"Cannot find file"<<endl;
   
    return EXIT_SUCCESS;
}

