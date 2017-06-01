#include "unix_fs.h"
#include <string.h>
#include <stdio.h>

extern void yyerror(const char *, ...);

char * get_root_dir(char * cur_dir){
	char * pattern = "FAC";
	char * pattern_pos = strstr(cur_dir, pattern);
	if(pattern_pos == NULL)
		yyerror("UnixFS: Wrong path. Please run the FAC-compiler from the project's root or a subdirectory of the root\n");
	int path_size = strlen(cur_dir)-strlen(pattern_pos)+strlen(pattern);
	char * root_dir = (char*) malloc(path_size*sizeof(char));
	memcpy(root_dir, &cur_dir[0], path_size-strlen(pattern));
	memcpy(&(root_dir[path_size-strlen(pattern)]), &pattern[0], strlen(pattern));
	root_dir[path_size] = '\0';
	return root_dir;
}