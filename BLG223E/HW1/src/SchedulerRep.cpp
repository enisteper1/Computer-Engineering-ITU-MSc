/* @Author
Student Name: Enis Teper
Student ID : 504221511
Date: 26/10/2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    // Initial assignments to use the required variables
    this->totalTime = 0;
    this->timeSliceCount = 0;
    this->mpProcessFIFO[0] = new FIFORep();
    this->mpProcessFIFO[1] = new FIFORep();
    this->mpProcessFIFO[2] = new FIFORep();

    this->mpRunningProcess = NULL;
    this->pCpuObj = new CPURep();

}
SchedulerRep::~SchedulerRep()
{
    // frees the memory at destruction
    delete mpProcessFIFO[0];
    delete mpProcessFIFO[1];
    delete mpProcessFIFO[2];

    delete pCpuObj;
    delete mpRunningProcess;

}

FIFORep* SchedulerRep::getProcessFIFO(int index)
{
    // returns fifo with respect to index
    return this->mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep* fifo, int index)
{
    // Sets fifo with respect to index
    this->mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep* p)
{
    // If assignment is not null
    if( p != NULL)
    {
        // set starting time for job
        if(p->startTime == -1)
            p->startTime = totalTime;
    }
    this->mpRunningProcess = p;
}

ProcessRep* SchedulerRep::getRunningProcess()
{
    // returns current job
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep* p)
{
    // If job is type A
    if(p->getProcessType() == "A")
        mpProcessFIFO[0]->queue(p);

    // If job is type B
    else if(p->getProcessType() == "B")
        mpProcessFIFO[1]->queue(p);

    // If job is type C
    else
        mpProcessFIFO[2]->queue(p);
}

ProcessRep* SchedulerRep::popProcess()
{
    // From 0 to 2 if dequeue out is not null
    // first obtained job is returned.
    ProcessRep* p = NULL;
    for(int i=0; i < 3; i++)
    {
        p = mpProcessFIFO[i]->dequeue();
        if (p != NULL)
            return p;
    }
    delete p;
    return NULL;
}

bool SchedulerRep::checkTimeSlice()
{   
    // If running process is type A and reaches 2 then return true
    if(mpRunningProcess->getProcessType() == "A" & timeSliceCount == 2)
        return true;

    // If running process is type B and reaches 4 then return true
    else if(mpRunningProcess->getProcessType() == "B" & timeSliceCount == 4) 
        return true;

    // If running process is type C and reaches 8 then return true
    else if (mpRunningProcess->getProcessType() == "C" & timeSliceCount == 8)
        return true;

    // If no condition is satisfied that means running continues
    return false;
}
ProcessRep* SchedulerRep::sendProcessToCPU(ProcessRep* p)
{
    return pCpuObj->runCPU(p, totalTime);
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{   
    // If it is initial job start running process
    if (arrivalTime == 0)
    {
        // placeholder to set process
        ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
        // set process
        setRunningProcess(p);
        // run the job 
        sendProcessToCPU(mpRunningProcess);
        // p is not deleted because of now it is running process

    }
    else
    {

        if(type=="A")
        {   
            // If running process is type A then schedule the event and continue to run
            if(mpRunningProcess->getProcessType() == "A")
            {
                // placeholder to queue the process
                ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
                // queue the process
                pushProcess(p);

            }
            // If running process is type B or C then set running process to A
            else
            {
                // queue current process
                pushProcess(mpRunningProcess);
                // placeholder to set the process
                ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
                // set new process
                setRunningProcess(p);
                // since new process is assigned set quantum to 0
                timeSliceCount = 0;

            }
            
        }
        // If scheduled type is B queue if running process is A or B
        // If running process is C put it into queue and set runing process as B
        else if (type=="B")
            {
                if (mpRunningProcess->getProcessType() == "B" || mpRunningProcess->getProcessType() == "A")
                {   
                    // placeholder to queue the process
                    ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
                    // queue the process
                    pushProcess(p);

                }
                    
                // If running process is type C then assign it to scheduler
                // Since new work is starting assign 0 to timeSliceCount (Quantum)
                else
                {
                    // queue current process
                    pushProcess(mpRunningProcess);
                    // placeholder to set the process
                    ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
                    // set new process
                    setRunningProcess(p);
                    // since new process is assigned set quantum to 0
                    timeSliceCount = 0;
                }
            }
        // If scheduled type is C just put it in queue since it is least important
        else if (type=="C")
            {
                // placeholder to queue the process
                ProcessRep* p = new ProcessRep(type, id, arrivalTime, processTime);
                // queue the process
                pushProcess(p);
            }


            // If running process finishes it returns NULL
            // So we insert that to finised processes and assign new job
            ProcessRep* p3 = sendProcessToCPU(mpRunningProcess);
            // Checking if returns null
            if (p3 == NULL)
            {       // If p3 is null then get job from scheduler
                    ProcessRep* p2 = popProcess();

                    // If it is not null assign it to cpu
                    if(p2 != NULL)
                        setRunningProcess(p2);
                    
                    // Since new job assigned quantum is set to 0
                    timeSliceCount = 0;
                        
            }
            else
            {
                // If the job did not finish increase quantum
                timeSliceCount += 1;
                // If timeSliceCount reaches to limit 
                if(checkTimeSlice() == true)
                {
                    pushProcess(p3);

                    ProcessRep* p2 = popProcess();
                    if(p2 != NULL)
                    {
                        setRunningProcess(p2);
                        timeSliceCount = 0;
                    }

                }

            }
        
    }
    // Increasing time by 1
    totalTime += 1;
}

void SchedulerRep::schedule(ProcessRep* p)
{   
   // Return if the mpRunningProcess is null
   if(getRunningProcess() == NULL)
        return;


   // placeholder to check whether output of runingcpu is null or not
   p = sendProcessToCPU(mpRunningProcess);

   // If the current work finishes
   if (p == NULL)
   {
        // Get the job with highest priority
        // placeholder process to check whether output of popProcess is null or not
        ProcessRep* p2 = popProcess();
        
          // If any job still remains to get done assign that job to running process
        if(p2 != NULL)
            setRunningProcess(p2);
        // If all jobs are done assign NULL to mpRunningProcess
        else
            setRunningProcess(NULL);
        
        // Since new work starts assign 0 to quantum
        timeSliceCount = 0;
        delete p;
   }
   else
   {    
        // Increasing the quantum to check time
        timeSliceCount += 1;
        // Check the quantum
        if(checkTimeSlice() == true)
        {
            // If quantum is at limit push running process to relevant FIFORep (A, B, C)
            pushProcess(p);

            // Get the job with highest priority
            ProcessRep* p2 = popProcess();
            // If any job still remains to get done assign that job to running process
            if(p2 != NULL)
            {
                // set obtained job as running process
                setRunningProcess(p2);
                timeSliceCount = 0;
            }
            else
                delete p2;

        }
   }
   // Increasing time by 1
   totalTime += 1;


}
