#include <sstream>
#include <Dictionary.h>
#include "GrammarOptimizer.h"

int main()
{
		std::stringstream input;
//		input << R"(
//<S> -> <A><B>#
//<A> -> a
//<A> -> a<A>c
//<B> -> b<A>
//)";
//		input << R"(
//<S> -> <E>#
//<E> -> <E>+<T>
//<E> -> <T>
//<T> -> <T>*<F>
//<T> -> <F>
//<F> -> id
//<F> -> -<F>
//<F> -> (<F>)
//)";

		input << R"(
<S> -> <ASS>#
<S> -> <IF>#
<ASS> -> <ident>=<Exp>
<IF> -> H<Exp>Z<ASS><ELSE>
<ELSE> -> J<ASS>
<ELSE> -> e
<Exp> -> <Exp><Rel><simexp>
<Exp> -> <simexp>

<simexp> -> <simexp><PLUSO><simterm>
<simexp> -> <simterm>

<simterm> -> <simterm><MULO><term>
<simterm> -> <term>

<term> -> +<term>
<term> -> -<term>
<term> -> (<Exp>)
<term> -> n<term>
<term> -> <ident>
<term> -> N
<term> -> t
<term> -> f
<term> -> s

<ident> -> <idindx>
<ident> -> <callFunc>

<comid> -> i
<comid> -> i.<comid>

<idindx> -> <comid><listInd>

<listInd> -> e
<listInd> -> [<Exp>]
<listInd> -> [<Exp>]<listInd>

<callFunc> -> <comid>(<listExp>)

<listExp> -> <Exp>
<listExp> -> <Exp>,<listExp>

<Rel> -> =
<Rel> -> q
<Rel> -> $
<Rel> -> @
<Rel> -> l
<Rel> -> b

<PLUSO> -> +
<PLUSO> -> -
<PLUSO> -> O

<MULO> -> A
<MULO> -> *
<MULO> -> /
<MULO> -> m
<MULO> -> d
)";

//	input << R"(
//<Rel> -> >
//)";

//	input << R"(
//<S> -> <Exp>#
//<Exp> -> <Exp><Rel><simexp>
//<Exp> -> <simexp>
//
//<simexp> -> <simexp><PLUSO><simterm>
//<simexp> -> <simterm>
//
//<simterm> -> <simterm><MULO><term>
//<simterm> -> <term>
//
//<term> -> +<term>
//<term> -> -<term>
//<term> -> (<Exp>)
//<term> -> n<term>
//<term> -> <ident>
//<term> -> N
//<term> -> t
//<term> -> f
//<term> -> s
//
//<ident> -> i
//<ident> -> <comid>
//<ident> -> <idindx>
//<ident> -> <callFunc>
//
//<comid> -> i
//<comid> -> i.<comid>
//
//<idindx> -> <comid><listInd>
//
//<listInd> -> [<Exp>]
//<listInd> -> [<Exp>]<listInd>
//
//<callFunc> -> <comid>(<listExp>)
//
//<listExp> -> <Exp>
//<listExp> -> <Exp>,<listExp>
//
//<Rel> -> =
//<Rel> -> q
//<Rel> -> $
//<Rel> -> @
//<Rel> -> l
//<Rel> -> b
//
//<PLUSO> -> +
//<PLUSO> -> -
//<PLUSO> -> O
//
//<MULO> -> A
//<MULO> -> *
//<MULO> -> /
//<MULO> -> m
//<MULO> -> d
//
//)"; //a = 5 + 5

		try
		{
			GrammarOptimizer::Optimize(input, std::cout);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
}