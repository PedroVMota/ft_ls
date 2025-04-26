#include "../Header/Core.h"
#include <stdint.h>

char *get_permission(struct stat data);
char *get_user(pid_t user);
char  *get_group(gid_t group);
char *get_size_hardlinks(struct stat data);
char *get_modification_time(struct stat data);



uint16_t FilePrintLong(struct stat data)
{
  char *perm = get_permission(data);
  if(!perm)
    return FLAG_ERROR_MALLOC;
  write(1, perm, ft_strlen(perm));
  char *nLinks = get_size_hardlinks(data);
  if(!nLinks)
    return FLAG_ERROR_MALLOC;
  write(1, nLinks, ft_strlen(nLinks));
  char *user = get_user(data.st_uid);
  char *group = get_group(data.st_gid);
  if(!user | !group)
    return FLAG_ERROR_MALLOC;
  write(1, user, ft_strlen(user));
  write(1, "\t", 1);
  write(1, group, ft_strlen(group));
  write(1, "\t", 1);
  char *size = ft_itoa(data.st_size);
  write(1, size, ft_strlen(size));
  write(1, "\t", 1);
  char *time = get_modification_time(data);
  write(1, time, ft_strlen(time) - 1);
  write(1, " ", 1);
  return 0;
}

void print_dir_content(File *file, int long_print)
{
  File *child = file->childs;
  while (child)
  {
    if (!long_print)
    {
      print(child->folderName);
      if(!isatty(fileno(stdout)) && child->next)
        print("\n")
      else if(child->next){
        print("\t")
      }
    }
    else
    {

      FilePrintLong(child->data);
      print(child->folderName);
      if(child->next)
        print("\n");
    }
    child = child->next;
  }
}

void printoutput(LsProgram *ls, int recursive)
{
  (void)recursive;
  size_t size = lst_size(ls->arr);
  FileArr *arr = ls->arr;

  while (arr)
  {
    if (size > 1)
    {
      if(arr->prev != NULL)
        print("\n\n");
      print(arr->file->folderName);
      print(":\n");
    }
    print_dir_content(arr->file, ls->flags_value & FLAG_LONG);
    arr = arr->next;
  }
}

flags_t ls_output(LsProgram *program, char *path)
{
  program->arr = add_file(&program->arr, new_file(path, program->flags_value & FLAG_RECURSIVE, (program->flags_value & FLAG_ALL), (program->flags_value & FLAG_TIME)));
  return 0;
}

void ls_execute(LsProgram *program)
{
  if (program == NULL || program->path == NULL)
  {
    print("program is NULL\n");
    return;
  }
  for (int index = 0; program->path[index] != NULL; index++)
  {
    program->pathindex = index;
    if (program->path[index][0] == '\0')
      return;
    ls_output(program, program->path[index]);
  }
  if (program->flags_value & FLAG_TIME)
  {

    FileArr *cur = program->arr;
    while (cur)
    {
      sortFilesByMTime(cur->file);
      cur = cur->next;
    }
  }
  printoutput(program, program->flags_value & FLAG_RECURSIVE);
  print("\n");
}

unsigned int parse_flags(int argc, char *argv[], char ***path)
{
  unsigned int flags = 0;

  int isfolder = 0;
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-' && isfolder != 1)
    {
      for (int j = 1; argv[i][j] != '\0'; j++)
      {
        switch (argv[i][j])
        {
        case 'l':
          flags |= FLAG_LONG;
          break;
        case 'R':
          flags |= FLAG_RECURSIVE;
          break;
        case 'a':
          flags |= FLAG_ALL;
          break;
        case 'r':
          flags |= FLAG_REVERSE;
          break;
        case 't':
          flags |= FLAG_TIME;
          break;
#ifdef BONUS_H
        case 'u':
          flags |= FLAG_ACCESS_TIME;
          break;
        case 'f':
          flags |= FLAG_DO_NOT_SORT;
          break;
        case 'g':
          flags |= FLAG_OMIT_OWNER;
          break;
        case 'd':
          flags |= FLAG_DIRECTORY_NAMES;
          break;
#endif // BONUS_H
        default:
          print("Unknown flag: ");
          print(argv[i]);
          print("\n");
          return FLAG_ERROR_PARSING;
        }
      }
    }
    else if (argv[i][0] != '-')
    {
      isfolder = 1;
      *path = ft_strconcat((*path), argv[i]);
      if (!path)
      {
        print("Failed to allocate memory for path\n");
        return FLAG_ERROR_MALLOC;
      }
    }
    else
    {
      print("Flags must be before paths\n");
      return FLAG_ERROR_PARSING;
    }
  }
  return flags;
}

LsProgram *ls_init(int ac, char **av)
{
  LsProgram *ls = malloc(sizeof(LsProgram));
  if (ls == NULL)
  {
    perror("Failed to allocate memory for LsProgram");
    return NULL;
  }
  ls->path = NULL;
  ls->flags_value = 0;
  ls->pathindex = 0;
  ls->arr = NULL;
  ls->flags_value = parse_flags(ac, av, &ls->path);
  if (ls->flags_value < 0)
  {
    free(ls);
    return NULL;
  }
  return ls;
}

void ls_destroy(LsProgram **ls)
{
  if (ls == NULL || *ls == NULL)
  {
    return;
  }
  if ((*ls)->path != NULL)
  {
    for (size_t i = 0; (*ls)->path[i] != NULL; i++)
    {
      if ((*ls)->path[i] != NULL)
        free((*ls)->path[i]);
    }
    free((*ls)->path);
  }

  free_file_arr((*ls)->arr);

  free(*ls);
  *ls = NULL;
}


void free_file_arr(FileArr *arr) {
  while (arr) {
      FileArr *next = arr->next;

      // Free the associated File structure
      if (arr->file) {
          free_file(arr->file);
      }

      // Free the FileArr node
      free(arr);

      arr = next;
  }
}

void clean_ls_files(LsProgram *program) {
  if (!program || !program->arr) {
      return;
  }

  // Free all FileArr and associated File structures
  free_file_arr(program->arr);

  // Set the array pointer to NULL to avoid dangling pointers
  program->arr = NULL;
}
