#include "../Header/Core.h"
#include <stdint.h>



void debug_print_file(File *file, int indent) {
    if (!file)
        return;
        
    // Print indentation for clarity.
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    // Print the file/folder name; append '/' if it is a directory.
    printf("%s%s\n", file->folderName, file->isdir ? "/" : "");
    
    // If the file is a directory, recursively print its child files.
    if (file->isdir) {
        File *child = file->childs;
        while (child) {
            debug_print_file(child, indent + 1);
            child = child->next;
        }
    }
}

// Function to print all files in the FileArr array.
void debug_print_filearr(FileArr *arr) {
    FileArr *current = arr;
    while (current) {
        debug_print_file(current->file, 0);
        current = current->next;
    }
}


flags_t ls_output(LsProgram *program, char *path)
{
    program->arr = add_file(&program->arr, new_file(path, program->flags_value & FLAG_RECURSIVE));
    return 0;
}

void ls_execute(LsProgram *program)
{
    if (program == NULL || program->path == NULL) {
        print("program is NULL\n");
        return;
    }
    for (int index = 0; program->path[index] != NULL; index++) {
        program->pathindex = index;
        if(program->path[index][0] == '\0')
            return;
        ls_output(program,program->path[index]); 
    }
    if(program->flags_value & FLAG_TIME){
        FileArr *cur = program->arr;
        while(cur){
            sortFilesByMTime(cur->file);
            cur = cur->next;
        }
    }

    debug_print_filearr(program->arr);
}


unsigned int parse_flags(int argc, char *argv[], char ***path) {
    unsigned int flags = 0;
    
    int isfolder = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && isfolder != 1) {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'l': flags |= FLAG_LONG; break;
                    case 'R': flags |= FLAG_RECURSIVE; break;
                    case 'a': flags |= FLAG_ALL; break;
                    case 'r': flags |= FLAG_REVERSE; break;
                    case 't': flags |= FLAG_TIME; break;
                    #ifdef BONUS_H
                    case 'u': flags |= FLAG_ACCESS_TIME; break;
                    case 'f': flags |= FLAG_DO_NOT_SORT; break;
                    case 'g': flags |= FLAG_OMIT_OWNER; break;
                    case 'd': flags |= FLAG_DIRECTORY_NAMES; break;
                    #endif // BONUS_H
                    default:
                        print("Unknown flag: ");
                        print(argv[i]);
                        print("\n");
                        return FLAG_ERROR_PARSING;
                }
            }
        }
        else if (argv[i][0] != '-') {
            isfolder = 1;
            *path = append_child((*path), argv[i]);
            if(!path)
            {
                print("Failed to allocate memory for path\n");
                return FLAG_ERROR_MALLOC;
            }
        }
        else {
            print("Flags must be before paths\n");
            return FLAG_ERROR_PARSING;
        }
        
    }
    return flags;
}

LsProgram *ls_init(int ac, char **av){
    LsProgram *ls = malloc(sizeof(LsProgram));
    if (ls == NULL) {
        perror("Failed to allocate memory for LsProgram");
        return NULL;
    }
    ls->path = NULL;
    ls->flags_value = 0;
    ls->pathindex = 0;
    ls->arr = NULL;
    ls->flags_value = parse_flags(ac, av, &ls->path);
    if (ls->flags_value < 0) {
        free(ls);
        return NULL;
    }
    return ls;
}

void ls_destroy(LsProgram **ls) {
    if (ls == NULL || *ls == NULL) {
        return;
    }
    if ((*ls)->path != NULL) {
        for (size_t i = 0; (*ls)->path[i] != NULL; i++) {
            if((*ls)->path[i] != NULL)
                free((*ls)->path[i]);
        }
        free((*ls)->path);
    }
    
    free(*ls);
    *ls = NULL;
}


