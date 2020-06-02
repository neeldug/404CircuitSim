#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int c;
    std::string inputFilePath = "default";
    std::string outputFilePath = "default";
    while ((c = getopt(argc, argv, "ai:o:")) != -1)
    {
        switch (c)
        {
        case 'a':
            printf("switch a \n");
            break;
        case 'i':
            // printf("switch b \n");
            inputFilePath = optarg;
            break;
        case 'o':
            // printf("switch c - val: %s\n", optarg);
            outputFilePath = optarg;
            break;
        default:
            abort();
        }
    }
    std::cout << "I = " << inputFilePath << '\n';
    std::cout << "O = " << outputFilePath << '\n';
    return 0;
}
