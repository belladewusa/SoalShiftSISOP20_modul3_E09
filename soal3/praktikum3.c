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

int f_or_d = 0;
char dash_d[200];

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
        if(tolower(fileext[len_ext-i])==dird_name[len_file-i]||toupper(fileext[len_ext-i])==dird_name[len_file-i]){
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
        if(f_or_d==2){
            sprintf(folder,"%sUnknown/",dash_d);
        } else {
            sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/Unknown/");
        }
        if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)==0){
            printf("success\n");
            DIR *d;
            struct dirent *dir;
            if(f_or_d==2){
                d = opendir(dash_d);
            } else {
                d = opendir("./soal3/");
            }
            if (d)
            {
                while ((dir = readdir(d)) != NULL){
                    char from[300],to[300];
                    memset(from,0,300*sizeof(char));
                    memset(to,0,300*sizeof(char));
                    if(f_or_d==2){
                        strcpy(from,dash_d);
                    } else {
                        strcpy(from,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                    }
                    sprintf(to,"%s",folder);
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
        char mkd[10] = "\0";
        for (int i = 0; i < strlen(ext); i++){
            mkd[i] = tolower(ext[i]);
        }
        if(f_or_d==2){
            sprintf(folder,"%s%s/",dash_d,mkd+1);
            // printf("%s\n",folder);
        } else {
            sprintf(folder,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/%s/",mkd + 1);
        }
        sprintf(fileext,"%s",ext);
        len_ext = strlen(fileext);
        if(mkdir(folder,S_IRUSR | S_IWUSR | S_IXUSR)==0){
            printf("success\n");
            DIR *d;
            struct dirent *dir;
            if(f_or_d==2){
                d = opendir(dash_d);
            } else {
                d = opendir("./soal3/");
            }
            if (d)
            {
                while ((dir = readdir(d)) != NULL){
                    // printf("%s\n",dir->d_name);
                    char tmp[300],new[300];
                    memset(tmp,0,300*sizeof(char));
                    memset(new,0,300*sizeof(char));
                    if(f_or_d==2){
                        strcpy(tmp,dash_d);
                    } else {
                        strcpy(tmp,"/home/edo/Kuliah/Sisop/SoalShiftSISOP20_modul3_E09/soal3/soal3/");
                    }
                    sprintf(new,"%s",folder);
                    len_file = strlen(dir->d_name);
                    if (is_ext(fileext,len_ext,dir->d_name,len_file)){
                        // printf("di if\n");
                        strcat(tmp,dir->d_name);
                        strcat(new,dir->d_name);
                        // printf("%s -> %s\n",tmp,new);
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
// =========================================================== MAIN ===========================================================
int main(int argc,char* argv[]){
    pthread_t *threads; 
	threads = (pthread_t*)malloc(100*sizeof(pthread_t));
    if(strcmp(argv[1],"-f")==0){
        if(argc<=2){
            printf("Argument passed wrong\n");
            exit(EXIT_FAILURE);
        }
        f_or_d = 1;
    } else if(strcmp(argv[1],"-d")==0){
        if(argc>2){
            printf("Argument passed wrong\n");
            exit(EXIT_FAILURE);
        }
        f_or_d = 2;
    } else if(strcmp(argv[1],"*")==0){
        f_or_d = 3;
    }else {
        printf("Argument not found\n");
        exit(EXIT_FAILURE);
    }

    if(f_or_d==1){
        int count = -1;
        for (int i = 2; i < argc; i++){
            if(is_dir(argv[i])){
                exit(EXIT_FAILURE);
            }
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
        
    } else if(f_or_d==2){
        int count = -1;
        if(is_dir(argv[2])){
            strcpy(dash_d,argv[2]);
            if(dash_d[strlen(dash_d)-1]!='/'){
                strcat(dash_d,"/");
            }
            char temp[300]="\0";
            // printf("%s\n",dash_d);
            DIR *d;
            struct dirent *dir;
            d = opendir(dash_d);
            if(d){
                while ((dir = readdir(d)) != NULL)
                {
                    memset(temp,0,200*sizeof(char));
                    strcpy(temp,dash_d);
                    strcat(temp,dir->d_name);
                    if(is_dir(temp)){
                        continue;
                    } else {
                        char *ext = strrchr(dir->d_name, '.');
                        if(!ext){
                            printf("%s - kosong\n",dir->d_name);
                            pthread_create(&threads[++count],NULL,dash_f,NULL);
                        } else {
                            printf("%s - %s\n",dir->d_name,ext);
                            pthread_create(&threads[++count],NULL,dash_f,(void*)ext);
                        }
                    }sleep(2);
                }closedir(d);
                
            }
            for(int i=0;i<=count;i++){
                pthread_join(threads[i],NULL);
            }
        } else {
            exit(EXIT_FAILURE);
        }
        
        
    }
    else if (f_or_d==3){
        int count = -1;
        DIR *d;
        struct dirent *dir;
        d = opendir("./soal3/");
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
                    continue;
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