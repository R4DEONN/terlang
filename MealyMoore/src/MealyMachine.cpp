//
// Created by admin on 08.10.2024.
//

#include <iostream>
#include "MealyMachine.h"
#include "nlohmann/json.hpp"
#include "iomanip"
#include "fstream"
#include "MooreMachine.h"
#include <algorithm>
#include <set>
#include <map>

using json = nlohmann::json;

void MealyMachine::OutputAsIs(std::ofstream& ofs)
{
	json data;

	data["type"] = "mealy";
	data["inputs"] = GetInputs();
	data["outputs"] = GetOutputs();

	for (const auto& state : m_states)
	{
		json stateJson;
		stateJson["name"] = state.first;

		json transitionsJson;
		for (const auto& transition : state.second)
		{
			json transitionJson;
			transitionJson["nextState"] = transition.second.nextState;
			transitionJson["output"] = transition.second.output;

			transitionsJson[transition.first] = transitionJson;
		}

		stateJson["transitions"] = transitionsJson;
		data["states"].push_back(stateJson);
	}

	ofs << std::setw(4) << data << std::endl;
}

void MealyMachine::OutputAsAnotherMachineType(std::ofstream& ofs)
{
	json data;
	data["type"] = "moore";
	data["inputs"] = GetInputs();
	data["outputs"] = GetOutputs();

	std::vector<MooreState> states;

	for (const auto& state : m_states)
	{
		std::vector<MooreState> newStates;
		for (const auto& jstate : m_states)
		{
			for (const auto& transition : jstate.second)
			{
				if (transition.second.nextState == state.first)
				{
					bool isExist = false;
					for (const auto& newState : newStates)
					{
						if (newState.name == state.first + "_" + transition.second.output)
						{
							isExist = true;
							break;
						}
					}
					if (isExist)
					{
						continue;
					}
					MooreState newState;
					newState.name = state.first + "_" + transition.second.output;
					newState.output = transition.second.output;
					newState.transitions = std::map<std::string, std::string>();
					for (auto oldTransition : state.second)
					{
						MooreTransition mooreTransition;
						mooreTransition.input = oldTransition.first;
						mooreTransition.data = oldTransition.second.nextState + "_" + oldTransition.second.output;
						newState.transitions[mooreTransition.input] = (mooreTransition.data);
					}

					newStates.push_back(newState);
				}
			}
		}

		for (const auto& newState : newStates)
		{
			states.push_back(newState);
		}
	}

	for (const auto& state : states)
	{
		json stateJson;
		stateJson["name"] = state.name;
		stateJson["output"] = state.output;

		json transitionsJson;
		for (const auto& transition : state.transitions)
		{
			json transitionJson;
			transitionJson[transition.first] = transition.second;
			transitionsJson.push_back(transitionJson);
		}

		stateJson["transitions"] = transitionsJson;

		data["states"].push_back(stateJson);
	}

	ofs << std::setw(4) << data << std::endl;
}

IMachine* MealyMachine::Minimize()
{
	std::map<std::string, std::set<std::string>> outputGroups;
	for (const auto& state : m_states)
	{
		std::string outputKey;
		for (const auto& transition : state.second)
		{
			outputKey += transition.second.output + "|";
		}
		outputGroups[outputKey].insert(state.first);
	}

	std::map<std::string, std::set<std::string>> equGroups;
	bool isSplit = true;
	while (isSplit)
	{
		for (const auto& group : outputGroups)
		{
			for (const auto& stateName : group.second)
			{
				const auto& transitions = GetState(stateName);
				std::string outputKey;
				bool isFind = false;
				for (const auto& transition : transitions)
				{
					for (const auto& oldGroup : outputGroups)
					{
						if (oldGroup.second.contains(transition.second.nextState))
						{
							outputKey += transition.first;
							for (const auto& oldStateName : oldGroup.second)
							{
								outputKey += oldStateName + '|';
							}
							isFind = true;
							break;
						}
					}
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
	for (const auto& group : outputGroups)
	{
		remapName[group.first] = "S" + std::to_string(i);
		i++;
		std::cout << remapName[group.first] << ": ";
		for (const auto& transition : group.second)
		{
			 std::cout << transition << ", ";
		}
		std::cout << std::endl;
	}
	std::map<std::string, std::map<std::string, MealyTransitionData>> newStates;
	for (const auto& group : outputGroups)
	{
		std::map<std::string, MealyTransitionData> transitions;
		for (const auto& oldTransition : GetState(*group.second.begin()))
		{
			for (const auto& oldGroup : outputGroups)
			{
				if (oldGroup.second.contains(oldTransition.second.nextState))
				{
					MealyTransitionData data;
					data.output = oldTransition.second.output;
					data.nextState = remapName[oldGroup.first];
					transitions[oldTransition.first] = data;
					break;
				}
			}
		}

		newStates[remapName[group.first]] = transitions;
	}

	return new MealyMachine(GetInputs(), GetOutputs(), newStates);
}

std::map<std::string, MealyTransitionData> MealyMachine::GetState(const std::string& name)
{
	return m_states[name];
}

void MealyMachine::Visualize(const std::string& filename)
{
	std::ofstream ofs(filename);
	ofs << "digraph MealyMachine {\n";

	// Добавляем состояния и переходы
	for (const auto& state : m_states) {
		const std::string& stateName = state.first;
		for (const auto& transition : state.second) {
			const std::string& input = transition.first;
			const MealyTransitionData& data = transition.second;

			ofs << "    \"" << stateName << "\" -> \"" << data.nextState
				<< "\" [ label=\"" << input << " / " << data.output << "\" ];\n";
		}
	}

	ofs << "}\n";
	ofs.close();
}
