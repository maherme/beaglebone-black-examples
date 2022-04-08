#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

char buffer[2048] = {0};

int main(int argc, char* argv[]){

    int fd = 0;
    int remaining = 0;
    int total_read = 0;
    int ret = 0;
    int n = 0;  /* For counting the reading attemps */

    if(argc != 2){
        printf("Wrong argument, usage: <file> <read count>\n");
        goto out;
    }

    remaining = atoi(argv[1]);
    printf("Read requested = %d\n", remaining);

    fd = open("/dev/pcdev-3", O_RDONLY);
    if(fd < 0){
        perror("Open device failed");
        goto out;
    }

    printf("Open was successful\n");

#if 0
    /* Activate this code for testing lseek */
    ret = lseek(fd, 10, SEEK_SET);
    if(ret < 0){
        perror("lseek failed");
        goto close_fd;
    }
#endif

    /* Attempt reading twice */
    while((n != 2) && remaining){
        ret = read(fd, &buffer[total_read], remaining);
        if(!ret){
            printf("End of file\n");
            break;
        }
        else if(ret <= remaining){
            printf("Read %d bytes of data\n", ret);
            total_read += ret;
            remaining -= ret;
        }
        else if(ret < 0){
            printf("Read file failed\n");
            break;
        }
        else{
            break;
        }
        n++;
    }

    printf("Total read = %d\n", total_read);

    for(int i = 0; i < total_read; i++){
        printf("%c", buffer[i]);
    }

close_fd:
    close(fd);
out:
    return 0;
}
