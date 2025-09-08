#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "task.h"



void test()
{
    srand(time(NULL));

    TaskArr *arr = newTaskArr(5);
    assert(arr && "buy more ram bro");

    Task *t1 = initTask("initialized task 1", 0 ,arr);
    Task *t2 = initTask("initialized task 2", 1 ,arr);
    Task *t3 = initTask("initialized task 3", 1 ,arr);
    Task *t4 = initTask("initialized task 4", 0 ,arr);


    printf("Adding 4 taskes ...\n");
    taskArrAdd(arr, t1);
    taskArrAdd(arr, t2);
    taskArrAdd(arr, t3);
    taskArrAdd(arr, t4);
    printTaskArr(arr);
    
    printf("Removing task 1 (uninitialized) ...\n");
    taskArrRemove(arr, arr->tasks[1]->id);
    printTaskArr(arr);

    printf("changing the first task description to : call mom\n");
    setTaskDescriptionById(arr, arr->tasks[0]->id, "call mom");
    printTaskArr(arr);

    printf("changing intitialzed task 4 status to done = 1\n");
    setTaskStatusById(arr, t4->id, 1);
    printTaskArr(arr);

    printf("saving tasks ..\n");
    if(!saveTasks(arr, FILEPATH))
    {
        printf("tasks aren't saved\n");
    }

    printf("tasks are saved !\n");

    destroyTaskArr(arr);
    arr = NULL;

    printf("restoring task array after freeing it\n");
    arr = loadTasks(FILEPATH);
    if(!arr)
    {
        printf("failed to load tasks\n");
        return;
    }
    printf("print task arr ...\n");
    printTaskArr(arr);

    destroyTaskArr(arr);

}

int main(void)
{
    printf("Test file ...\n");
    test();
    return 0;
}
