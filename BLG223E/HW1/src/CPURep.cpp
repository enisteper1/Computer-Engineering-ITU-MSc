/* @Author
Student Name: Enis Teper
Student ID : 504221511
Date: 26/10/2022
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
    // Generates fifo to hold finished processes
    mFinishedProcess = new FIFORep();
}

CPURep::~CPURep()
{
    // frees the memory
    delete mFinishedProcess;
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   
    // remove remaining time from the job
    p->remainingTime -= 1;
    // If job finishes
    if (p->remainingTime == 0)
    {
        // set end time
        p->endTime = time+1;
        // insert job to finished queues
        mFinishedProcess->queue(p);
        // To indicate the job is finished return null
        return NULL;
    }
    return p;
}

FIFORep* CPURep::getFinishedProcess()
{
    // returns finished process FIFO
    return this->mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    // Sets finished process
    this->mFinishedProcess = finishedProcess;
}