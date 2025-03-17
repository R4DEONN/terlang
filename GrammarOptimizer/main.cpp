#include <sstream>
#include <Dictionary.h>

int main()
{
		std::stringstream input;
		input << R"(
<S> -> <A><B>#
<A> -> c<A>
<A> -> a
<B> -> b<A>
)";
	auto dictionary = CreateDictionaryFromInput(input);

	std::cout << "Non-terminals:" << std::endl;
	for (const auto& nonTerminal : dictionary.GetNonTerminals())
	{
		std::cout << nonTerminal->GetValue() << std::endl;
	}

	std::cout << "\nTerminals:" << std::endl;
	for (const auto& terminal : dictionary.GetTerminals())
	{
		std::cout << terminal->GetValue() << std::endl;
	}

	dictionary.ComputeFirstStar();
}