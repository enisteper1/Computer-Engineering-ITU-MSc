/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
    this->mFinishedProcess = new FIFORep();
}

CPURep::~CPURep()
{
    delete this->mFinishedProcess;
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   
    /*
        The function takes the process and simulation time as input and returns it.
        If the process is fnished the time is assigned into process endTime.
        
    */
    if(p!=NULL)
    {
        p->remainingTime--;
        if (p->remainingTime == 0)
        {
            p->setNext(NULL);
            p->endTime = time;
            this->mFinishedProcess->queue(p);
            return NULL;
        }
        else
        {
            return p;
        }
    }
    else
    {
        return NULL;
    }
}

FIFORep* CPURep::getFinishedProcess()
{
    return this->mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    this->mFinishedProcess = finishedProcess;
}