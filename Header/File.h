#ifndef FILE_H
#define FILE_H

#define _GNU_SOURCE
#include <unistd.h>     // write, read, readlink, close
#include <dirent.h>     // readdir, closedir
#include <sys/stat.h>   // stat, lstat
#include <pwd.h>        // getpwuid
#include <grp.h>        // getgrgid
#include <sys/xattr.h>  // listxattr, getxattr
#include <time.h>       // time, ctime
#include <stdlib.h>     // malloc, free, exit
#include <stdio.h>      // perror
#include <string.h>     // strerror
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include "../Lib/libft/libft.h"

//write -> Writes into an fd
//read -> Reads from an fd
//readdir -> Reads a directory
//closedir -> Closes a directory
//stat -> Gets the status of a file
//lstat -> Gets the status of a file (with symlinks)
//getpwuid -> Gets the password file entry for a user
//getgrgid -> Gets the group file entry for a group
//listxattr -> Lists the extended attributes of a file
//getxattr -> Gets the extended attribute of a file
//time -> Gets the time of a file
//ctime -> Converts a time to a string
//readlink -> Reads a symbolic link
//malloc -> Allocates memory
//free -> Frees memory
//perror -> Prints an error message
//strerror -> Converts an error number to a string
//exit -> Exits the program

/*
    * Project fundamental make a ls with -l (long format) -R (recursive) -a (all) -r (reverse) and -t (sort by time)
*/

#define print(x) write(1, x, ft_strlen(x));

typedef struct dirent Dirent;


typedef struct FileMetadata FileMetadata;

struct File{
    char *folderName;
    char *name;
    struct stat data;
    bool isdir;



    struct File *childs;
    struct File *next; // That will be used if the args are more than one;
    struct File *prev;
};

typedef struct File File;


File *Load(char *, bool, int);
void sortFilesByMTime(File *head);

void free_file(File *file);


    
#endif
