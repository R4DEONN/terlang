#include "Lexer.h"
#include "iostream"
#include "../TokenUtils.h"

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
		if (m_state == LexerState::COMMENT)
		{
			PushToken(lexem, row, pos);
			m_state = LexerState::DEFAULT;
			pos = 1;
		}
		if (m_state != LexerState::DEFAULT)
		{
			lexem += '\n';
		}
		else
		{
			row = actualRow;
		}

		for (int i = 0; i < line.size(); i++)
		{
			if (lexem.empty())
			{
				pos = i + 1;
			}
			else if (IsOperation(lexem[lexem.size() - 1]) && !IsOperation(line[i]) && m_state == LexerState::DEFAULT)
			{
				if (lexem.size() == 1 || (lexem.size() > 1 && lexem[lexem.size() - 2] != 'e' && lexem[lexem.size() - 2] != 'E'))
				{
					PushToken(lexem, row, pos);
					pos = i + 1;
				}
			}


			if (m_state == LexerState::BLOCK_COMMENT && line[i] == '/' && i != 0 && line[i - 1] == '*')
			{
				lexem += line[i];
				PushToken(lexem, row, pos);
				m_state = LexerState::DEFAULT;
				row = actualRow;
				continue;
			}

			if (m_state == LexerState::DEFAULT && line[i] == '/')
			{
				if (i + 1 < line.size())
				{
					if (line[i + 1] == '/')
					{
						PushToken(lexem, row, pos);
						m_state = LexerState::COMMENT;
						pos = i + 1;
						lexem = "//";
						i += 1;
						continue;
					}
					else if (line[i + 1] == '*')
					{
						PushToken(lexem, row, pos);
						m_state = LexerState::BLOCK_COMMENT;
						pos = i + 1;
						lexem = "/*";
						i += 1;
						continue;
					}
				}
			}

			if (!lexem.empty() && IsOperation(line[i]) && !IsOperation(lexem[lexem.size() - 1]) && m_state == LexerState::DEFAULT)
			{
				if (i == 0 || (lexem[lexem.size() - 1] != 'e' && lexem[lexem.size() - 1] != 'E'))
				{
					PushToken(lexem, row, pos);
				}
			}

			if (IsSeparator(line[i]) && m_state == LexerState::DEFAULT)
			{
				PushToken(lexem, row, pos);

				pos = i + 1;
				if (!std::isspace(line[i]))
				{
					lexem = line[i];
					PushToken(lexem, row, pos);
				}
				lexem = "";

				continue;
			}

			if (IsQuote(line[i]))
			{
				if (i == 0 || line[i - 1] != '\\')
				{
					if (m_state == LexerState::DEFAULT)
					{
						m_state = LexerState::STRING;
						PushToken(lexem, row, pos);
						lexem = line[i];
						pos = i + 1;
						continue;
					}
					else if (m_state == LexerState::STRING)
					{
						m_state = LexerState::DEFAULT;
						lexem += line[i];
						PushToken(lexem, row, pos);
						continue;
					}
				}
			}

			lexem += line[i];
		}

		if (!lexem.empty() && m_state == LexerState::DEFAULT)
		{
			PushToken(lexem, row, pos);
			pos = 1;
		}
	}

	PushToken(lexem, row, pos);
}

std::vector<Token> Lexer::GetTokenList()
{
	return m_tokenList;
}

void Lexer::PushToken(std::string& lexem, int row, int pos)
{
	if (!lexem.empty())
	{
		m_tokenList.push_back(CreateToken(lexem, row, pos));
		lexem = "";
	}
}
