
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>


int main()
{
    int fd = open("/Users/ybourais/Desktop/blanat/mini.txt", O_RDWR);

    struct stat sb;

    int fs;
    fs = fstat(fd, &sb);
    char *containt = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    int i = 0;
    while (i < sb.st_size) 
    {
        write(1, &containt[i++], 1);
    }
    munmap(containt, sb.st_size);
    close(fd);
}
