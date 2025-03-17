#include "catch2/catch.hpp"
#include "sstream"
#include "SyntaxAnalizator/SyntaxAnalizator.h"

TEST_CASE("Positive")
{
	std::stringstream input;

	SECTION("Just id")
	{
		std::stringstream output;
		input << "a";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Member")
	{
		std::stringstream output;
		input << "a.b";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Logical")
	{
		std::stringstream output;
		input << "a > b";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Hard")
	{
		std::stringstream output;
		input << "-a + 5.3E-15 * (-a + -b * (a * -b) -c) != abc";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Hard2")
	{
		std::stringstream output;
		input << "not a[7][a+5][b(3.5, c.d[f * ab])] or 15 * (r - br mod 5) and true";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Hard3")
	{
		std::stringstream output;
		input << "x + (x + + y)";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}

	SECTION("Hard4")
	{
		std::stringstream output;
		input << "- ( - 5 - 5 + 5) - 5 * -5.0e+10";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "OK\n");
	}
}

TEST_CASE("Negative")
{
	std::stringstream input;

	SECTION("Bracket")
	{
		std::stringstream output;
		input << ")(";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION("ab.5")
	{
		std::stringstream output;
		input << "ab.5";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION("ab.")
	{
		std::stringstream output;
		input << "ab.";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION("5a")
	{
		std::stringstream output;
		input << "5a";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION("fn(a, )")
	{
		std::stringstream output;
		input << "fn(a, )";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION("+-+-")
	{
		std::stringstream output;
		input << "+-+-";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}

	SECTION(".")
	{
		std::stringstream output;
		input << ".";
		SyntaxAnalizator().ScanExpression(input, output);
		REQUIRE(output.str() == "ERROR\n");
	}
}