#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include "utils.h"
#include "settings.h"

int generateRandomId(TaskArr *arr)
{
    if(!arr)
    {
        INVALID_ARGS();
        return -1;
    }

    int random;
    size_t capacity = (arr->capacity == arr->size ? arr->capacity * 2 : arr->capacity);
    do
    {
        random = rand() % capacity;

    } while (!uniqueId(arr, random));
    
    return random;
}

bool uniqueId(TaskArr *arr, int id)
{
    if(!arr || id < 0)
    {
        INVALID_ARGS();
        return false;
    }

    for(size_t i = 0; i < arr->size; i++)
    {
        if(arr->tasks[i]->id == id)
        {
            return false;
        }
    }
    return true;
}

bool fileExists(const char *path)
{
    FILE *file = fopen(path, "rb");
    if(!file)
    {
        return false;
    }
    fclose(file);
    return true;
}


bool isValidInteger(const char *str)
{
    if (str == NULL || *str == '\0') {
        return false;
    }
    
    // Optional sign
    if (*str == '+' || *str == '-') {
        str++;
    }
    
    // Must have at least one digit
    if (*str == '\0') {
        return false;
    }
    
    // All characters must be digits
    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return false;
        }
        str++;
    }
    
    return true;
}


char *getHomeDir()
{
    char *home = NULL;
    #ifdef _WIN32
    home = getenv("USERPROFILE");
    #else
    home = getenv("HOME");
    if(!home)
    {
        home = getpwuid(getuid())->pw_dir;
    }
    #endif
    return home;
}
