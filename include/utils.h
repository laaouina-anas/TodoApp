#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include "task.h"

#define INVALID_ARGS() fprintf(stderr, "invalid args in %s:%d function %s\n", __FILE__, __LINE__, __func__)

/// @brief Generates unique random id
/// @param arr Array used to ensure uniqueness
/// @return Random Id on success, -1 on failure
int generateRandomId(TaskArr *arr);

/// @brief Checks if an id is unique
/// @param arr Array used to check uniqueness
/// @return true if unique and positive, false otherwise
bool uniqueId(TaskArr *arr, int id);

/// @brief utility function to check file existance
/// @param path path string
/// @return true if file exists, false otherwise
bool fileExists(const char *path);


/// @brief atoi doesn't have error cheking, it was crucial to check valid integers
/// @param str string to check
/// @return true if valid string integer, false otherwise
bool isValidInteger(const char *str);


/// @brief Portable function to get the home directory, it accesses enviroment variable
/// @return NULL on failure, the string on success
char *getHomeDir();

#endif // UTILS_H


