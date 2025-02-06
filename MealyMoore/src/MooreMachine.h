#pragma once

#include "Machine.h"

#include <utility>
#include <map>

struct MooreTransition
{
	std::string input;
	std::string data;
};

struct MooreState
{
	std::string name;
	std::string output;
	std::map<std::string, std::string> transitions;
};

class MooreMachine : public Machine
{
public:
	MooreMachine(std::vector<std::string> inputs, std::vector<std::string> outputs, std::map<std::string, MooreState> states)
	: Machine(std::move(inputs), std::move(outputs)), m_states(std::move(states))
	{}

	void OutputAsIs(std::ofstream& ofs) override;

	void OutputAsAnotherMachineType(std::ofstream& ofs) override;

	IMachine* Minimize() override;

	void Visualize(const std::string& filename) override;

private:
	MooreState GetState(const std::string& name);

	std::map<std::string, MooreState> m_states;
};
