#include <myls.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#define UNUSED(x) (void)(x);

struct timefile {
    char *filename;
    int time_nsec;
    int time_sec;
};

int is_directory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

bool is_special_dir(char *s) {
    if (strlen(s) == 1 && s[0] == '.') {
        return true;
    }
    if (strlen(s) == 2 && s[0] == '.' && s[1] == '.') {
        return true;
    }
    return false;
}

char* my_strcat(char *a, char *b, char *c){
    int i = 0;
    int j = 0;
    while (a[i] != '\0'){
        c[i] = a[i];
        i++;
    }
    while (b[j] != '\0'){
        c[i] = b[j];
        i++;
        j++;
    }
    c[i] = '\0';
    return c;
}
char* my_strcat_R(char *a, char *b, char *c){
    int i = 0;
    int j = 0;
    while (a[i] != '\0'){
        c[i] = a[i];
        i++;
    }
    while (b[j] != '\0'){
        c[i] = b[j];
        i++;
        j++;
    }
    c[i] = '/';
    c[i + 1] = '\0';
    return c;
}

int last_modified_time(char *path, char *filename){
    UNUSED(filename);
    struct stat tm;
    stat(path, &tm);
    //printf("%s %ld", filename, tm.st_mtim.tv_sec);
    return tm.st_mtim.tv_sec;
}
int last_modified_time_nsec(char *path, char *filename){
    UNUSED(filename);
    struct stat tm;
    stat(path, &tm);
    return tm.st_mtim.tv_nsec;
}


void print_array(char*s[], int len){
    for (int j=0; j < len; j++){
        if (j == len-1){
            printf("%s", s[j]);
        }
        else{
            printf("%s  ", s[j]);
        }
    }
}

bool my_strcmp(char* a, char* b){
   // printf("%s %s\n", a, b);
    int i = 0;
    while (a[i] != '\0' || b[i] != '\0'){
        if (a[i] == b[i]){
            i++;
        }
        else if (a[i] > b[i]){
            return true;
        }
        else{
            return false;
        }
    }
    if (a[i] == '\0'){
        return false;
    }
    return false;
}

void alphabet_sort(char **s, int len){
    char *temp [60];
    for (int i=0; i <len; i++){
        for (int j = i+1; j < len; j++){
            if (my_strcmp(s[i], s[j]) == true){
                *temp = s[i];
                s[i] = s[j];
                s[j] = *temp;
            }
        }
    }

}
void time_sort(struct timefile* items, int len){
    struct timefile *temp = malloc(sizeof(int)*100);
    for (int i = 0; i<len; i++){
        for (int j = i+1; j<len; j++){
            if (items[i].time_sec < items[j].time_sec){
                *temp = items[i];
                items[i] = items[j];
                items[j] = *temp;
            }
            else if (items[i].time_sec == items[j].time_sec){
                if (items[i].time_nsec < items[j].time_nsec){
                    *temp = items[i];
                    items[i] = items[j];
                    items[j] = *temp;
                }
                else if (items[i].time_nsec == items[j].time_nsec){
                    if (my_strcmp(items[i].filename, items[j].filename) == true){
                        *temp = items[i];
                        items[i] = items[j];
                        items[j] = *temp;
                    }
                }
            }
            
        }
    }
    free(temp);
}

void print_struct(struct timefile* items, int len){
    for (int i = 0; i < len; i++){
        printf("%s %d %d\n", items[i].filename, items[i].time_sec, items[i].time_nsec);
    }
}

int read_file(bool a, char **s, DIR *dir) {
    struct dirent *file;
    int i = 0;
    while ((file = readdir(dir)) != NULL){
        if (a == false){
            if (file->d_name[0] != '.'){
                s[i] = file->d_name;
                i++;
            }
        }
        else{
            s[i] = file->d_name;
            i++;
        }
    }
    return i;
}
char** sort_files(bool t, char** s, char *path, int len) {
    if (t==true){
        struct timefile* time_files = malloc(sizeof(struct timefile) * 100);
        int i = 0;
        while (i < len){
            char *b = malloc(sizeof(char)*1000);
            time_files[i].filename = s[i];
            time_files[i].time_sec = last_modified_time(my_strcat(path, s[i], b), s[i]);
            time_files[i].time_nsec = last_modified_time_nsec(my_strcat(path, s[i], b), s[i]);
            //printf("%s %d", s[i], last_modified_time(my_strcat(path, s[i], b), s[i]));
            i++;
            free(b);
        }
        time_sort(time_files, len);
        int j = 0;
        while (j < len){
            s[j] = time_files[j].filename;
            j++;
        }
        //print_struct(time_files, len);
        free(time_files);
        return s;
    }
    else{
        alphabet_sort(s, len);
        return 0;
    }
}

void print_ls(char *path, bool a, bool R, bool t, int dir_num) {
    char **s = malloc(sizeof(char)*10000);
    DIR *dir = opendir(path);
    int len = read_file(a, s, dir);
    if (t==true){
        if (R == true) {
            for (unsigned int j = 0; j < strlen(path)-1; j++){
                printf("%c", path[j]);
            }
            printf(":\n");
        }
        if (dir_num > 1){
            for (unsigned int j = 0; j < strlen(path); j++){
                printf("%c", path[j]);
            }
            printf(":\n");
        }
        print_array(sort_files(t, s, path, len), len);
    }
    else{
        sort_files(t, s, path, len);
        if (R == true || dir_num > 1) {
            for (unsigned int j = 0; j < strlen(path); j++){
                if (j == strlen(path)-1 && path[j] == '/'){
                    break;
                }
                printf("%c", path[j]);
            }
            printf(":\n");
        }
        print_array(s, len);
    }
    if (R == true) {
        for (int i = 0; i < len; i++) {
            char *newpath = malloc(sizeof(char) * 100);
            my_strcat_R(path, s[i], newpath);
            if (is_directory(newpath) > 0 && is_special_dir(s[i]) == false) {
                printf("\n \n");                
                print_ls(newpath, a, R, t, dir_num);
            }
            free(newpath);
        }
    }
    closedir(dir);
    free(s);
}
