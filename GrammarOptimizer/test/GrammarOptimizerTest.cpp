#include "catch2/catch_test_macros.hpp"
#include "sstream"
#include "GrammarOptimizer.h"

TEST_CASE("Positive")
{
	std::stringstream input;

	SECTION("Easy")
	{
		input << R"(
<S> -> <A><B>#
<A> -> c<A>
<A> -> a
<B> -> b<A>
)";

		std::stringstream output;

		GrammarOptimizer::Optimize(input, output);

		REQUIRE(output.str() == R"(<S> -> <A><B># / a, c
<A> -> c<A> / c
<A> -> a / a
<B> -> b<A> / b
)");
	}
}