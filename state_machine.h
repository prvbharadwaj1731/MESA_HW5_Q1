/*
    Description: Holds enums for different states in the state machine, and function descriptions for different events possible
    Author: Pranav Bharadwaj
*/

/*
    Enum holding all possible states
    Default state chosen: New Task
*/

#define NUMBER_OF_STATES    10
#define NUMBER_OF_EVENTS    21
#define DEFAULT_STATE   new_task
//#define ENABLE_LOGGING            //Enable this to print state transitions

typedef enum{
    new_task=0,
    suspended_ready=1,
    ready=2,
    running=3,
    waiting=4,
    blocked=5,
    suspended_blocked=6,
    suspended_delayed=7,
    terminated=8,
    kernel=9
}state;


/*
    Enum holding all possible events
*/
typedef enum{
    create,
    admit,
    delay_timeout,
    wakeup,
    mutex_unlock,
    semgive,
    activate,
    exception_cleared,
    suspend,
    exception,
    semtake,
    wait,
    mutex_lock,
    task_delay,
    pause,
    sleep,
    terminate,
    interrupt,
    service_call,
    dispatch,
    preempt
}event;


//Structure holding input task data
typedef struct{
    int task_id;
    state current_state;
}task_s;


//Function descriptions for state machine handler
void handleTask(task_s, event *);
