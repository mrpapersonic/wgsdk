#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "dirtools.h"

int dirtools_directory_exists(char* path) {
	DWORD attrib = GetFileAttributesA(path);
	return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

int dirtools_create_directory(char* path) {
	char* alltoks = calloc(strlen(path), sizeof(char)), *tok;

	for (tok = strtok(path, "\\"); tok != NULL; tok = strtok(NULL, "\\")) {
		strcat(alltoks, tok);
		if (dirtools_directory_exists(path)) {
			if (!CreateDirectoryA(alltoks, NULL)) {
				if (GetLastError() == ERROR_PATH_NOT_FOUND) {
					/* ERROR_PATH_NOT_FOUND should NOT happen here */
					return 1;
				}
			}
		}
	}

	free(alltoks);

	return 0;
}

char* dirtools_concat_paths(char* a, char* b) {
	if (a[0] == '\0' || b[0] == '\0')
		return NULL;
	char* out = calloc((strlen(a) + strlen(b) + 2), sizeof(char));
	if (out == NULL)
		return out;
	strcpy(out, a);
	if (a[strlen(a)] != '\\' && b[0] != '\\')
		strcat(out, "\\");
	strcat(out, b);
	return out;
}
