#pragma once

#include "vector"
#include "TokenType.h"
#include "Token.h"

enum class Block
{
	DEFAULT,
	COMMENT,
	COMMENT_BLOCK,
	STRING,
};

class Lexer
{
public:
	void ScanInput(std::istream& input);
	std::vector<Token> GetTokenList();
	void PushToken(std::string& lexem, int row, int pos);
private:
	std::vector<Token> m_tokenList;
	Block m_currentBlock = Block::DEFAULT;

	void CheckBlockEnd(const std::string& input, int currentPos, const std::string& lexem);
};