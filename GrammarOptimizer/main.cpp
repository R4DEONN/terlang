#include <sstream>
#include <Dictionary.h>
#include "GrammarOptimizer.h"

int main()
{
		std::stringstream input;
//		input << R"(
//<S> -> <A><B>#
//<A> -> <A>c
//<A> -> a
//<B> -> b<A>
//<B> -> b<A><B>
//)";

	input << R"(
<Exp> -> <Exp><Rel><simexp>
<Exp> -> <simexp>

<simexp> -> <simexp><PLUSO><simterm>
<simexp> -> <simterm>

<simterm> -> <simterm><MULO><term>
<simterm> -> <term>

<term> -> +<term>
<term> -> -<term>
<term> -> (<Exp>)
<term> -> not<term>
<term> -> <ident>
<term> -> Number
<term> -> true
<term> -> false
<term> -> string

<ident> -> id
<ident> -> <comid>
<ident> -> <idindx>
<ident> -> <callFunc>

<comid> -> id
<comid> -> id.<comid>

<idindx> -> <comid><listInd>

<listInd> -> [<Exp>]
<listInd> -> [<Exp>]<listInd>

<callFunc> -> <comid>(<listExp>)

<listExp> -> <Exp>
<listExp> -> <Exp>,<listExp>

<Rel> -> =
<Rel> -> !=
<Rel> -> >
<Rel> -> <
<Rel> -> <=
<Rel> -> >=

<PLUSO> -> +
<PLUSO> -> -
<PLUSO> -> OR

<MULO> -> AND
<MULO> -> *
<MULO> -> /
<MULO> -> mod
<MULO> -> div

)"; //a = 5 + 5

		try
		{
			GrammarOptimizer::Optimize(input, std::cout);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
}