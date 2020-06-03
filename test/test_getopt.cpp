#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int c;
    bool s,n = false;
    std::string o,p;
    while ((c = getopt(argc, argv, "sno:p:")) != -1)
    {
        switch (c)
        {
        case 's':
            s = true;
            break;
        case 'n':
            n = true;
            break;
        case 'o':
            o = optarg;
            break;
        case 'p':
            p = optarg;
            break;
        default:
            abort();
        }
    }

    std::cerr << "s = " << s << '\n';
    std::cerr << "n = " << n << '\n';
    std::cerr << "o = " << o << '\n';
    std::cerr << "p = " << p << '\n';
    return 0;
}
