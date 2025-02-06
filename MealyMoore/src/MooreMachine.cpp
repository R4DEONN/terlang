#include "MooreMachine.h"
#include "fstream"
#include "iostream"
#include "iomanip"
#include "set"
#include "map"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void MooreMachine::OutputAsIs(std::ofstream& ofs)
{
	json data;
	data["type"] = "moore";
	data["inputs"] = GetInputs();
	data["outputs"] = GetOutputs();

	for (const auto& state: m_states)
	{
		json stateJson;
		stateJson["name"] = state.first;
		stateJson["output"] = state.second.output;

		json transitionsJson;
		for (const auto& transition: state.second.transitions)
		{
			transitionsJson[transition.first] = transition.second;
		}

		stateJson["transitions"] = transitionsJson;
		data["states"].push_back(stateJson);
	}

	ofs << std::setw(4) << data << std::endl;
}

void MooreMachine::OutputAsAnotherMachineType(std::ofstream& ofs)
{
	json data;
	data["type"] = "mealy";
	data["inputs"] = GetInputs();
	data["outputs"] = GetOutputs();

	for (const auto& state: m_states)
	{
		json stateJson;
		stateJson["name"] = state.first;

		json transitionsJson;
		for (const auto& transition: state.second.transitions)
		{
			json transitionJson;
			transitionJson["nextState"] = transition.second;
			for (const auto& oldState: m_states)
			{
				if (oldState.first == transition.second)
				{
					transitionJson["output"] = oldState.second.output;
					break;
				}

			}

			transitionsJson[transition.first] = transitionJson;
		}

		stateJson["transitions"] = transitionsJson;
		data["states"].push_back(stateJson);
	}

	ofs << std::setw(4) << data << std::endl;
}

IMachine* MooreMachine::Minimize()
{
	std::map<std::string, std::set<std::string>> outputGroups;
	for (const auto& state: m_states)
	{
		std::string outputKey = state.second.output;
		outputGroups[outputKey].insert(state.first);
	}

	std::map<std::string, std::set<std::string>> equGroups;
	bool isSplit = true;
	while (isSplit)
	{
		for (const auto& group: outputGroups)
		{
			for (const auto& stateName: group.second)
			{
				const auto& state = GetState(stateName);
				std::string outputKey;
				bool isFind = false;
				for (const auto& transition: state.transitions)
				{
					for (const auto& oldGroup: outputGroups)
					{
						if (oldGroup.second.contains(transition.second))
						{
							outputKey += oldGroup.first + "|" + transition.first + "|" + transition.second;

							for (const auto& oldStateName: oldGroup.second)
							{
								outputKey += oldStateName + '|';
							}
							isFind = true;
							break;
						}
					}
//					if (isFind) break;
				}
				equGroups[outputKey].insert(stateName);
			}
		}

		isSplit = outputGroups.size() != equGroups.size();
		outputGroups = equGroups;
		equGroups.clear();
	}

	std::map<std::string, std::string> remapName;
	int i = 1;
	for (const auto& group: outputGroups)
	{
		remapName[group.first] = "S" + std::to_string(i);
		i++;
		std::cout << remapName[group.first] << ": ";
		for (const auto& transition: group.second)
		{
			std::cout << transition << ", ";
		}
		std::cout << std::endl;
	}
	std::map<std::string, MooreState> newStates;
	for (const auto& group: outputGroups)
	{
		auto oldState = GetState(*group.second.begin());
		MooreState state;
		state.output = oldState.output;
		state.name = oldState.name;
		for (const auto& oldTransition: GetState(*group.second.begin()).transitions)
		{
			for (const auto& oldGroup: outputGroups)
			{
				if (oldGroup.second.contains(oldTransition.second))
				{
					state.transitions[oldTransition.first] = remapName[oldGroup.first];
					break;
				}
			}
		}

		newStates[remapName[group.first]] = state;
	}

	return new MooreMachine(GetInputs(), GetOutputs(), newStates);
}

MooreState MooreMachine::GetState(const std::string& name)
{
	return m_states[name];
}

void MooreMachine::Visualize(const std::string& filename)
{
	std::cout << "mew" << std::endl;
	std::ofstream ofs(filename);
	ofs << "digraph MooreMachine {\n";

	for (const auto& statePair: m_states)
	{
		const std::string& stateName = statePair.first;
		const MooreState& state = statePair.second;

		ofs << "    \"" << stateName << "\" [ label=\"" << stateName << "\\nOutput: " << state.output << "\" ];\n";

		for (const auto& transition: state.transitions)
		{
			const std::string& input = transition.first;
			const std::string& nextState = transition.second;

			ofs << "    \"" << stateName << "\" -> \"" << nextState
				<< "\" [ label=\"" << input << "\" ];\n";
		}
	}

	ofs << "}\n";
	ofs.close();
}
