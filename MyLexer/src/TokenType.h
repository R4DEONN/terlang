#pragma once
#include "string"
#include "regex"
#include "map"

enum class TokenType
{
	NUMBER,
	VARIABLE_DECLARATION,
	IF_STATEMENT,
	ELSE_BLOCK,
	PLUS,
	MINUS,
	DIVISION,
	MULTIPLICATION,
	RETURN,
	FUNCTION,
	ASSIGNMENT,
	LOGICAL,
	FOR_STATEMENT,
	IDENTIFIER,
	STRING,
	COMMENT,
	COMMENT_BLOCK,
	BRACKET,
	CURLY_BRACKET,
	SEPARATOR,
	ERROR,
};

inline static std::vector<std::pair<TokenType, std::regex>> tokenTypeToRegexMap
{
	{TokenType::STRING, std::regex(R"lit((^"([^"\\]*(\\.[^"\\]*)*)"))lit")},
	{TokenType::COMMENT, std::regex("(^//.*)")},
	{TokenType::COMMENT_BLOCK, std::regex(R"(^\/\*[\s\S]*\*\/)")},
	{TokenType::LOGICAL, std::regex(R"(^(===)|(==)|(!==)|(!=)|(<=)|(>=)|(<)|(>))")},
	{TokenType::NUMBER, std::regex(R"(^-?\d*(\.)?\d*([eE][-+]?\d+)?$)")},
	{TokenType::VARIABLE_DECLARATION, std::regex(R"(^(var)|(let)|(const))")},
	{TokenType::IF_STATEMENT, std::regex(R"(^if)")},
	{TokenType::ELSE_BLOCK, std::regex(R"(^else)")},
	{TokenType::PLUS, std::regex(R"(^\+)")},
	{TokenType::MINUS, std::regex(R"(^-)")},
	{TokenType::DIVISION, std::regex(R"(^/)")},
	{TokenType::MULTIPLICATION, std::regex(R"(^\*)")},
	{TokenType::ASSIGNMENT, std::regex(R"(^=)")},
	{TokenType::FOR_STATEMENT, std::regex(R"(^for)")},
	{TokenType::RETURN, std::regex(R"(^return)")},
	{TokenType::FUNCTION, std::regex(R"(^function)")},
	{TokenType::BRACKET, std::regex(R"(^(\()|(\)))")},
	{TokenType::CURLY_BRACKET, std::regex(R"(^(\{)|(\}))")},
	{TokenType::SEPARATOR, std::regex(R"(^(\;))")},
	{TokenType::IDENTIFIER, std::regex(R"(^[а-яА-Яa-zA-Z][а-яА-Яa-zA-Z0-9_.]*$)")},
};
