#include "../Header/LsProgram.h"
#include "../Header/File.h"


FileArr *last_pos(FileArr *cur){
    if(!cur)
        return NULL;
    while(cur->next)
        cur = cur->next;
    return cur;
}

FileArr *new_file(char *root, int recursive, int include_hidden, int sort_time){
    
    File *file = Load(root, recursive, include_hidden, sort_time);
    if(!file)
        return NULL;
    FileArr *node = malloc(sizeof(FileArr));
    if(!node){
        return NULL;
    }
    node->file = file;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
FileArr *add_file(FileArr **arr, FileArr *node){
    if(!arr || !(*arr))
        return node;
    FileArr *last = last_pos(*arr);
    if(!last){
        *arr = node;
        return *arr;
    }
    last->next = node;
    node->prev = last->next;
    return *arr;
}

size_t lst_size(FileArr *cur){
    size_t len = 0;
    while(cur){
        len++;
        cur = cur->next;
    }
    return len;
}


void free_file(File *file) {
    if (!file) return;
    File *child = file->childs;
    while (child) {
        File *next = child->next;
        free_file(child);
        child = next;
    }
    free(file->folderName);
    file->folderName = NULL;
    file->childs = NULL;
    file->next = NULL;
    file->prev = NULL;
    free(file);
}

