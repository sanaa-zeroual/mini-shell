#include <unistd.h>
#include <stdio.h>

void create_pipe(int files[2])
{
    if(pipe(files) == -1)
    {
        printf("error");
        return;
    }
    int id = fork();
    if(id == 0)
    {
        int x = 42;
        close(files[0]);
        write(files[1],&x, sizeof(int));
        close(files[1]);
    }
    else
    {
        int y ;
        close(files[1]);
        read(files[0], &y, sizeof(int));
        printf("parent read; %d", y);
        close(files[0]);
    }
}
