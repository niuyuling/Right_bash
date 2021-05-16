#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include "getopt.h"

#define CACHE_SIZE 270

void showErrorText(DWORD error_num);
void add_Right();
void del_Right();
void add_boot_ssh(char* optarg);
void del_boot_ssh(char* optarg);
void help(char** argv);

