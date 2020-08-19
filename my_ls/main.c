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

int main(int argc, char* argv[]){
    UNUSED(argc);
    bool a = false, t = false, R = false;
    char* path = "./";
    int flag = 0;
    int j = 1;

    while (argv[j] != NULL){
        if (argv[j] != NULL && argv[j][0] == '-') {
            for (unsigned int i = 0; i < strlen(argv[j]); i++) {
                if (argv[j][i] == 't') {
                    t = true;
                }
                if (argv[j][i] == 'R') {
                    R = true;
                }
                if (argv[j][i]== 'a') {
                    a = true;
                }
            }
        }
        j++;
    }
    
    int dir_num = 0;
    int file_num = 0;
    char** dirs = malloc(sizeof(char)*100);
    char** files = malloc(sizeof(char)*100);
    int k = 1;

    if (argv[k] != NULL){
        while(argv[k] != NULL){
            if (argv[k][0] != '-' && is_directory(argv[k]) == 0) {
                files[file_num] = argv[k];
                file_num++;
            }
            else if (is_directory(argv[k]) > 0){
                dirs[dir_num] = argv[k];
                dir_num++;
            }
            k++;
        }
        if (file_num > 0 || dir_num >= 1){
            alphabet_sort(dirs, dir_num);
            alphabet_sort(files, file_num);
            print_array(files, file_num);
            if (file_num > 0){
                printf("\n \n");
            }
            j = 0;
            while (j < dir_num){
                print_ls(dirs[j], a, R, t, dir_num);
                if (j != dir_num-1){
                    printf("\n \n");
                }
                j++;
            }
            flag = 1;
        }
    }
    if (flag == 0){
        print_ls(path, a, R, t, dir_num);
    }
    free(dirs);
    free(files);
    return 0;
}
