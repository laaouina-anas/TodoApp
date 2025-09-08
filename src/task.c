// allow all extentions
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "task.h"
#include "settings.h"
#include "utils.h"


Task *newTask()
{
    
    Task *task = malloc(sizeof(Task));
    if(!task)
    {
        perror("newTask");
        return NULL;
    }

    task->description = NULL;
    task->descriptionSize = 0;
    task->done = 0;
    task->id = -1;
    task->time = time(NULL);
    return task;
}

Task *initTask(const char *description, bool done, TaskArr *arr)
{
    if(!description || strlen(description) == 0 || !arr)
    {
        INVALID_ARGS();
        return NULL;
    }

    Task *task = malloc(sizeof(Task));
    if(!task)
    {
        perror("newTask");
        return NULL;
    }

    task->description = strdup(description);
    task->descriptionSize = strlen(task->description);
    task->done = done;
    task->id = generateRandomId(arr);
    task->time = time(NULL);
    return task;
}

bool setTaskDescription(Task *task, const char *description)
{
    if(!task || !description)
    {
        INVALID_ARGS();
        return false;
    }
    free(task->description);
    task->description = strdup(description);
    task->descriptionSize = strlen(task->description);
    return true;
}

bool setTaskStatus(Task *task, bool done)
{
    if(!task)
    {
        INVALID_ARGS();
        return false;
    }
    task->done = done;
    return true;
}

void destroyTask(Task *task)
{
    if(!task)
    {
        INVALID_ARGS();
        return;
    }
    free(task->description);
    free(task);
}

