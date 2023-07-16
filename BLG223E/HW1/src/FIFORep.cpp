/* @Author
Student Name: Enis Teper
Student ID : 504221511
Date: 26/10/2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    // assign null to head and tail at initialization
    mpHead = NULL;
    mpTail = NULL;

}

FIFORep::FIFORep(ProcessRep* head)
{
    // If initialization done with head definition
    // Assign head and set tail to null
    this->mpHead = head;
    this->mpTail = NULL;
}


FIFORep::~FIFORep()
{
    if (mpHead != NULL)
    {
        ProcessRep* p = mpHead;
        while(mpHead->getNext() != NULL)
        {   
            p = mpHead;
            mpHead = mpHead->getNext();
            delete p;
        }
        mpHead = NULL;
        delete mpHead;
    }
    else
        delete mpHead;
    

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
    if(mpHead==NULL){
        mpHead=p;
        mpTail=p;
    }
    else{
        mpTail->setNext(p);
        mpTail=p;
    }
    
}

ProcessRep* FIFORep::dequeue()
{   
    /*
        The function remove a process from the tail of the queue and returns it.
    */ 
    // If head is null just assign null 
    if(getHead() == NULL)
    {
        mpHead = NULL;
        mpTail = NULL;
        return NULL;
    }
    // If not
    else
    {   
        // If FIFO has 1 job
        // dequeue the job and assign null
        if (mpHead == mpTail)
        {
            ProcessRep* p = mpHead;
            mpHead = NULL;
            mpTail = NULL;
            return p;
        }
        // If FIFO has more than 1 job
        // dequeue head and assign next node to head
        else
        {
            ProcessRep* p = mpHead;
            mpHead = mpHead->getNext();
            return p;
        }
        
    }

}

ProcessRep* FIFORep::searchID(int id)
{   
    /*
        The function search a process by id and returns it if it exist otherwise returns NULL.
    */ 
    ProcessRep *p = mpHead;
    // Searches the node with given id
    while(p != NULL)
    {
        if(p->getNext()->getProcessID() == id)
            return p;
        p=p->getNext();
    }
    delete p;
    return NULL;
}

void FIFORep::printFIFO()
{
    /*
        The function prints the proceses in the queue starting from Head to tail.
    */
    // If FIFO has any job
    if (mpHead != NULL)
    {

        ProcessRep* p = mpHead;
        // prints all jobs inside FIFO
        while(p != mpTail){
            cout<<p->getProcessType()<<p->getProcessID()<<" "<<p->startTime<<" "<<p->endTime<<" ";
            p=p->getNext();
        }
        // prints the latest one if it is not null
        if(p != NULL)
            cout<<p->getProcessType()<<p->getProcessID()<<" "<<p->startTime<<" "<<p->endTime<<" ";
    }
    

}