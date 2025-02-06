#pragma once

#include <sstream>
#include "Machine.h"
#include "string"
#include "iostream"
#include "algorithm"

std::vector<std::string> ParseLine(const std::string& string, char delim)
{
	std::vector<std::string> values;
	std::istringstream istringStream(string);

	std::string value;
	while (std::getline(istringStream, value, delim))
	{
		values.push_back(value);
	}
	if (string.back() == delim)
	{
		std::getline(istringStream, value);
		values.push_back(value);
	}

	return values;
}

States GetStatesWithoutTransition(std::istream& input)
{
	States states;

	std::string finalFlagString;
	std::string statesStr;
	std::getline(input, finalFlagString);
	std::getline(input, statesStr);

	std::string finalFlag;
	std::string stateName;
	std::istringstream finalFlagStream(finalFlagString);
	std::istringstream statesStream(statesStr);
	std::getline(finalFlagStream, finalFlag, ';');
	auto values = ParseLine(finalFlagString, ';');
	values.erase(values.begin());

	for (const auto& value: values)
	{
		State state;
		state.isFinal = value == "F";
		states.push_back(state);
	}

	return states;
}

States CreateMachine(std::istream& input)
{
	States states = GetStatesWithoutTransition(input);

	std::string line;
	for (size_t i = 0; std::getline(input, line); i++)
	{
		auto values = ParseLine(line, ';');
		for (size_t j = 0; j < states.size(); j++)
		{
			states[j].transitions.emplace(values[0], ParseLine(values[j + 1], ','));
		}
	}

	return states;
}