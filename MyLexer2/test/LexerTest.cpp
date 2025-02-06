#include "catch2/catch_test_macros.hpp"
#include "sstream"
#include "Lexer.h"

void CheckToken(const Token& token, TokenType tokenType, std::string data, int row, int pos)
{
	REQUIRE(token.GetType() == tokenType);
	REQUIRE(token.GetData() == data);
	REQUIRE(token.GetRow() == row);
	REQUIRE(token.GetPos() == pos);
}

TEST_CASE("Numbers")
{
	std::stringstream input;
	SECTION("Just number")
	{
		input << "5";

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 1);
		auto numberToken = tokens[0];
		CheckToken(numberToken, TokenType::NUMBER, "5", 1, 1);
	}

	SECTION("Float")
	{
		auto data = "5.5";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 1);
		auto numberToken = tokens[0];
		CheckToken(numberToken, TokenType::NUMBER, data, 1, 1);
	}

	SECTION("Float unright")
	{
		auto data = "5. 5";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 2);
		CheckToken(tokens[0], TokenType::NUMBER, "5.", 1, 1);
		CheckToken(tokens[1], TokenType::NUMBER, "5", 1, 4);
	}
}

TEST_CASE("String")
{
	std::stringstream input;
	SECTION("Just string")
	{
		auto data = "\"kjfdskalf\"";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 1);
		CheckToken(tokens[0], TokenType::STRING, data, 1, 1);
	}

	SECTION("Multi string")
	{
		auto data = "\"kjfdskalf\nsfsdfa\n\"";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 1);
		CheckToken(tokens[0], TokenType::STRING, data, 1, 1);
	}
}

TEST_CASE("Comment")
{
	std::stringstream input;
	SECTION("Just comment")
	{
		auto data = "//sfasdfasdfas\n6";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 2);
		CheckToken(tokens[0], TokenType::COMMENT, "//sfasdfasdfas", 1, 1);
		CheckToken(tokens[1], TokenType::NUMBER, "6", 2, 1);
	}

	SECTION("Multi string")
	{
		auto data = "/*sa\nsd\n*/";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 1);
		CheckToken(tokens[0], TokenType::COMMENT_BLOCK, data, 1, 1);
	}
}

TEST_CASE("vars")
{
	std::stringstream input;
	SECTION("vats")
	{
		auto data = "var let const";
		input << data;

		auto lexer = Lexer();
		lexer.ScanInput(input);

		auto tokens = lexer.GetTokenList();
		REQUIRE(tokens.size() == 3);
		CheckToken(tokens[0], TokenType::VARIABLE_DECLARATION, "var", 1, 1);
		CheckToken(tokens[1], TokenType::VARIABLE_DECLARATION, "let", 1, 5);
		CheckToken(tokens[2], TokenType::VARIABLE_DECLARATION, "const", 1, 9);
	}
}