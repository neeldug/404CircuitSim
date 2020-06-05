#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int c;
    std::string inputFilePath = "default";
    std::string outputFilePath = "default";
    bool s,n = false;
    while ((c = getopt(argc, argv, "sni:o:")) != -1)
    {
        switch (c)
        {
        case 's':
            s = true;
            break;
        case 'n':
            n = true;
            break;
        case 'i':
            inputFilePath = optarg;
            break;
        case 'o':
            outputFilePath = optarg;
            break;
        default:
            abort();
        }
    }

    std::cerr << "s = " << s << '\n';
    std::cerr << "n = " << n << '\n';
    std::cerr << "i = " << outputFilePath << '\n';
    std::cerr << "o = " << inputFilePath << '\n';
    return 0;
}
