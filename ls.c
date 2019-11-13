#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

char* get_mode(mode_t m,char* str)//Permission
{
    if(S_ISREG(m))
        strcpy(str,"-");
    else if(S_ISDIR(m))
        strcpy(str,"d");
    else if(S_ISCHR(m))
        strcpy(str,"c");
    else if(S_ISBLK(m))
        strcpy(str,"b");
    else if(S_ISFIFO(m))
        strcpy(str,"f");
    else if(S_ISLNK(m))
            strcpy(str,"l");
    else if(S_ISSOCK(m))
        strcpy(str,"n");

    // users
    strcat(str,m&S_IRUSR?"r":"-");
    strcat(str,m&S_IWUSR?"w":"-");
    strcat(str,m&S_IXUSR?"x":"-");

    // group
    strcat(str,m&S_IRGRP?"r":"-");
    strcat(str,m&S_IWGRP?"w":"-");
    strcat(str,m&S_IXGRP?"x":"-");

    //Other
    strcat(str,m&S_IROTH?"r":"-");
    strcat(str,m&S_IWOTH?"w":"-");
    strcat(str,m&S_IXOTH?"x":"-");

    return str;
}

int main(int argc,char** argv,char** environ)
{
    char* dir_name= ".";
    if(argc == 1)
    {
        dir_name = argv[0];

        DIR* dp=opendir(".");
        if(NULL == dp)
        {
            perror("opendir");
            return -1;
        }
        struct dirent* de=readdir(dp);
        for(;de;de=readdir(dp))
        {
            printf("%s\t",de->d_name);

        }
  printf("\n");
        closedir(dp);
    }
    else if(argc == 2 && strcmp ( "-l",argv[1])== 0)
    {
        dir_name = argv[0];

        DIR* dp=opendir(".");
        if(NULL == dp)
        {
            perror("opendir");
            return -1;
        }
        struct dirent* de=readdir(dp);
        for(;de;de=readdir(dp))
        {
            if('.'==de->d_name[0]) continue;
            struct stat s;
            int ret = lstat(de->d_name,&s);
            if(0 > ret)
            {
                perror("stat");
                return -1;
            }
            char str[11] = {};
            printf("%s ",get_mode(s.st_mode,str));//type
            struct passwd *passwd;
             passwd = getpwuid(s.st_uid);
            printf ("%s ", passwd->pw_name);//user
            struct group *group;
            group = getgrgid(passwd->pw_gid);
            printf ("%s ", group->gr_name); //group
            printf("%5lu ",s.st_size);//size

            printf("%s\t",de->d_name);//file
            printf("\n");
        }
        closedir(dp);


    }

    else
    {
        puts("user:ls dir");
        return -1;
    }


}
