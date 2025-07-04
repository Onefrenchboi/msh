#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define PATH_SIZE 2048;
#define MAX_LINE_SIZE 1024







int ls(char* directory);
int ls_a(char* directory);
int ls_l(char* directory);


int is_visible(const struct dirent *file);
char* get_user(uid_t uid);
char* get_grp(uid_t gid);
unsigned char get_type(unsigned long mode);
void fmt_rights(mode_t mode, char* rights);
void print_line(char mode, char* rights, char* user, char* group, long long size, char* time, char* filename);
int only_me(const struct dirent *file);
int get_name_from_ino(char* dir, ino_t inode, char* dirname);



int cat(const char *filename);
int cp(const char *src, const char *dst);