#include "fstream"
#include "Lexer/Lexer.h"
#include "Lexer/LexerReader.h"

int main(int argc, char** argv)
{
	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);
	auto lexer = Lexer();
	lexer.ScanInput(input);

	auto lexerReader = LexerReader();
	lexerReader.WriteTokens(lexer.GetTokenList(), output);
}