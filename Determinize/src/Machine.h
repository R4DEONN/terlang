#pragma once

#include <utility>
#include <map>
#include <string>
#include <vector>

struct State
{
	bool isFinal;
	std::map<std::string, std::vector<std::string>> transitions;
};

using States = std::vector<State>;

struct DeterminizeState
{
	bool isFinal;
	std::map<std::string, std::string> transitions;
};
using DeterminizeStates = std::vector<DeterminizeState>;
