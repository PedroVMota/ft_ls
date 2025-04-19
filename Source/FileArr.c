#include "../Header/LsProgram.h"
#include "../Header/File.h"


FileArr *last_pos(FileArr *cur){
    if(!cur)
        return NULL;
    while(cur->next)
        cur = cur->next;
    return cur;
}

FileArr *new_file(char *root, int recursive){
    File *file = Load(root, recursive);
    if(!file)
        return NULL;
    FileArr *node = malloc(sizeof(FileArr *));
    if(!node){
        return NULL;
    }
    node->file = file;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
FileArr *add_file(FileArr **arr, FileArr *node){
    if(!arr || *arr)
        return node;
    FileArr *last = last_pos(*arr);
    if(!last){
        *arr = node;
        return *arr;
    }
    last->next = node;
    node->prev = last->next;
    return node;
}

size_t lst_size(FileArr *cur){
    size_t len = 0;
    while(cur){
        len++;
        cur = cur->next;
    }
    return len;
}
