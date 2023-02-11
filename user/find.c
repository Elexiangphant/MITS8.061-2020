#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char* path,char *file){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    //fd refers to path 
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    } 
    switch(st.type){
    case T_FILE:
        printf("find: %s is not a path\n",path);
        break;

    case T_DIR:
    //when path is a directory, we should traversal the file in it.
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
        break;
    }
        
        //Traversal
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0||strcmp(".",de.name)==0||strcmp(de.name,"..")==0)
            continue;
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);    //add de.name back to path/
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
       
        if(st.type==T_DIR) find(buf,file);
        else if(st.type==T_FILE&&strcmp(de.name,file)==0)
            printf("%s\n",buf);
        
    }
        break;
  }
    close(fd);       
}

int main(int argc,char *argv[]){

    if(argc==3){
        char path[512];
        strcpy(path,argv[1]);
        char file[512];
        strcpy(file,argv[2]);
        find(path,file);
    }
    exit(0);
}