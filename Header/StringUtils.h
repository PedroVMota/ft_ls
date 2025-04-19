#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>


/// @brief Appends a new element to a dynamically allocated array of strings.
/// @param array The original array of strings.
/// @param newelement The new string to append.
char **append_child(char **array, char *newelement);

/// @brief Duplicates a string.
/// @param str The string to duplicate.
/// @return A pointer to the duplicated string.
/// The caller is responsible for freeing the memory allocated for the duplicated string.
char *duplicate_string(char *str);



char *concat_strings(size_t count, ...);

/// @brief Returns the length of a dynamically allocated array of strings.
/// @param dptr The array of strings.
/// @return The length of the array.
/// The function returns the number of strings in the array, excluding the NULL terminator.
size_t dptr_len(void **dptr, size_t ptr_size);

/// @brief Returns the length of a string.
/// @param str The string.
/// @return The length of the string.
/// The function returns the length of the string, excluding the null terminator.
size_t string_len(char *str);


#endif
