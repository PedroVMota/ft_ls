#include "../Header/Core.h"





int main(int argc, char *argv[])
{
    #ifdef DEBUG
    print("======= Be aware this is the development version! ========\n");
    #endif
    LsProgram *program = ls_init(argc, argv);
    if (program == NULL) {
        print("failed to initialize lsprogram\n");
        return 1;
    }

    if(program->path == NULL)
    {
        program->path = malloc(2 * sizeof(char *));
        program->path[0] = malloc(2 * sizeof(char));
        if (program->path[0] == NULL) {
            print("Failed to allocate memory for path\n");
            ls_destroy(&program);
            return 1;
        }
        program->path[0][0] = '.';
        program->path[0][1] = '\0';
        program->path[1] = NULL;

    }
    ls_execute(program);




    ls_destroy(&program);
}
