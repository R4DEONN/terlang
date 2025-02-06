#pragma once

#include "Machine.h"
#include "fstream"
#include "exception"
#include "MealyMachine.h"
#include "MooreMachine.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct MealyArgs
{
	std::map<std::string, std::map<std::string, MealyTransitionData>> states;
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
};

struct MooreArgs
{
	std::map<std::string, MooreState> states;
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
};

MealyArgs GetMealyArgs(const json& data)
{
	MealyArgs args;

	data.at("inputs").get_to(args.inputs);
	data.at("outputs").get_to(args.outputs);

	for (const auto& stateJson : data.at("states"))
	{
		MealyState state;
		stateJson.at("name").get_to(state.name);

		for (const auto& transitionJson : stateJson.at("transitions").items())
		{
			MealyTransition transition;
			transition.input = transitionJson.key();
			transition.data.nextState = transitionJson.value().at("nextState").get<std::string>();
			transition.data.output = transitionJson.value().at("output").get<std::string>();
			state.transitions[transition.input] = transition.data;
		}

		args.states[state.name] = state.transitions;
	}

	return args;
}

MooreArgs GetMooreArgs(const json& data)
{
	MooreArgs args;

	data.at("inputs").get_to(args.inputs);
	data.at("outputs").get_to(args.outputs);

	for (const auto& stateJson : data.at("states"))
	{
		MooreState state;
		stateJson.at("name").get_to(state.name);
		stateJson.at("output").get_to(state.output);

		for (const auto& transitionJson : stateJson.at("transitions").items())
		{
			MooreTransition transition;
			transition.input = transitionJson.key();
			transition.data = transitionJson.value().get<std::string>();
			state.transitions[transition.input] = transition.data;
		}

		args.states[state.name] = state;
	}

	return args;
}

IMachine* CreateMachine(std::ifstream& ifs)
{
	json data = json::parse(ifs);
	const auto type = data.at("type").get<std::string>();

	if (type == "mealy")
	{
		auto args = GetMealyArgs(data);
		return new MealyMachine(args.inputs, args.outputs, args.states);
	}
	else if (type == "moore")
	{
		auto args = GetMooreArgs(data);
		return new MooreMachine(args.inputs, args.outputs, args.states);
	}

	throw std::invalid_argument("Unknown type of machine");
}