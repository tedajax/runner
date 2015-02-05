#include "../directory.h"

#include <dirent.h>

//TODO: need to implement this on the linux side and get the makefile ignoring the \Win directory

Directory* platform_directory_open(const char* path) {
    //DIR *dir;
    //struct dirent *ent;
    //if ((dir = opendir("c:\\src\\")) != NULL)
    //{
    //    /* print all the files and directories within directory */
    //    while ((ent = readdir(dir)) != NULL)
    //    {
    //        printf("%s\n", ent->d_name);
    //    }
    //    closedir(dir);
    //}
    //else
    //{
    //    /* could not open directory */
    //    perror("");
    //    return EXIT_FAILURE;
    //}
}
