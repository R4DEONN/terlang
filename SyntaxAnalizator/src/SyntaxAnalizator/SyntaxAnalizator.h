#pragma once

#include "Lexer/Lexer.h"
#include "iterator"

enum class Error
{
	MISSING_IDENTIFIER,
	MISSING_CLOSING_BRACKET,
};

class SyntaxAnalizator
{
public:
	void ScanExpression(std::istream& input, std::ostream& output);
private:
	bool IsExpression();
	bool IsRel();
	bool IsSimExp();
	bool IsPluso();
	bool IsMulo();
	bool IsSimTerm();
	bool IsTerm();
	bool IsCallFunc();
	bool IsListInd();
	bool IsListExp();
	bool IsIdentifier();
	bool IsTokensEnd();

	Lexer m_lexer;
	std::vector<Token> m_tokenList;
	std::vector<Token>::iterator m_tokenIterator;
	std::stack<Error> m_errorStack;
};
