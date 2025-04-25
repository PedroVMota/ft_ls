#include "../Header/File.h"


static inline File *init(){
    File *folder = malloc(sizeof(File));
	if (!folder)
		return NULL;
	ft_bzero(folder, sizeof(File));
    return folder;
}

static inline int LoadDir(char *root, DIR **dir)
{
	if ((*dir))
		return 0;
	*dir = opendir(root);
	if (!*dir)
		return -1;
	return 0;
}

static inline int use_hiden(int use, struct dirent *entry){
    return (!use && (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, "..") || (char)entry->d_name[0] == '.'));
}


static void addchilds(File *folder, File *child) {
    // reset the new node’s pointers
    child->next = NULL;
    child->prev = NULL;

    // 1) empty list → child becomes first
    if (!folder->childs) {
        folder->childs = child;
        return;
    }

    File *cur = folder->childs;

    // 2) insert before head if it sorts earlier
    if (ft_strcmp(child->folderName, cur->folderName) < 0) {
        child->next        = cur;
        cur->prev          = child;
        folder->childs     = child;
        return;
    }

    // 3) otherwise walk until the next node would sort >= new child
    while (cur->next
        && ft_strcmp(cur->next->folderName, child->folderName) < 0)
    {
        cur = cur->next;
    }

    // 4) splice child in between cur and cur->next
    child->next = cur->next;
    if (cur->next)
        cur->next->prev = child;
    cur->next  = child;
    child->prev = cur;
}

static void readalldir(File *file, DIR *dir, int usehiddens, char *parentpath, int recursive){
    struct dirent *entry;
    while((entry = readdir(dir))){
        if(use_hiden(usehiddens, entry))
            continue;
        char *cpath = NULL;
        if (!strcmp(parentpath, "/"))
            cpath = ft_strjoin_va(2, "/", entry->d_name);
        else
            cpath = ft_strjoin_va(3, parentpath, "/", entry->d_name);
        if(!cpath)
            continue;
        File *folderchild = NULL;
        if (recursive && entry->d_type == DT_DIR)
            folderchild = Load(cpath, recursive, usehiddens);
        else{
            folderchild = ft_calloc(1, sizeof(File));
            if(folderchild){
                folderchild->folderName = ft_strdup(entry->d_name);
				folderchild->isdir = (entry->d_type == DT_DIR);
				ft_bzero(&folderchild->data, sizeof(folderchild->data));
				if (stat(cpath, &folderchild->data) == -1)
					print(strerror(errno));
				folderchild->next = NULL;
				folderchild->prev = NULL;
            }
        }
        addchilds(file, folderchild);
        free(cpath);
    }
}

File *Load(char *root, bool recursive, int include_hiden)
{
	File *folder = init();
	if (!folder)
		return NULL;
	folder->folderName = ft_strdup(root);
	if (!folder->folderName) {
			free(folder);
			return NULL;
	}
	DIR *dir = NULL;
	if (LoadDir(root, &dir) == -1)
	{
		free(folder);
		return NULL;
	}
    
    readalldir(folder, dir, include_hiden, root, recursive);
	closedir(dir);
	// Return the created File structure
	return folder;
}