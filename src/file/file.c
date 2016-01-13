#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NAMESIZE 50

int main(void)
{
    size_t l;
    FILE *fp;
    struct {
        short count;
        long  total;
        char  name[NAMESIZE];
    } item;

    if ((fp = fopen("/tmp/test", "w+")) == NULL) {
        perror("fopen wrong");
        exit(EXIT_FAILURE);
    }

    if ((l = fwrite(&item, sizeof(item), 1, fp)) != 1) {
        perror("fwrite wrong");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    return 0;
}
