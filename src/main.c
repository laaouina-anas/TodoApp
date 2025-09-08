#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "task.h"
#include "settings.h"
#include "utils.h"


/*
Usage:
    task
    task [-a --add| -d --delete] [list of strings or ids separated by space]
    task [-c --clear | -h --help]
    task [-m --modify] [id] [-s --status | -t --text] [text | [done | undone]]
*/

typedef enum Option {ADD, DELETE, MODIFY, CLEAR, HELP, LIST, VERSION, NONE} Option;

void displayUsage()
{
    const char usage[] =\
    "Usage:\n" 
    " task\n"
    " task [-h --help | -c --clear | -l --list]\n"
    " task [-a --add | -d --delete] [list of strings or ids separated by space]\n"
    " task [-m --modify] [id] [-s --status | -t --text] [text | [d | n]]\n"
    "Options :\n"
    " -h --help  : display this help output\n"
    " -a --add   : add a list of tasks\n"
    " -d --delete: delete a list of tasks by id"
    " -l --list  : print your task list\n"
    " -c --clear : clear your task list, this action is permanent\n"
    " -m --modify: modify a specific task by it's id\n"
    " -s --status: for modifying the status, see usage above, 'd' is for done, 'n' is for not done\n"
    " -t --text  : for modifying the text/description, see usage above\n";

    printf("%s", usage);

}

Option getOption(const char *str)
{
    if(strlen(str) == 0 || str[0] != '-') return NONE;
    if(strcmp("-a", str) == 0 || 0 == strcmp("--add", str)) return ADD;
    if(strcmp("-d", str) == 0 || 0 == strcmp("--delete", str)) return DELETE;
    if(strcmp("-v", str) == 0 || 0 == strcmp("--version", str)) return VERSION;
    if(strcmp("-m", str) == 0 || 0 == strcmp("--modify", str)) return MODIFY;
    if(strcmp("-h", str) == 0 || 0 == strcmp("--help", str)) return HELP;
    if(strcmp("-c", str) == 0 || 0 == strcmp("--clear", str)) return CLEAR;
    if(strcmp("-l", str) == 0 || 0 == strcmp("--list", str)) return LIST;
    return NONE;
}


int main(int argc, char const *argv[])
{

    char path[LINE];
    char *home = getHomeDir();
    if(!home)
    {
        fprintf(stderr, "could't find the home directory\n");
        return EXIT_FAILURE;
    }
    sprintf(path, "%s%c%s", home, SLASH, FILENAME);

    TaskArr *arr = loadTasks(path);
    if(!arr)
    {
        fprintf(stderr, "failed to load tasks\n");
        return EXIT_FAILURE;
    }

    if(argc <= 1)
    {
        displayUsage();
        return EXIT_SUCCESS;
    }

    if(argc == 2)
    {
        switch (getOption(argv[1]))
        {
        case LIST:
            printTaskArr(arr);
            break;
        case CLEAR:
            clearTaskArr(arr);
            printf("tasks have been cleared!\n");
            break;
        case VERSION:
            printf("Task management version 1.0\n");
            break;
        case HELP:
        default:
            displayUsage();
            break;
        }
    }
    else
    {
        switch (getOption(argv[1]))
        {
        case ADD:
            // add all strings //
            Task *task;
            for(int i = 2; i < argc; i++)
            {
                task = initTask(argv[i], 0, arr);
                if(!task)
                {
                    printf("failed to add : %s\n", argv[i]);
                    break;
                }
                taskArrAdd(arr, task);
            }
            printf("tasks have been added succesfully\n");
            break;
        case DELETE:
            // remove all tasks with id //
            for(int i = 2; i < argc; i++)
            {
                // check if conversion hapened //
                if(isValidInteger(argv[i]))
                {
                    printf("%s is not a valid id\n", argv[i]);
                    break;
                }
                int id = atoi(argv[i]);
                bool removed = taskArrRemove(arr, id);

                if(removed) printf("task \"%s\" has been removed\n", argv[i]);
                else printf("task \"%s\" hasn't been removed\n", argv[i]);

            }
            //printTaskArr(arr);
            break;
        case MODIFY:
            if(argc != 5)
            {
                displayUsage();
                break;
            }

            // get the id //
            if(!isValidInteger(argv[2]))
            {
                printf("\"%s\" is not a valid id\n", argv[2]);
                break;
            }

            int id = atoi(argv[2]);
            bool success = 1;
            if(strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--status") == 0)
            {
                if(strcmp("d", argv[4]) == 0)
                {
                    success = setTaskStatusById(arr, id, 1);
                    if(success) printf("status set successfully\n");
                }
                else if(strcmp("n", argv[4]) == 0)
                {
                    success = setTaskStatusById(arr, id, 0);
                    if(success) printf("status set successfully\n");
                }
                else
                {
                    printf("status wasn't set\n");
                    displayUsage();
                }
                if(!success) printf("status wasn't set\n");
                break;
            }
            else if(strcmp(argv[3], "-t") == 0 || strcmp(argv[3], "--text") == 0)
            {
                success = setTaskDescriptionById(arr, id, argv[4]);
                if(success)
                    printf("description was set succesfully\n");
                else 
                    printf("description wasn't set\n");
            }
            else
            {
                displayUsage();
            }
            break;
        default:
            displayUsage();
            break;
        }
    }
    
    saveTasks(arr, path);
    destroyTaskArr(arr);
    return 0;
}
