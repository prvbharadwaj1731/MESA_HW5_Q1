/*
    Author: Pranav Bharadwaj
    Submission: Mastering Embedded System Architecture (ECEN 5803) Homework 5

    Description: Provides API for state machine for each task. Tasks are provided by external code/events running.
    Some important assumptions about state machine is provided below:    
    1. Once a task is terminated, state machine shall terminate. 
    2. Once a task is sent to the kernel for execution, it is assumed to execute with the only exception of an interrupt/servicecall() being generated.
       After this, the task is assumed to be complete, and the state machine associated with it is complete.
    3. Since we cannot emulate events occuring in real-time in parallel with the state machine operation,
       we shall create a history of events that each input task will operate with. This way, we can obtain the different 
       state transitions a task goes through.
    4. Each task has a state machine associated with itself, and sees a complete virtual system available to itself
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "state_machine.h"

//Function returning different state names from their respective enum values for logging functionality
static const char* getStateName(state input_state_enum)
{
    switch(input_state_enum){
        case new_task: return "New Task";
        case suspended_ready: return "Suspended Ready";
        case ready: return "Ready";
        case running: return "Running";
        case waiting: return "Waiting";
        case blocked: return "Blocked";
        case suspended_blocked: return "Suspended Blocked";
        case suspended_delayed: return "Suspended Delayed";
        case terminated: return "Terminated";
        case kernel: return "Kernel";
        default: return "Illegal enum value";
    }
}


//State change function
static void changeState(state curr_state, state new_state)
{
#ifdef ENABLE_LOGGING
    printf("State transition from %s to %s occured.\n", getStateName(curr_state), getStateName(new_state));
#endif    
    curr_state = new_state;  //this operation should be atomic
    printf("Current state = %s.\n", getStateName(curr_state));
}

/*
    Description: State machine being handled by this function. Accepts a current task 
    and a list(array) of events passed by reference as input.
*/
void handleTask(task_s input_task, event *current_event)
{
    //Printing task ID
    printf("State machine begin for task with ID = %d.\n", input_task.task_id);
    //Set task state to default state
    input_task.current_state = DEFAULT_STATE;
    while(input_task.current_state != terminated || input_task.current_state != kernel){
            switch(input_task.current_state){
        case new_task:
            if(*current_event == create || *current_event == admit)
                changeState(input_task.current_state, suspended_ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
        break;

        case suspended_ready:
            if(*current_event == activate || *current_event == exception_cleared)
                changeState(input_task.current_state, ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
        break;

        case ready:
            if(*current_event == dispatch)
                changeState(input_task.current_state, running);
            else if(*current_event == suspend || *current_event == exception)
                changeState(input_task.current_state, suspended_ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
        break;

        case running:
            if(*current_event == preempt)
                changeState(input_task.current_state, ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
            else if(*current_event == suspend || *current_event == exception)
                changeState(input_task.current_state, suspended_ready);
            else if(*current_event == semtake || *current_event == wait || *current_event == mutex_lock)
                changeState(input_task.current_state, blocked);
            else if(*current_event == task_delay || *current_event == pause || *current_event == sleep)
                changeState(input_task.current_state, waiting);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
            else //No other event has occured, indicating that task is picked up by kernel for execution
                changeState(input_task.current_state, kernel);
        break;

        case waiting:
            if(*current_event == suspend || *current_event == exception)
                changeState(input_task.current_state, suspended_delayed);
            if(*current_event == delay_timeout || *current_event == wakeup)
                changeState(input_task.current_state, ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);

        case blocked:
            if(*current_event == mutex_unlock || *current_event == semgive)
                changeState(input_task.current_state, ready);
            else if(*current_event == suspend || *current_event == exception)
                changeState(input_task.current_state, suspended_blocked);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
            
        break;

        case suspended_blocked:
            if(*current_event == activate || *current_event == exception_cleared)
                changeState(input_task.current_state, blocked);
            if(*current_event == mutex_unlock || *current_event == semgive)
                changeState(input_task.current_state, suspended_ready);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);
            
        break;

        case suspended_delayed:
            if(*current_event == delay_timeout || *current_event == wakeup)
                changeState(input_task.current_state, suspended_ready);
            if(*current_event == activate || *current_event == exception_cleared)
                changeState(input_task.current_state, waiting);
            else if(*current_event == terminate)
                changeState(input_task.current_state, terminated);

        break;

        case terminated:
            printf("Task terminated by user. Exiting...\n");
            //Do nothing
        break;

        case kernel:
            printf("Kernel receieved task. Execution complete. Exiting...\n");
            //Do nothing
        break;

        default:
            printf("Illegal state. Exiting...\n");
            //Do nothing
        break;
        
    }

    //Advance event list pointer
    current_event++;
    }

}





//Function handlers for different states
