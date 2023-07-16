/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    this->mpProcessFIFO[0] = new FIFORep();
    this->mpProcessFIFO[1] = new FIFORep();
    this->mpProcessFIFO[2] = new FIFORep();
    this->mpRunningProcess = NULL;
    this->timeSliceCount = 0;
    this->totalTime = 0;
    this->pCpuObj = new CPURep();
}

SchedulerRep::~SchedulerRep()
{
    delete this->mpProcessFIFO[0];
    delete this->mpProcessFIFO[1];
    delete this->mpProcessFIFO[2];
    delete this->mpRunningProcess;
    delete this->pCpuObj;
}

FIFORep* SchedulerRep::getProcessFIFO(int index)
{
    return this->mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep* fifo, int index)
{
    this->mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep* p)
{
    this->mpRunningProcess = p;
}

ProcessRep* SchedulerRep::getRunningProcess()
{
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep* p)
{
    if (p->remainingTime>0)
    {
        if (p->getProcessType() == "A")
        {
            this->mpProcessFIFO[0]->queue(p);
        }
        else if (p->getProcessType() == "B")
        {
            this->mpProcessFIFO[1]->queue(p);
        }
        else if (p->getProcessType() == "C")
        {
            this->mpProcessFIFO[2]->queue(p);
        } 
    }
}

ProcessRep* SchedulerRep::popProcess()
{
    ProcessRep* p = NULL;
    if (this->mpProcessFIFO[0]->getHead() != NULL)
    {
        p = this->mpProcessFIFO[0]->dequeue();
    }
    else if (this->mpProcessFIFO[1]->getHead() != NULL)
    {
        p = this->mpProcessFIFO[1]->dequeue();
    }
    else if (this->mpProcessFIFO[2]->getHead() != NULL)
    {
        p = this->mpProcessFIFO[2]->dequeue();
    }
    else
    {
        p = NULL;
    }

    if(p!=NULL)
    {
        this->timeSliceCount = 0;
    }
    return p;
}

bool SchedulerRep::checkTimeSlice()
{   
    if(this->getRunningProcess()!=NULL){    
        if(this->getRunningProcess()->getProcessType()== "A")
        {
            if (this->timeSliceCount == 2)
            {
                this->timeSliceCount = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(this->getRunningProcess()->getProcessType()== "B")
        {
            if (this->timeSliceCount == 4)
            {
                this->timeSliceCount = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(this->getRunningProcess()->getProcessType()== "C")
        {
            if (this->timeSliceCount == 8)
            {
                this->timeSliceCount = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }    
    }
    else{
        return true;
    }
}
ProcessRep* SchedulerRep::sendProcessToCPU(ProcessRep* p){
    this->setRunningProcess(p);
    timeSliceCount++;
    this->totalTime++;
    ProcessRep* temp = this->pCpuObj->runCPU(p, this->totalTime);
    if(p!=NULL)
    {
        if (p->startTime == -1)
        {
            p->startTime = this->totalTime-1;
        }
    }

    if (temp == NULL && p!=NULL)
    {
        temp = this->popProcess();
        if(temp!=NULL)
        {
            if (temp->startTime == -1)
            {
                temp->startTime = this->totalTime;
            }
        }
    }
    
    return temp;
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
    ProcessRep *p = new ProcessRep(type, id, arrivalTime, processTime);
    ProcessRep* temp;
    if(p != NULL){
        this->pushProcess(p);
        if(this->mpRunningProcess !=NULL)
        {
            if(this->mpRunningProcess->getProcessType() > p->getProcessType())
            {
                this->pushProcess(this->mpRunningProcess);
                temp = this->popProcess();
            }
            else{
                if(this->checkTimeSlice()==true)
                {
                    /*  
                        if the process utilize the CPU along time slice of its level's and it has remaining time
                        then the process is put back in its own fifo.
                    */
                    if(this->mpRunningProcess->remainingTime>0)
                    {
                        this->pushProcess(this->mpRunningProcess);
                        
                    }
                    temp = this->popProcess();
                }
                else
                {
                    temp = this->mpRunningProcess;
                }
            }
        }
        else
        {
            temp = this->popProcess();
        }
    }
    else{
        if(this->mpRunningProcess !=NULL){
            if(this->checkTimeSlice()==true)
            {
                /*  
                    if the process utilize the CPU along time slice of its level's and it has remaining time
                    then the process is put back in its own fifo.
                */
                if(this->mpRunningProcess->remainingTime>0){
                    this->pushProcess(this->mpRunningProcess);
                    
                }
                temp = this->popProcess();
            }
            else{
                temp = this->mpRunningProcess;
            }
        }
        else
        {
            temp = this->popProcess();
        }
    }

    this->mpRunningProcess = this->sendProcessToCPU(temp);
}
void SchedulerRep::schedule(ProcessRep* p)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
    ProcessRep* temp;
    if(p != NULL){
        this->pushProcess(p);
        if(this->mpRunningProcess !=NULL)
        {
            if(this->mpRunningProcess->getProcessType() > p->getProcessType())
            {
                this->pushProcess(this->mpRunningProcess);
                temp = this->popProcess();
            }
            else{
                if(this->checkTimeSlice()==true)
                {
                    /*  
                        if the process utilize the CPU along time slice of its level's and it has remaining time
                        then the process is put back in its own fifo.
                    */
                    if(this->mpRunningProcess->remainingTime>0)
                    {
                        this->pushProcess(this->mpRunningProcess);
                        
                    }
                    temp = this->popProcess();
                }
                else
                {
                    temp = this->mpRunningProcess;
                }
            }
        }
        else
        {
            temp = this->popProcess();
        }
    }
    else{
        if(this->mpRunningProcess !=NULL){
            if(this->checkTimeSlice()==true)
            {
                /*  
                    if the process utilize the CPU along time slice of its level's and it has remaining time
                    then the process is put back in its own fifo.
                */
                if(this->mpRunningProcess->remainingTime>0){
                    this->pushProcess(this->mpRunningProcess);
                    
                }
                temp = this->popProcess();
            }
            else{
                temp = this->mpRunningProcess;
            }
        }
        else
        {
            temp = this->popProcess();
        }
    }

    
    this->mpRunningProcess = this->sendProcessToCPU(temp);
}

// ProcessRep* SchedulerRep::findProcess(string type, int id)
// {
//     /*  
//         The function search for the process whose type and id is defined by the function arguments.
//         It returns the pointer of the process if the process is found, otherwise it returns NULL.    
//     */
//     ProcessRep* temp = this->mpRunningProcess;
//     int fifo_index = 0;

//     if (type == "A")
//     {
//        fifo_index = 0; 
//     }
//     else if (type == "B")
//     {
//         fifo_index = 1; 
//     }
//     else if (type == "C")
//     {
//         fifo_index = 2; 
//     }

//     if(temp==NULL) 
//     {
//         temp = this->mpProcessFIFO[fifo_index]->searchID(id);
//         if(temp==NULL)
//         {
//             temp = this->pCpuObj->getFinishedProcess()->searchID(id);           
//         }
//     } 
//     else
//     {
//         if(temp->getProcessType() != type || temp->getProcessID() != id ) 
//         {
//             temp = this->mpProcessFIFO[fifo_index]->searchID(id);
//             if(temp==NULL)
//             {
//                 temp = this->pCpuObj->getFinishedProcess()->searchID(id);           
//             }
//         }
//     }

//     return temp;
// }