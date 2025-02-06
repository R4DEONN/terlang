#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "src/Machine.h"
#include "src/MachineCreator.h"

struct Args
{
	std::string inputFile;
	std::string outputFile;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 4)
	{
		return std::nullopt;
	}

	Args args;

	args.inputFile = argv[2];
	args.outputFile = argv[3];
	return args;
}

int main(int argc, char** argv)
{
	auto args = ParseArgs(argc, argv);
	if (!args)
	{
		std::cout << "Usage: MachineTransformer.exe (mealy-to-moore  | moore-to-mealy) in.json out.json" << std::endl;
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
		using namespace std::string_literals;
		auto machine = CreateMachine(input);
		if ("min"s == argv[1])
		{
			machine = machine->Minimize();
			machine->OutputAsIs(output);
			machine->Visualize("vis.dot");
		}
		else if ("morph"s == argv[1])
		{
			machine->OutputAsAnotherMachineType(output);
			machine->Visualize("vis.dot");
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return EXIT_SUCCESS;
}