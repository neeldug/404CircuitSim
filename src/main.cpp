#include <iostream>
#include <fstream>
#include <circuit.hpp>
#include <filesystem>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int c;
    std::map<std::string, std::string> stringFlags;
    std::map<std::string, bool> boolFlags;
    while ((c = getopt(argc, argv, "f:i:o:p")) != -1)
    {
        switch (c)
        {
        case 'f':
            stringFlags["outputFormat"] = optarg;
        case 'i':
            stringFlags["inputFilePath"] = optarg;
            break;
        case 'o':
            stringFlags["outputFolderPath"] = optarg;
            break;
        case 'p':
            boolFlags["plotOutput"] = true;
            break;
        default:
            std::cerr << "Unknown Flag: '" << c << "'\n";
            exit(1);
        }
    }

    std::ifstream inputFile;
    if (!stringFlags["inputFilePath"].empty())
    {
        inputFile.open(stringFlags["inputFilePath"]);
    }
    else
    {
        std::cerr << "-i flag required to specify input netlist" << std::endl;
        exit(1);
    }

    if (inputFile.fail())
    {
        std::cerr << "File: " << stringFlags["inputFileName"] << " was not found!\n";
        exit(1);
    }

    Circuit::Schematic *schem = Circuit::Parser::parse(inputFile);
    inputFile.close();

    if (stringFlags["outputFolderPath"].empty())
    {
        stringFlags["outputFolderPath"] = "out";
    }

    std::filesystem::create_directory(stringFlags["outputFolderPath"]);
    std::ofstream out;
    std::string outputPath;
    Circuit::Simulator::OutputFormat outputFormat;

    if (stringFlags["outputFormat"].empty() || tolower(stringFlags["outputFormat"][0]) == 'c')
    {
        outputFormat = Circuit::Simulator::OutputFormat::CSV;
    }
    else
    {
        outputFormat = Circuit::Simulator::OutputFormat::SPACE;
    }

    for (Circuit::Simulator *sim : schem->sims)
    {
        outputPath = stringFlags["outputFolderPath"] + "/" + schem->title.substr(2) + sim->simulationTypeMap[sim->type];
        if (outputFormat == Circuit::Simulator::OutputFormat::CSV && sim->type != Circuit::Simulator::SimulationType::OP)
        {
            outputPath += ".csv";
        }
        else
        {
            outputPath += ".txt";
        }
        out.open(outputPath);
        sim->run(out, outputFormat);
        out.close();

        if (boolFlags["plotOutput"] && sim->type != Circuit::Simulator::SimulationType::OP)
        {
            std::string systemCall = "env/bin/python3 plot.py '" + outputPath + "'";

            if (outputFormat == Circuit::Simulator::OutputFormat::SPACE)
            {
                systemCall += " -m space";
            }
            system(systemCall.c_str());
        }
    }
    return 0;
}
