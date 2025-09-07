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
#define MAX_COMMAND_SIZE 16



#define RED(string) "\033[0;31m"string"\033[0m" 
#define GREEN(string) "\033[0;32m"string"\033[0m"
#define YELLOW(string) "\033[0;33m"string"\033[0m"



void loop();
char *read_line();
void create_command_from_line(char *line, char *command[]);
int run(char *command[]);


int nb_commands();

int execute_built_in_function(const char *cmd);




int is_visible(const struct dirent *file);
char* get_user(uid_t uid);
char* get_grp(uid_t gid);
unsigned char get_type(unsigned long mode);
void fmt_rights(mode_t mode, char* rights);
void print_line(char mode, char* rights, char* user, char* group, long long size, char* time, char* filename);
int only_me(const struct dirent *file);
int get_name_from_ino(char* dir, ino_t inode, char* dirname);






int ls(char* directory);
int ls_a(char* directory);
int ls_l(char* directory);
int cat(const char *filename);
int cp(const char *src, const char *dst);
int help();
