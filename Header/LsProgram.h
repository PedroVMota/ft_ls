#ifndef LSPROGRAM_H
#define LSPROGRAM_H


#include <stdint.h>
#include "../Lib/libft/libft.h"
#include "./File.h"

    #ifndef LS_FLAGS_EXECUTION_H
        #define FLAG_LONG           0b0000000000000001  // 0x0001 -l int value: 1
        #define FLAG_RECURSIVE      0b0000000000000010  // 0x0002 -R int value: 2
        #define FLAG_ALL            0b0000000000000100  // 0x0004 -a int value: 4
        #define FLAG_REVERSE        0b0000000000001000  // 0x0008 -r int value: 8
        #define FLAG_TIME           0b0000000000010000  // 0x0010 -t int value: 16

        #ifndef BONUS_H
            #define FLAG_ACCESS_TIME    0b0000000000100000  // 0x0020 -u int value: 32
            #define FLAG_DO_NOT_SORT    0b0000000001000000  // 0x0040 -f int value: 64
            #define FLAG_OMIT_OWNER     0b0000000010000000  // 0x0080 -g int value: 128
            #define FLAG_DIRECTORY_NAMES 0b0000000100000000 // 0x0100 -d int value: 256
        #endif // BONUS_H

        #define FLAG_ERROR_PARSING  -0b0000000000000001 // 0xFFFF -1 int value: -1
        #define FLAG_ERROR_MALLOC   -0b0000000000000010 // 0xFFFE -2 int value: -2
        #define FLAG_ERROR_NULL     -0b0000000000000100 // 0xFFFD -3 int value: -3
        #define FLAG_ERROR_LONG_DETAILS -0b0000000000001000 // 0xFFFC -4 int value: -4
    #endif

typedef uint16_t flags_t;


struct FileArr{
    File *file;
    struct FileArr *next;
    struct FileArr *prev;
};

typedef struct FileArr FileArr;

/******** FileArr FUNCTIONS ********/
FileArr *new_file(char *, int, int);
FileArr *add_file(FileArr **, FileArr *);
FileArr *last_pos(FileArr *);
size_t lst_size(FileArr *);
/************************************/




typedef struct s_lsprogram{
    int pathindex;
    char **path;
    flags_t flags_value;

    FileArr *arr;
} LsProgram;


    
/******** LS FUNCTIONS ********/
LsProgram *ls_init(int ac, char **av);
void ls_execute(LsProgram *ls);
void ls_destroy(LsProgram **ls);
void clean_ls_files(LsProgram *program);
void free_file_arr(FileArr *arr);
/******************************/

#endif
