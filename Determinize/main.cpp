#include <optional>
#include <string>
#include <iostream>
#include <fstream>
#include "src/CreateMachine.h"
#include "src/Determinize.h"

struct Args
{
	std::string inputFile;
	std::string outputFile;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		return std::nullopt;
	}

	Args args;

	args.inputFile = argv[1];
	args.outputFile = argv[2];
	return args;
}

int main(int argc, char** argv)
{
	auto args = ParseArgs(argc, argv);
	if (!args)
	{
		std::cout << "Usage: Determinize.exe in.csv out.csv" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(args->inputFile);
	if (!input.is_open())
	{
		std::cout << "Input file couldn't be opened" << std::endl;
		return EXIT_FAILURE;
	}

	std::ofstream output(args->outputFile);
	if (!output.is_open())
	{
		std::cout << "Output file couldn't be opened" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		auto machine = CreateMachine(input);
		auto dfa = Determinize(machine);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return EXIT_SUCCESS;
}