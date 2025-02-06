#include "catch2/catch_test_macros.hpp"
#include "TokenUtils.h"

TEST_CASE("GetFirstToken")
{
	std::string data = "5";
	auto token = GetFirstToken(data, 1, 1);

	REQUIRE(token.GetType() == TokenType::NUMBER);
	REQUIRE(token.GetData() == data);
}
