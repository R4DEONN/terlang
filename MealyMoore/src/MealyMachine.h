#pragma once

#include "Machine.h"
#include <utility>
#include <fstream>
#include <map>

struct MealyTransitionData
{
	std::string nextState;
	std::string output;
};

struct MealyTransition
{
	std::string input;
	MealyTransitionData data;
};

struct MealyState
{
	std::string name;
	std::map<std::string, MealyTransitionData> transitions;
};

class MealyMachine : public Machine
{
public:
	MealyMachine(std::vector<std::string> inputs, std::vector<std::string> outputs, std::map<std::string, std::map<std::string, MealyTransitionData>> states)
	: Machine(std::move(inputs), std::move(outputs)), m_states(std::move(states))
	{}

	void OutputAsIs(std::ofstream& ofs) override;

	void OutputAsAnotherMachineType(std::ofstream& ofs) override;

	IMachine* Minimize() override;

	void Visualize(const std::string& filename) override;

private:
	std::map<std::string, MealyTransitionData> GetState(const std::string& name);

	std::map<std::string, std::map<std::string, MealyTransitionData>> m_states;
};
