#include "../Header/File.h"
#include "../Header/StringUtils.h"





void swapFileContent(File *a, File *b) {
    char *tempFolderName = a->folderName;
    struct stat tempData = a->data;
    bool tempIsDir = a->isdir;
    File *tempChilds = a->childs;

    a->folderName = b->folderName;
    a->data = b->data;
    a->isdir = b->isdir;
    a->childs = b->childs;

    b->folderName = tempFolderName;
    b->data = tempData;
    b->isdir = tempIsDir;
    b->childs = tempChilds;
}

void sortFilesByMTime(File *head) {
    printf("Sorting by Modification time\n");
    if (!head) return;

    bool swapped;
    File *ptr;
    do {
        swapped = false;
        ptr = head;

        while (ptr->next) {
            if (ptr->data.st_mtime < ptr->next->data.st_mtime) {
                swapFileContent(ptr, ptr->next);
                swapped = true;
            }
            ptr = ptr->next;
        }
    } while (swapped);

    // After sorting this level, sort each child's list
    ptr = head;
    while (ptr) {
        if (ptr->isdir && ptr->childs) {
            sortFilesByMTime(ptr->childs);
        }
        ptr = ptr->next;
    }
}



static inline int LoadDir(char *root, DIR **dir) {
  if ((*dir))
    return 0;
  *dir = opendir(root);
  if (!*dir)
    return -1;
  return 0;
}

FileMetadata *LoadMeta(char *root, struct dirent *entry) {
  struct stat data;
  if (stat(root, &data) == -1)
    return NULL;
  FileMetadata *ptr = (FileMetadata *)malloc(sizeof(FileMetadata *));
  if (!ptr)
    return NULL;

  return NULL;
}

File *Load(char *root, bool recursive) {
  File *folder = malloc(sizeof(File));
  if (!folder)
    return NULL;
  folder->folderName = duplicate_string(root);
  folder->isdir = true;
  folder->childs = NULL;
  folder->next = NULL;
  folder->prev = NULL;
  DIR *dir = NULL;
  if (LoadDir(root, &dir) == -1) {
    free(folder);
    return NULL;
  }
  struct dirent *entry;
  while ((entry = readdir(dir))) {
    // skip current and parent directories
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
    // Construct full path: root + "/" + entry name
    char *childPath = concat_strings(3, root, "/", entry->d_name);
    File *childFile = NULL;
    if (recursive && entry->d_type == DT_DIR) {
      childFile = Load(childPath, recursive);
    } else {
      childFile = malloc(sizeof(File));
      if (childFile) {
        childFile->folderName = duplicate_string(childPath);
        childFile->isdir = (entry->d_type == DT_DIR);
        if (stat(childPath, &childFile->data))
          childFile->childs = NULL;
        childFile->next = NULL;
        childFile->prev = NULL;
      }
    }
    // Append childFile to folder->childs linked list
    if (childFile) {
      if (folder->childs == NULL) {
        folder->childs = childFile;
      } else {
        File *cur = folder->childs;
        while (cur->next)
          cur = cur->next;
        cur->next = childFile;
        childFile->prev = cur;
      }
    }
    free(childPath);
  }
  closedir(dir);
  // Return the created File structure
  return folder;
}
