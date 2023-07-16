//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname: Enis Teper
//---Student Number: 504221511
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>

#include "huffman.h"
#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-----------------------------------------------------//
//-----------Reads the key from text file--------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::readKey(const char* argv){
    ifstream readKeyFile;
    readKeyFile.open(argv);

    if(readKeyFile.is_open()){
        while(!readKeyFile.eof()){
            readKeyFile >> key;
        }
    }
    readKeyFile.close();
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------Sorts the key in an alpabetic order---------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::sortKey(){
    sortedKey = key;
    sort(sortedKey.begin(), sortedKey.end());
};
//-----------------------------------------------------//


//-----------------------------------------------------//
//-------Finds the frequency of the characters---------//
//-----------------------------------------------------//
void Huffman::findFrequency(){
    //DO NOT CHANGE THIS
    sortKey();
    //DO NOT CHANGE THIS
    
    //TODO
    int frequencies[256] = {0};
    for(int i = 0; sortedKey[i] != '\0'; i++)
    {
        frequencies[int(sortedKey[i])]++;
    }

    for(int i = 0; i < 256; i++)
    {
        if(frequencies[i] != 0)
        {
            Node* node = new Node();
            node->token.val = frequencies[i];
            node->token.symbol = char(i);
            queue.enque(node);
            //cout<<"The frequency of "<<char(i)<<" is "<<frequencies[i]<<endl;
        }
    }

};
//-----------------------------------------------------//


//-----------------------------------------------------//
//----------------Creates Huffman Tree-----------------//
//-----------------------------------------------------//
void Huffman::createHuffmanTree(){
    //TODO
    // Find frequency of each character
    findFrequency();
    Node *temp1, *temp2, *merged_node;
    while(true)
    {
        // get first 2 nodes
        temp1 = queue.dequeue();
        temp2 = queue.dequeue();
        // Merge nodes
        merged_node = huffmanTree.mergeNodes(temp1, temp2);
        // generate sub tree
        merged_node->left = temp1;
        merged_node->right = temp2;
        if(temp2->next == NULL)
            break;
        // enque subtree
        queue.enque(merged_node);
    }
    huffmanTree.root = merged_node;
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Prints the Huffman Tree---------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printHuffmanTree(){
    huffmanTree.printTree(huffmanTree.root, 0);
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Finds and returns the binary------------//
//----------------value to given character-------------//
//-----------------------------------------------------//
string Huffman::getTokenBinary(char tokenChar, Node* traverse, string tokenBinary){
    // Return if it is leaf node
    if(traverse->left == NULL && traverse->right == NULL)
        return tokenBinary;
    // If left side has the char continue left
    if(traverse->left->token.symbol.find(tokenChar) != string::npos)
        tokenBinary = getTokenBinary(tokenChar, traverse->left, tokenBinary + "0");
    // If right side has the char continue right
    if(traverse->right->token.symbol.find(tokenChar) != string::npos)
        tokenBinary = getTokenBinary(tokenChar, traverse->right, tokenBinary + "1");
    // Return latest token
    return tokenBinary;
    
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//--------------Encodes the given password-------------//
//-----------------------------------------------------//
void Huffman::encodePassword(string password){
    string tmp;
    encodedBinaryPassword = "";
    encodedValPassword = "";
    for(int i=0;password[i] != '\0'; i++)
    {
        tmp = getTokenBinary(password[i], huffmanTree.root, "");
        encodedBinaryPassword += tmp;
        encodedValPassword += to_string(tmp.length());
    }
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the encoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printEncodedPassword(){
    cout << "Encoded Password Binary: " << encodedBinaryPassword << endl;
    cout << "Encoded Password Value: " << encodedValPassword << endl;
};

//-----------------------------------------------------//
//--------------Decodes the given password-------------//
//-----------------------------------------------------//
void Huffman::decodePassword(string encodedBinaryPassword, string encodedValPassword){
    //To divide binary password into substrings
    int start = 0, val = 0;
    // substring
    string token;
    // Password placeholder
    decodedPassword = "";
    // For loop to decode all encoded characters
    for(int i = 0; encodedValPassword[i] != '\0'; i++)
    {
        // decimal to binary
        val = int(encodedValPassword[i]) - 48;
        // get substring
        token = encodedBinaryPassword.substr(start, val);
        // decode it
        decodeToken(token);
        // move to next token
        start += val;
    }
};

//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Decodes the given binary--------------//
//-----------------------------------------------------//
void Huffman::decodeToken(string encodedToken){
    // to traverse the tree
    Node* p = huffmanTree.root;
    // Traverse to target character
    for(int i = 0; encodedToken[i] != '\0'; i++)
    {
        if(encodedToken[i] == '1')
            p = p->right;
        else
            p = p->left;
    }
    // insert target character to decoded password
    decodedPassword += p->token.symbol;
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the decoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printDecodedPassword(){
    cout << "Decoded Password: " << decodedPassword << endl;
};