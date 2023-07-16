/* @Author
Student Name: Enis Teper
Student ID : 504221511
Date: 26/10/2022
*/
#include <iostream> 
#include <stdio.h>
#include <string>
#include "ProcessRep.h"


using namespace std;

ProcessRep::ProcessRep(string processType, int processID, int deliverTime, int processTime)
{   
    // Initial assignments
    this->mProcessType = processType;
    this->mProcessID = processID;
    this->mDeliverTime = deliverTime;
    this->mProcessTime = processTime;
    
    this->mpNext = NULL;
    this->remainingTime = processTime;
    this->endTime = -1;
    this->startTime = -1;
}

ProcessRep::~ProcessRep()
{
    // Because of dequeue process next node is not deleted
}

ProcessRep* ProcessRep::getNext()
{
    // returns next node
    return this->mpNext;
}

void ProcessRep::setNext(ProcessRep *next)
{
    // sets next node
    this->mpNext = next;
}

string ProcessRep::getProcessType()
{
    // returns process type
    return this->mProcessType;
} 
void ProcessRep::setProcessType(string processType)
{
    // sets process type
    this->mProcessType = processType;
}

int ProcessRep::getDeliverTime()
{
    // returns deliver time
    return this->mDeliverTime;
}

void ProcessRep::setDeliverTime(int deliverTime)
{
    // sets deliver time
    this->mDeliverTime = deliverTime;
}

int ProcessRep::getProcessID()
{
    // returns process id
    return this->mProcessID;
}

void ProcessRep::setProcessID(int processID)
{
    // sets process id
    this->mProcessID = processID;
}

int ProcessRep::getProcessTime()
{   
    // returns process time
    return this->mProcessTime;
}

void ProcessRep::setProcessTime(int ProcessTime)
{
    // sets process time
    this->mProcessTime = ProcessTime;
}