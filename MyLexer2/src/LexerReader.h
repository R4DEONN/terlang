#pragma once

#include "vector"
#include "Token.h"
#include "iostream"

class LexerReader
{
public:
	void WriteTokens(const std::vector<Token>& tokens, std::ostream& output)
	{
		for (const auto& token : tokens)
		{
			if (token.GetType() == TokenType::COMMENT_BLOCK || token.GetType() == TokenType::COMMENT)
			{
				continue;
			}
			WriteName(token, output);
			output << " (" << token.GetRow() << ", " << token.GetPos() << ") " << token.GetData();
			output << std::endl;
		}
	}
private:
	void WriteName(Token token, std::ostream& output)
	{
		std::string str = "";
		switch (token.GetType())
		{
		case TokenType::IDENTIFIER:
			str = "identifier";
			break;
		case TokenType::IF_STATEMENT:
			str = "if_statement";
			break;
		case TokenType::NUMBER:
			str = "number";
			break;
		case TokenType::VARIABLE_DECLARATION:
			str = "variable_declaration";
			break;
		case TokenType::ELSE_BLOCK:
			str = "else_block";
			break;
		case TokenType::PLUS:
			str = "plus";
			break;
		case TokenType::MINUS:
			str = "minus";
			break;
		case TokenType::DIVISION:
			str = "division";
			break;
		case TokenType::MULTIPLICATION:
			str = "multiplication";
			break;
		case TokenType::ASSIGNMENT:
			str = "assignment";
			break;
		case TokenType::LOGICAL:
			str = "logical";
			break;
		case TokenType::FOR_STATEMENT:
			str = "for_statement";
			break;
		case TokenType::BRACKET:
			str = "bracket";
			break;
		case TokenType::CURLY_BRACKET:
			str = "curly_bracket";
			break;
		case TokenType::STRING:
			str = "string";
			break;
		case TokenType::COMMENT:
			str = "comment";
			break;
		case TokenType::COMMENT_BLOCK:
			str = "comment_block";
			break;
		case TokenType::SEPARATOR:
			str = "separator";
			break;
		case TokenType::ERROR:
			str = "error";
			break;
		case TokenType::RETURN:
			str = "return";
			break;
		case TokenType::FUNCTION:
			str = "function_declaration";
			break;
		}

		output << str;
	}
};
