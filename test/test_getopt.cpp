#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "abc:")) != -1)
    {
        switch (c)
        {
        case 'a':
            printf("switch a \n");
            break;
        case 'b':
            printf("switch b \n");
            break;
        case 'c':
            printf("switch c - val: %s\n", optarg); 
            break;
        default:
            abort();
        }
    }

    return 0;
}
