/*
    Author: Pranav Bharadwaj

    Description: main() that provides tasks and list of events to scheduler function. 
    Log of state transitions printed if ENABLE_LOGGING is defined to be true
*/


#include <stdio.h>
#include <stdbool.h>
#include "state_machine.h"

#define TASK1_ID    1234
#define TASK2_ID    2048

//Globals defining tasks and list of events to pass as inputs
task_s input_task_1_g = {TASK1_ID, new_task};
task_s input_task_2_g = {TASK2_ID, new_task};

event event_list_1_g[7] = {create, activate, dispatch, semtake, semgive, dispatch, terminate};
event event_list_2_g[9] = {create, exception_cleared, suspend, activate, dispatch, task_delay, delay_timeout, dispatch, terminate};


void main()
{
    //Pass task and associated event list of scheduler handler function
    handleTask(input_task_1_g, event_list_1_g);
    handleTask(input_task_2_g, event_list_2_g);

    printf("Scheduler complete. Exiting...\n");
}