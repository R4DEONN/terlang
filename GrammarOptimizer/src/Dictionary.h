#pragma once

#include "unordered_map"
#include "set"
#include "Symbol.h"

class Dictionary
{
public:
	void Add(const std::shared_ptr<Symbol>& symbol)
	{
		if (std::find_if(
			m_allSymbols.begin(),
			m_allSymbols.end(),
			[&symbol](const std::shared_ptr<Symbol>& s) { return *s == *symbol; }) != m_allSymbols.end())
		{
			return;
		}
		m_allSymbols.push_back(symbol);

		if (symbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
		{
			auto nonTerminal = std::static_pointer_cast<NonTerminal>(symbol);
			if (nonTerminal)
			{
				m_nonTerminals.push_back(nonTerminal);
			}
			return;
		}

		auto terminal = std::static_pointer_cast<Terminal>(symbol);
		if (terminal)
		{
			m_terminals.push_back(terminal);
		}
	}

	[[nodiscard]] const std::vector<std::shared_ptr<NonTerminal>>& GetNonTerminals() const
	{
		return m_nonTerminals;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Terminal>>& GetTerminals() const
	{
		return m_terminals;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Symbol>>& GetAllSymbols() const
	{
		return m_allSymbols;
	}

	std::unordered_map<std::string, std::set<std::string>> ComputeFirstStar() const
	{
		// Шаг 1: Создание матрицы смежности
		size_t size = m_nonTerminals.size();
		std::vector<std::vector<bool>> adjacencyMatrix(size, std::vector<bool>(size, false));

		std::unordered_map<std::string, size_t> nonTerminalIndexMap;
		for (size_t i = 0; i < m_nonTerminals.size(); ++i)
		{
			nonTerminalIndexMap[m_nonTerminals[i]->GetValue()] = i;
		}

		for (const auto& nonTerminal : m_nonTerminals)
		{
			for (const auto& rule : nonTerminal->GetRules())
			{
				if (!rule.empty())
				{
					const auto& firstSymbol = rule[0];
					size_t from = nonTerminalIndexMap[nonTerminal->GetValue()];
					if (firstSymbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
					{
						size_t to = nonTerminalIndexMap[firstSymbol->GetValue()];
						adjacencyMatrix[from][to] = true;
					}
				}
			}
		}
		for (size_t k = 0; k < size; ++k)
		{
			for (size_t i = 0; i < size; ++i)
			{
				for (size_t j = 0; j < size; ++j)
				{
					adjacencyMatrix[i][j] = adjacencyMatrix[i][j] || (adjacencyMatrix[i][k] && adjacencyMatrix[k][j]);
				}
			}
		}

		// Шаг 4: Формирование результата (FIRST*)
		std::unordered_map<std::string, std::set<std::string>> firstStar;
		for (const auto& nonTerminal : m_nonTerminals)
		{
			const auto& nonTerminalValue = nonTerminal->GetValue();
			for (const auto& rule: nonTerminal->GetRules())
			{
				if (!rule.empty())
				{
					const auto& firstSymbol = rule[0];
					if (firstSymbol->GetType() == SYMBOL_TYPE::TERMINAL)
					{
						firstStar[nonTerminalValue].insert(firstSymbol->GetValue());
					}
				}
			}
		}
		for (const auto& nonTerminal : m_nonTerminals)
		{
			const auto& nonTerminalValue = nonTerminal->GetValue();

			for (const auto& rule: nonTerminal->GetRules())
			{
				if (!rule.empty())
				{
					const auto& firstSymbol = rule[0];
					if (firstSymbol->GetType() == SYMBOL_TYPE::TERMINAL)
					{
						firstStar[nonTerminalValue].insert(firstSymbol->GetValue());
					}
					else if (firstSymbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
					{
						for (const auto& terminal : firstStar[firstSymbol->GetValue()])
						{
							firstStar[nonTerminalValue].insert(terminal);
						}
					}
				}
			}
		}
		return firstStar;
	}

private:
	std::vector<std::shared_ptr<NonTerminal>> m_nonTerminals;
	std::vector<std::shared_ptr<Terminal>> m_terminals;
	std::vector<std::shared_ptr<Symbol>> m_allSymbols;
};

Dictionary CreateDictionaryFromInput(std::istream& input)
{
	Dictionary dictionary;
	std::string row;

	std::regex ruleRegex(R"((<[^>]+>)\s*->\s*((?:<[^>]+>|[^<>])+))");
	std::smatch matches;

	while (std::getline(input, row))
	{
		if (std::regex_match(row, matches, ruleRegex))
		{
			std::string left = matches[1].str();
			std::string right = matches[2].str();

			auto nonTerminalIt = std::find_if(
				dictionary.GetNonTerminals().begin(),
				dictionary.GetNonTerminals().end(),
				[left] (const std::shared_ptr<NonTerminal>& nonTerminal) {return nonTerminal->GetValue() == left;}
				);

			auto nonTerminalPtr = std::make_shared<NonTerminal>(left);
			if (nonTerminalIt == dictionary.GetNonTerminals().end())
			{
				nonTerminalPtr = std::make_shared<NonTerminal>(left);
				dictionary.Add(nonTerminalPtr);
			}
			else
			{
				nonTerminalPtr = *nonTerminalIt;
			}

			std::regex symbolRegex(R"(<[^>]+>|[^<>])");
			std::vector<std::shared_ptr<Symbol>> ruleSymbols;

			for (std::sregex_iterator it(right.begin(), right.end(), symbolRegex), end; it != end; ++it)
			{
				std::string symbol = it->str();
				if (symbol.front() == '<' && symbol.back() == '>')
				{
					auto nonTerminal = std::make_shared<NonTerminal>(symbol);
					ruleSymbols.push_back(nonTerminal);
				}
				else
				{
					auto terminal = std::make_shared<Terminal>(symbol);
					ruleSymbols.push_back(terminal);
					dictionary.Add(terminal);
				}
			}

			Rule rule(ruleSymbols.begin(), ruleSymbols.end());
			nonTerminalPtr->AddRule(rule);
		}
	}

	return dictionary;
}