TaskArr *newTaskArr(size_t capacity)
{
    TaskArr *arr = malloc(sizeof(TaskArr));

    if(!arr || capacity == 0)
    {
        perror("newTaskArr");
        return NULL;
    }

    arr->tasks = malloc(sizeof(Task *) * capacity);

    if(!arr->tasks)
    {
        free(arr);
        perror("newTaskArr");
        return NULL;
    }
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void clearTaskArr(TaskArr *arr)
{
    if(!arr)
    {
        INVALID_ARGS();
        return;
    }

    for(size_t i = 0; i < arr->size; i++)
    {
        destroyTask(arr->tasks[i]);
    }

    arr->size = 0;
}


void destroyTaskArr(TaskArr *arr)
{
    if(!arr)
    {
        INVALID_ARGS();
        return;
    }

    for(size_t i = 0; i < arr->size; i++)
    {
        destroyTask(arr->tasks[i]);
    }

    free(arr);
}



bool taskArrAdd(TaskArr *arr, Task *task)
{
    if(!arr || !task)
    {
        INVALID_ARGS();
        return false;
    }
    if(arr->size == arr->capacity)
    {
        size_t newCapacity = arr->capacity * 2;
        
        Task **newTaskArr = realloc(arr->tasks, newCapacity * sizeof(Task *));
        if(!newTaskArr)
        {
            return false;
        }
        arr->tasks = newTaskArr;
    }
    // task->id = generateRandomId(arr);
    arr->tasks[arr->size++] = task;
    return true;
}

size_t getTaskIndex(TaskArr *arr, int id)
{
    if(!arr || id < 0)
    {
        INVALID_ARGS();
        return SIZE_MAX;
    }

    for(size_t i = 0; i < arr->size; i++)
    {
        if(arr->tasks[i]->id == id)
        {
            return i;
        }
    }
    return SIZE_MAX;
}

Task *getTaskById(TaskArr *arr, int id)
{
    if(!arr || id < 0)
    {
        INVALID_ARGS();
        return NULL;
    }
    size_t foundIndex;

    if((foundIndex = getTaskIndex(arr, id)) != SIZE_MAX)
    {
        return arr->tasks[foundIndex];
    }

    return NULL;
}

bool taskArrRemove(TaskArr *arr, int id)
{
    if(!arr || id < 0 || arr->size == 0)
    {
        INVALID_ARGS();
        return false;
    }

    size_t index = getTaskIndex(arr, id);
    if(index == SIZE_MAX)
    {
        return false;
    }
    

    destroyTask(arr->tasks[index]); // same as : destroyTask(task);

    // shift all pointers left by 1
    for(size_t i = index; (i + 1) < arr->size ; i++)
    {
        arr->tasks[i] =  arr->tasks[i + 1];
    }

    // decrease the size
    arr->size--;

    return true;

}

bool setTaskDescriptionById(TaskArr *arr, int id, const char *description)
{
    if(!arr || id < 0 || !description)
    {
        INVALID_ARGS();
        return false;
    }

    size_t index = getTaskIndex(arr, id);
    if(index == SIZE_MAX)
    {
        return false;
    }

    return setTaskDescription(arr->tasks[index], description);
    
}

bool setTaskStatusById(TaskArr *arr, int id, bool done)
{
    if(!arr || id < 0)
    {
        INVALID_ARGS();
        return false;
    }
    size_t index = getTaskIndex(arr, id);
    if(index == SIZE_MAX)
    {
        return false;
    }
    arr->tasks[index]->done = done;
    return true;
}

TaskArr *loadTasks(const char *path)
{

    if(!path || strlen(path) == 0)
    {
        INVALID_ARGS();
        return NULL;
    }

    FILE *file = fopen(path, "rb+"); /* rb+ for reading and write later */

    if(!file && errno == ENOENT) // file does not exist, create it
    {
        file = fopen(path, "wb");
        if(!file)
        {
            fprintf(stderr, "failed to create %s\n",path);
            return NULL;
        }

        // now write a header //
        FileHeader header = {.magic = MAGIC, .reserved = 0, .version = 1, .taskCount = 0};
        if(fwrite(&header, sizeof(header), 1, file) != 1)
        {
            perror("failed to write header to the file");
            fclose(file);
            return NULL;
        }
        
        // header written succusfully, now allocate empty taskarr
        TaskArr *arr = newTaskArr(DEFAULT_CAPACITY);
        if(!arr)
        {
            perror("newTaskArr");
            return NULL;
        }

        return arr;
    }

    // check if the file is empty //

    if(fseek(file, 0, SEEK_END) != 0)
    {
        perror("fseek");
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if(size == 0) // file is empty
    {
        // file is opened in reading/write mode //
        fflush(file);
        rewind(file);
        FileHeader header = {.magic = MAGIC, .reserved = 0, .version = 1, .taskCount = 0};
        if(fwrite(&header, sizeof(header), 1, file) != 1)
        {
            perror("failed to write header to the file");
            fclose(file);
            return NULL;
        }
        
        // header written succusfully, now allocate empty taskArr
        TaskArr *arr = newTaskArr(DEFAULT_CAPACITY);
        if(!arr)
        {
            perror("newTaskArr");
            fclose(file);
            return NULL;
        }

        fclose(file);
        return arr;
    }
    // switch back to reading //
    fflush(file);
    rewind(file);
    
    // check file //
    FileHeader header = {0};
    if(fread(&header, sizeof(FileHeader), 1, file) != 1 || header.magic != MAGIC)
    {
        perror("fread");
        return NULL;
    }

    // allocate TaskArr //
    TaskArr *arr = newTaskArr((header.taskCount ? header.taskCount * 2 : DEFAULT_CAPACITY));
    if(!arr)
    {
        perror("newTaskArr");
        fclose(file);
        return NULL;
    }

    // reading all tasks //
    Task *buffer;
    
    for(size_t i = 0; i < header.taskCount; i++)
    {
        buffer = newTask();
        if(!buffer)
        {
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }
        // read id //
        if(fread(&buffer->id, sizeof(buffer->id), 1, file) != 1)
        {
            perror("failed to read tasks, file preserved ..");
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        // read description size //
        if(fread(&buffer->descriptionSize, sizeof(buffer->descriptionSize), 1, file) != 1)
        {
            perror("failed to read tasks, file preserved ..");
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        // read description string //
        char *str = malloc(buffer->descriptionSize + 1);
        if(!str)
        {
            perror("failed to read tasks, file preserved ..");
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        if(fread(str, buffer->descriptionSize + 1, 1, file) != 1)
        {
            perror("failed to read tasks, file preserved ..");
            free(str);
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        buffer->description = str;

        // read time //
        if(fread(&buffer->time, sizeof(buffer->time), 1, file) != 1)
        {
            perror("failed to read tasks, file preserved ..");
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        // read status //
        if(fread(&buffer->done, sizeof(buffer->done), 1, file) != 1)
        {
            perror("failed to read tasks, file preserved ..");
            fclose(file);
            destroyTaskArr(arr);
            return NULL;
        }

        // now add this data to the arr, arr takes ownership //
        taskArrAdd(arr, buffer);
        buffer = NULL; // reset the buffer to NULL //
    }

    return arr;

}

bool saveTasks(TaskArr *arr, const char *path)
{
    // check input //
    if(!arr || !path)
    {
        INVALID_ARGS();
        return false;
    }

    // open a temporary file //
    char tmp[LINE];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    FILE *file = fopen(tmp, "wb");
    if(!file)
    {
        perror("fopen");
        return false;
    }


    // write the header //
    FileHeader header = {.magic = MAGIC, .version = 1, .reserved = 0, .taskCount = arr->size};

    if(fwrite(&header, sizeof(FileHeader), 1, file) != 1)
    {
        perror("fwrite failed");
        goto cleanup;
    }

    // write all tasks //
    for(size_t i = 0; i < arr->size; i++)
    {
        // order : id, desc size, desc buffer, time, done
        if(fwrite(&arr->tasks[i]->id, sizeof(arr->tasks[i]->id), 1, file) != 1)
        {
            perror("fwrite failed to write tasks, file preserved ..");
            goto cleanup;
        }

        
        if(fwrite(&arr->tasks[i]->descriptionSize, sizeof(arr->tasks[i]->descriptionSize), 1, file) != 1)
        {
            perror("fwrite failed to write tasks, file preserved ..");
            goto cleanup;
        }

        if(fwrite(arr->tasks[i]->description, arr->tasks[i]->descriptionSize + 1, 1, file) != 1)
        {
            perror("fwrite failed to write tasks, file preserved ..");
            goto cleanup;
        }

        if(fwrite(&arr->tasks[i]->time, sizeof(arr->tasks[i]->time), 1, file) != 1)
        {
            perror("fwrite failed to write tasks, file preserved ..");
            goto cleanup;
        }

        if(fwrite(&arr->tasks[i]->done, sizeof(arr->tasks[i]->done), 1, file) != 1)
        {
            perror("fwrite failed to write tasks, file preserved ..");
            goto cleanup;
        }   
    }

    // all tasks are written to temp files //
    // replace the original file //

    remove(path);
    rename(tmp, path);
    fclose(file);
    return true;

cleanup:
    if(file) fclose(file);
    remove(tmp);
    return false;

}

// output //

void printTask(Task *task)
{
    if(!task) return;
    struct tm *t = localtime(&task->time);
    char buffer[LINE];
    strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M", t);
    printf(" id : %03d | desc : %-50s | time: %s | done: %-s\n",
    task->id, task->description, buffer, task->done ? "yes" : "no");
}

void printTaskArr(TaskArr *arr)
{
    if(!arr) return;
    printf("Tasks : \n");
    for (size_t i = 0; i < arr->size; i++)
    {
        printTask(arr->tasks[i]);
    }
}


