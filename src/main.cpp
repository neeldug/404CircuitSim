#include <iostream>
#include <fstream>
#include <circuit.hpp>
#include <filesystem>
#include <regex>

using flagString = std::pair<std::string, std::string>;
using flagBool = std::pair<std::string, bool>;

std::map<std::string, std::string> inputFlags = {
    flagString("-p", ""),    //plot output
    flagString("-o", "out"), //name output folder
};
std::map<std::string, bool> inputlessFlags = {
    flagBool("-s", false), //dump spice file
    flagBool("-n", false), //print nodes and attached components names
    flagBool("--interactive", false)};
int main(int argc, char const *argv[])
{
    assert(argc > 1 && "File Name required as first parameter");
    std::string inputFileName = argv[1];

    for (int i = 2; i < argc; i++)
    {
        if (inputFlags.find(std::string(argv[i])) != inputFlags.end())
        {
            if (i + 1 < argc)
            {
                inputFlags[std::string(argv[i])] = argv[i + 1];
            }
        }
        if (inputlessFlags.find(std::string(argv[i])) != inputlessFlags.end())
        {
            inputlessFlags[argv[i]] = true;
        }
    }
    std::ifstream inputFile;
    inputFile.open(inputFileName);
    if (inputFile.fail())
    {
        std::cerr << "File: " << inputFileName << " not found!\n";
        exit(1);
    }
    Circuit::Schematic *schem = Circuit::Parser::parse(inputFile);
    inputFile.close();

    for (Circuit::Simulator *sim : schem->sims)
    {
        std::filesystem::create_directory(inputFlags["-o"]);
        std::ofstream out(inputFlags["-o"] + "/" + schem->title + sim->simulationTypeMap[sim->type]);
        // REVIEW - output format could be a flag
        sim->run(out, Circuit::Simulator::OutputFormat::CSV);
        out.close();
    }

    delete schem;
    return 0;
}
