#include "../Header/Core.h"

char *get_permission(struct stat data)
{
	char *permissions = malloc(sizeof(char) * 11);
	if(!permissions)
		return NULL;
	permissions[0] = S_ISDIR(data.st_mode) ? 'd' : '-';
	permissions[1] = (data.st_mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (data.st_mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (data.st_mode & S_IXUSR) ? 'x' : '-';
	permissions[4] = (data.st_mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (data.st_mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (data.st_mode & S_IXGRP) ? 'x' : '-';
	permissions[7] = (data.st_mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (data.st_mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (data.st_mode & S_IXOTH) ? 'x' : '-';
	permissions[10] = ' ';
	permissions[11] = '\0';
	return permissions;
}

char *get_user(pid_t user) {
	struct passwd *pwd = getpwuid(user);
	if (!pwd)
		return NULL;
	char *username = ft_strdup(pwd->pw_name);
	if (!username)
		return NULL;
	return username;
}

char  *get_group(gid_t group) {
	struct group *grp = getgrgid(group);
	if (!grp)
		return NULL;
	char *groupName = ft_strdup(grp->gr_name);
	if (!groupName)
		return NULL; 
	return groupName;
}


char *get_size_hardlinks(struct stat data) {
	char buffer[21];
	nlink_t nlink = data.st_nlink;
	int i = 0;

	if (nlink == 0)
		buffer[i++] = '0';
	else {
		while (nlink > 0) {
			buffer[i++] = '0' + (nlink % 10);
			nlink /= 10;
		}
	}
	char *hardlink = malloc(i + 1);
	if (!hardlink)
		return NULL;
	for (int j = 0; j < i; j++)
		hardlink[j] = buffer[i - j - 1];
	hardlink[i] = ' ';
	hardlink[i + 1] = '\0';
	return hardlink;
}