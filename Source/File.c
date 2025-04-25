#include "../Header/File.h"

void swapFileContent(File *a, File *b)
{
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

void sortFilesByMTime(File *head)
{
	if (!head)
		return;
	bool swapped;
	File *ptr;
	do
	{
		swapped = false;
		ptr = head;
		while (ptr->next)
		{
			if (ptr->data.st_mtime < ptr->next->data.st_mtime)
			{
				swapFileContent(ptr, ptr->next);
				swapped = true;
			}
			ptr = ptr->next;
		}
	} while (swapped);
	// After sorting this level, sort each child's list
	ptr = head;
	while (ptr)
	{
		if (ptr->isdir && ptr->childs)
		{
			sortFilesByMTime(ptr->childs);
		}
		ptr = ptr->next;
	}
}

FileMetadata *LoadMeta(char *root, struct dirent *entry)
{
	struct stat data;
	(void)entry;
	if (stat(root, &data) == -1)
		return NULL;
	FileMetadata *ptr = (FileMetadata *)malloc(sizeof(FileMetadata *));
	if (!ptr)
		return NULL;
	return NULL;
}
