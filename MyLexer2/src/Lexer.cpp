#include "Lexer.h"
#include "iostream"
#include "TokenUtils.h"
#include "stack"

void Lexer::ScanInput(std::istream& input)
{
	std::string line;
	int row{};
	int actualRow{};
	int pos = 1;
	std::string lexem;

	while (std::getline(input, line))
	{
		actualRow++;
		for (int i = 0; i < line.size(); i++)
		{
			if (m_currentBlock != Block::DEFAULT)
			{
				lexem += line[i];

			}
		}
	}
}

std::vector<Token> Lexer::GetTokenList()
{
	return m_tokenList;
}

void Lexer::PushToken(std::string& lexem, int row, int pos)
{
	if (!lexem.empty())
	{
		m_tokenList.push_back(GetFirstToken(lexem, row, pos));
		lexem = "";
	}
}

void Lexer::CheckBlockEnd(const std::string& input, int currentPos, const std::string& lexem)
{
	if (m_currentBlock == Block::COMMENT && input[currentPos] == '"')
	{
		m_currentBlock == Block::DEFAULT;
	}
}
