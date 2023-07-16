/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    
    this->mpHead = NULL;
    this->mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep* head)
{
    this->mpHead = head;
    this->mpTail = head;
}


FIFORep::~FIFORep()
{
    delete this->mpHead;
}

void FIFORep::setHead(ProcessRep* head)
{
    this->mpHead = head;
}

ProcessRep* FIFORep::getHead()
{
    return this->mpHead;
}

void FIFORep::setTail(ProcessRep* tail)
{
    this->mpTail = tail;
}

ProcessRep* FIFORep::getTail()
{
    return this->mpTail;
}

void FIFORep::queue(ProcessRep* p)
{
    /*
        The function add a process to the tail of the queue.
    */
    if (this->mpHead == NULL)
    {
        this->mpHead = p;
        this->mpTail = p;
    }
    else
    {
        this->mpTail->setNext(p);
        this->mpTail = p;
        p->setNext(NULL);
    }
}

ProcessRep* FIFORep::dequeue()
{   
    /*
        The function remove a process from the tail of the queue and returns it.
    */   
    if(this->mpHead !=NULL){
        ProcessRep* p = this->mpHead;
        this->mpHead = this->mpHead->getNext();
        if (this->mpTail == p)
        {
            this->mpTail = this->mpHead;
        }
        p->setNext(NULL);
        return p;
    }
    else
    {
        return NULL;
    }    
}

ProcessRep* FIFORep::searchID(int id)
{   
    /*
        The function search a process by id and returns it if it exist otherwise returns NULL.
    */ 
    ProcessRep* p = this->mpHead;
    while (p != NULL)
    {
        if (p->getProcessID() == id)
        {
            return p;
        }
        p = p->getNext();
    }
    return NULL;
}

void FIFORep::printFIFO()
{
    /*
        The function prints the proceses in the queue starting from Head to tail.
    */
    ProcessRep* p = this->mpHead;
    while (p != NULL)
    {
        cout << p->getProcessType()  << p->getProcessID();
        cout << " " << p->startTime << " " << p->endTime<< " ";
        p = p->getNext();
    }
}