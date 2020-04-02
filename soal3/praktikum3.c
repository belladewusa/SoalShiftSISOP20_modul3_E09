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
#include <ctype.h>

int is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

int is_ext(char fileext[],int len_ext,char* dird_name,int len_file) {
    int count = 0;
    // printf("masuk\n");
    for (int i = 1; i <= len_ext; i++){
        if(len_ext-i==-1||len_file-i==-1){
            break;
        }
        if(fileext[len_ext-i]==dird_name[len_file-i]||toupper(fileext[len_ext-i])==dird_name[len_file-i]){
            // printf("%c->%c\n",fileext[len_ext-i],dird_name[len_file-i]);    
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

void* dash_f(void* argv){
    char *ext = (char*)argv;
    char folder[100];
    memset(folder,0,100*sizeof(char));
    int len_ext,len_file;
    char fileext[10];
    if (!ext) {
        sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/Unknown");
        if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)==0){
            printf("success\n");
            DIR *d;
            struct dirent *dir;
            d = opendir("./soal3");
            if (d)
            {
                while ((dir = readdir(d)) != NULL){
                    char from[100],to[100];
                    memset(from,0,150*sizeof(char));
                    memset(to,0,150*sizeof(char));
                    strcpy(from,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                    sprintf(to,"%s/",folder);
                    char *dr = strrchr(dir->d_name, '.');
                    if(!dr){
                        strcat(from,dir->d_name);
                        strcat(to,dir->d_name);
                        printf("%s\n",dir->d_name);
                        if(is_dir(from)){
                            continue;
                        } else {
                            rename(from,to);
                        }
                    }                                
                }
                closedir(d);
            }
            // exit(EXIT_SUCCESS);
        } else {
            printf("Error %s\n",strerror(errno));
        }
    } else {
        // printf("extension is %s\n", ext);
        sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/%s",ext + 1);
        // pthread_create(&threads[count++],NULL,&categorize_it,(void*)ext);
        sprintf(fileext,"%s",ext);
        len_ext = strlen(fileext);
        if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)==0){
            printf("success\n");
            DIR *d;
            struct dirent *dir;
            d = opendir("./soal3");
            if (d)
            {
                while ((dir = readdir(d)) != NULL){
                    // printf("%s\n",dir->d_name);
                    char tmp[150],new[150];
                    memset(tmp,0,150*sizeof(char));
                    memset(new,0,150*sizeof(char));
                    strcpy(tmp,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                    sprintf(new,"%s/",folder);
                    len_file = strlen(dir->d_name);
                    if (is_ext(fileext,len_ext,dir->d_name,len_file)){
                        // printf("di if\n");
                        strcat(tmp,dir->d_name);
                        strcat(new,dir->d_name);
                        if(is_dir(tmp)){
                            continue;
                        } else {
                            rename(tmp,new);
                        }
                    }                                
                }
                closedir(d);
            }
            // exit(EXIT_SUCCESS);
        } else {
            printf("Error %s\n",strerror(errno));
        }
    }
}

int main(int argc,char* argv[]){
    int f_or_d = 0;
    pthread_t *threads; 
	threads = (pthread_t*)malloc(100*sizeof(pthread_t));
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
            // count++;
            // printf("%s ",argv[i]);
            char *ext = strrchr(argv[i], '.');
            if(!ext){
                pthread_create(&threads[++count],NULL,dash_f,NULL);
            } else {
                pthread_create(&threads[++count],NULL,dash_f,(void*)ext);
            }
            // dash_f((void*)ext);
        }
        printf("selesai\n");
        for(int i=0;i<=count;i++){
            pthread_join(threads[i],NULL);
        }
        
    } else if (f_or_d==3){
        int count = -1;
        DIR *d;
        struct dirent *dir;
        d = opendir("./soal3");
        if (d)
        {
            while ((dir = readdir(d)) != NULL){
                char format[150]="\0";
                memset(format,0,150*sizeof(char));
                strcpy(format,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                char *ext = strrchr(dir->d_name, '.');
                strcat(format,dir->d_name);
                if(is_dir(format)){
                    printf("%s ==============directory=============\n",dir->d_name);
                } else {
                    if(!ext){
                        printf("%s - kosong\n",dir->d_name);
                        pthread_create(&threads[++count],NULL,dash_f,NULL);
                    } else {
                        printf("%s - %s\n",dir->d_name,ext);
                        pthread_create(&threads[++count],NULL,dash_f,(void*)ext);
                    }
                }
                sleep(2);
            } closedir(d);
        }
        for(int i=0;i<=count;i++){
            pthread_join(threads[i],NULL);
        }
    }
    

    return 0;
}