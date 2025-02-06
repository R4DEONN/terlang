#pragma once

#include "Machine.h"
#include "set"
#include "queue"
#include "algorithm"

// Помощник для формирования имени нового состояния
std::string GenerateStateName(const std::set<std::string>& states)
{
	std::string name;
	for (const auto& state : states)
	{
		name += state + ",";
	}
	if (!name.empty())
	{
		name.pop_back(); // Удалить последнюю запятую
	}
	return name;
}

std::set<std::string> EpsilonClosure(const std::string& state, const States& states)
{
	std::set<std::string> closure = { state };
	std::queue<std::string> toProcess;
	toProcess.push(state);

	while (!toProcess.empty())
	{
		std::string current = toProcess.front();
		toProcess.pop();

		auto currentState = states[std::stoi(current)];

		// Обрабатываем ε-переходы
		const auto& epsilonTransitions = currentState.transitions.find("e");
		if (epsilonTransitions != currentState.transitions.end())
		{
			for (const auto& next : epsilonTransitions->second)
			{
				if (closure.insert(next).second)
				{
					toProcess.push(next);
				}
			}
		}
	}

	return closure;
}

// Находим ε-замыкание для множества состояний
std::set<std::string> EpsilonClosure(const std::set<std::string>& states, const States& allStates)
{
	std::set<std::string> closure;
	for (const auto& state : states)
	{
		auto singleClosure = EpsilonClosure(state, allStates);
		closure.insert(singleClosure.begin(), singleClosure.end());
	}
	return closure;
}

// Переход по символу
std::set<std::string> Move(const std::set<std::string>& states, const std::string& symbol, const States& allStates)
{
	std::set<std::string> result;

	for (const auto& stateName : states)
	{
		// Находим состояние
		auto currentState = allStates[std::stoi(stateName)];

		// Ищем переход по символу
		const auto& transition = currentState.transitions.find(symbol);
		if (transition != currentState.transitions.end())
		{
			result.insert(transition->second.begin(), transition->second.end());
		}
	}

	return result;
}

// Функция детерминизации
DeterminizeStates Determinize(const States& states)
{
	DeterminizeStates determinizedStates;
	std::map<std::set<std::string>, std::string> processedStates;
	std::queue<std::set<std::string>> stateQueue;

	std::set<std::set<std::string>> allStates; // Множество всех обработанных множеств
	std::map<std::string, DeterminizeState> dfaMap;

	// Начальное состояние
	std::set<std::string> startState = { "0" }; // Предполагаем начальное состояние
	auto startClosure = EpsilonClosure(startState, states);
	std::string startStateName = GenerateStateName(startClosure);

	stateQueue.push(startClosure);
	processedStates[startClosure] = startStateName;

	while (!stateQueue.empty())
	{
		auto currentSet = stateQueue.front();
		stateQueue.pop();

		DeterminizeState currentDfaState;
		currentDfaState.isFinal = false;

		// Проверяем финальность
		for (const auto& stateName : currentSet)
		{
			auto currentState = states[std::stoi(stateName)];

			if (currentState.isFinal)
			{
				currentDfaState.isFinal = true;
				break;
			}
		}

		// Обрабатываем все символы алфавита (кроме ε)
		std::set<std::string> alphabet;
		for (const auto& state : states)
		{
			for (const auto& [symbol, _] : state.transitions)
			{
				if (symbol != "e")
				{
					alphabet.insert(symbol);
				}
			}
		}

		for (const auto& symbol : alphabet)
		{
			auto newStates = Move(currentSet, symbol, states);
			auto newClosure = EpsilonClosure(newStates, states);

			if (!newClosure.empty())
			{
				std::string newStateName;

				if (processedStates.count(newClosure) == 0)
				{
					newStateName = GenerateStateName(newClosure);
					processedStates[newClosure] = newStateName;
					stateQueue.push(newClosure);
				}
				else
				{
					newStateName = processedStates[newClosure];
				}

				currentDfaState.transitions[symbol] = newStateName;
			}
		}

		dfaMap[processedStates[currentSet]] = currentDfaState;
	}

	// Формируем результат
	for (const auto& [stateName, dfaState] : dfaMap)
	{
		determinizedStates.push_back(dfaState);
	}

	return determinizedStates;
}