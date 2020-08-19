#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void print_ls(char *path, bool a, bool R, bool t, int dir_num);

bool my_strcmp(char* a, char* b);

void alphabet_sort(char **s, int len);

int is_directory(const char *path);

void print_array(char*s[], int len);