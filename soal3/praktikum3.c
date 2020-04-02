#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>

// void* categorize_it(void* argv){
//     char* folder = (char*)argv;
//     if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)){
//         printf("success\n");
//     } else {
//         exit(EXIT_FAILURE);
//     }
// }

int is_ext(char fileext[],int len_ext,char* dird_name,int len_file) {
    int count = 0;
    for (int i = 1; i <= len_ext; i++){
        if(fileext[len_ext-i]==dird_name[len_file-i]){
            count++;
        }
    }
    // printf("len -> %d count -> %d\n",len_ext,count);
    if(len_ext==count){
        return 1;
    } else {
        return 0;
    }
}

int main(int argc,char* argv[]){
    int f_or_d = 0;
    char folder[100];
    int len_ext,len_file;
    char fileext[10];
    pthread_t *threads; 
	threads = (pthread_t*)malloc(argc*sizeof(pthread_t));
    if(strcmp(argv[1],"-f")==0){
        f_or_d = 1;
    } else if(strcmp(argv[1],"-d")==0){
        f_or_d = 2;
    } else if(strcmp(argv[1],"*")==0){
        f_or_d = 3;
    }else {
        printf("Argument no found\n");
        exit(EXIT_FAILURE);
    }

    if(f_or_d==1){
        int count = -1;
        for (int i = 2; i < argc; i++){
            // printf("%s ",argv[i]);
            char *ext = strrchr(argv[i], '.');
            if (!ext) {
                sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/Unknown");
            } else {
                // printf("extension is %s\n", ext);
                sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/%s",ext + 1);
                // pthread_create(&threads[count++],NULL,&categorize_it,(void*)ext);
                sprintf(fileext,"%s",ext);
                len_ext = strlen(fileext);
                if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)==0){
                    printf("success\n");
                } else {
                    printf("Error %s\n",strerror(errno));
                    DIR *d;
                    struct dirent *dir;
                    d = opendir("./soal3");
                    if (d)
                    {
                        while ((dir = readdir(d)) != NULL){
                            char tmp[100];
                            strcpy(tmp,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                            strcpy(tmp,dir->d_name);
                            len_file = strlen(dir->d_name);
                            if (is_ext(fileext,len_ext,dir->d_name,len_file)){
                                printf("%s\n",dir->d_name);
                            }                                
                        }
                        closedir(d);
                    }
                    exit(EXIT_FAILURE);
                }
            }
        }
        printf("selesai\n");
        // for(int i=0;i<=count;i++){
        //     pthread_join(threads[i],NULL);
        // }
        
    }
    

    return 0;
}