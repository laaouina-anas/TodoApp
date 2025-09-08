
#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"

/// @brief Task struct
typedef struct Task
{
    int id; // unique id for each Task
    size_t descriptionSize; // the size of the description
    char *description; // pointer to the description on the heap
    bool done; // true if the Task is completed, false otherwise
    time_t time; // Time of Task creation

} Task;

/// @brief An array of Task struct on the heap
/// @note This struct gets ownership of all task allocations, DO NOT free individual task structs
typedef struct TaskArr
{
    Task **tasks; // Array of (Task *) each pointing to a Task object
    size_t size; // number of Task structs
    size_t capacity; // How much element this array can currently hold

} TaskArr;


/// @brief Makes an empty Task struct, id set = -1
/// @return A pointer to Task struct on success, NULL on failure
Task *newTask();


/// @brief creates and intialises a Task struct
/// @param description a string literal - will be copied using strdup
/// @param done a boolean for a Task completion
/// @param arr used for unique id generation
/// @return A pointer to Task struct on success, NULL on failure
Task *initTask(const char *description, bool done, TaskArr *arr);

/// @brief changes task description
/// @param task Pointer to task object to modify, NULL is ignored
/// @param description new string description 
/// @return true on success, false on failure
bool setTaskDescription(Task *task, const char *description);

/// @brief changes task status using id
/// @param task Task object to modify
/// @param done new status
/// @return true on success, false on failure
bool setTaskStatus(Task *task, bool done);


/// @brief clears the TaskArr
/// @param arr A pointer to TaskArr, ignored if NULL, cleared otherwise
void clearTaskArr(TaskArr *arr);

/// @brief frees Task struct from memory
/// @param Task A pointer to Task, ignored if NULL, freed otherwise
void destroyTask(Task *task);


/// @brief Creates an empty Task array
/// @param capacity Initial capacity for the array
/// @return TaskArr pointer on success, NULL on failure
TaskArr *newTaskArr(size_t capacity);

/// @brief Frees TaskArr object from memory
/// @param arr Array of tasks to free, ignored if NULL
void destroyTaskArr(TaskArr *arr);

/// @brief Pushes a Task object back into the array
/// @param Task Task pointer, ignored if NULL
/// @param arr TaskArr pointer, ignored if NULL
/// @return true on sucess, false on failure
bool taskArrAdd(TaskArr *arr, Task *task);

/// @brief Gets a Task index by it's id
/// @param arr Task array to search
/// @param id selected id
/// @return index of task on success, SIZE_MAX otherwise
size_t getTaskIndex(TaskArr *arr, int id);

/// @brief Gets a Task by it's id
/// @param arr Task array to search
/// @param id selected id
/// @return ponter to task on success, NULL otherwise
Task *getTaskById(TaskArr *arr, int id);

/// @brief Removes a Task object from the array
/// @param Task Task pointer, ignored if NULL
/// @param id unique id
/// @return true on sucess, false on failure
bool taskArrRemove(TaskArr *arr, int id);


/// @brief changes task description using id
/// @param arr Task array to search
/// @param id selected id
/// @param description new description string
/// @return true on success, false on failure
bool setTaskDescriptionById(TaskArr *arr, int id, const char *description);

/// @brief changes task status using id
/// @param arr Task array to search
/// @param id selected id
/// @param done new status
/// @return true on success, false on failure
bool setTaskStatusById(TaskArr *arr, int id, bool done);


/// >>>>>>>>>>>>>> STORAGE MANAGEMENT <<<<<<<<<<<<<<<<<< //

typedef struct FileHeader
{
    int magic;
    short version;
    short reserved;
    size_t taskCount;

} FileHeader;



/// @brief loads TaskArr stored file
/// @param path path of the file
/// @return TaskArr pointer (allocated)
/// @note each task's attribute is read in this sequence (id -> desc size -> desc string -> time -> done)
TaskArr *loadTasks(const char *path);

/// @brief saves TaskArr stored file atomically
/// @param path path of the file
/// @param TaskArr pointer (allocated)
/// @return true on success, false on failure
/// @note each task is saved in this sequence (id -> desc size -> desc string -> time -> done)
/// @note if this function fails, the original file is preserved
bool saveTasks(TaskArr *arr ,const char *path);


/// @brief displays a readable format of the task object
/// @param task a pointer to a task struct
void printTask(Task *task);

/// @brief displays a readable format of the taskArr object
/// @param task a pointer to a taskArr struct
void printTaskArr(TaskArr *arr);


#endif // TASK_H






