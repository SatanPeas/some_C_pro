#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>

int my_remove(char *, char*, char *);
int my_removeSubFile(char *, char *);

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("%s\n", "please input path and name in args!");
		return 0;
	}

	my_remove(argv[1], NULL, argv[2]);
}

int my_remove(char *path, char *pathname, char *name)
{
	long fileHandle;
	struct _finddata_t data;

	if (chdir(path) != 0) {
		return 0;
	}

	if (NULL != pathname && strcmp(pathname, name) == 0) {
		my_removeSubFile(path, pathname);
		return 0;
	}

	if ((fileHandle = _findfirst("*.*", &data)) == -1l) {
		return 1;
	}

	char *temp = (char*) malloc(512);
	if (strcmp(data.name, ".") != 0 && strcmp(data.name, "..") != 0) {
		strcpy(temp, path);
		strcat(temp, "\\");
		strcat(temp, data.name);
		my_remove(temp, data.name, name);
	}

	while (_findnext(fileHandle, &data) != -1l) {
		if (strcmp(data.name, ".") != 0 && strcmp(data.name, "..") != 0) {
			strcpy(temp, path);
			strcat(temp, "\\");
			strcat(temp, data.name);
			my_remove(temp, data.name, name);
		}
	}

	_findclose(fileHandle);
	free(temp);
	return 0;
}

int my_removeSubFile(char *path, char *pathname)
{

	if (chdir(path) != 0) {
		if (unlink(path) == 0)
			printf("remove %s success!\n", path);
		return 0;
	}

	long fileHandle;
	struct _finddata_t data;

	if ((fileHandle = _findfirst("*.*", &data)) == -1l) {
		return 1;
	}

	char *temp = (char*) malloc(512);
	if (strcmp(data.name, ".") != 0 && strcmp(data.name, "..") != 0) {
		strcpy(temp, path);
		strcat(temp, "\\");
		strcat(temp, data.name);
		my_removeSubFile(temp, data.name);
	}
	while (_findnext(fileHandle, &data) != -1l) {
		if (strcmp(data.name, ".") != 0 && strcmp(data.name, "..") != 0) {
			strcpy(temp, path);
			strcat(temp, "\\");
			strcat(temp, data.name);
			my_removeSubFile(temp, data.name);
		}
	}

	chdir(strcat(path, "\\.."));
	if (_rmdir(pathname) == 0) {
		printf("remove %s success!\n", path);
	}

	_findclose(fileHandle);
	free(temp);
	return 0;
}                                                                                                         