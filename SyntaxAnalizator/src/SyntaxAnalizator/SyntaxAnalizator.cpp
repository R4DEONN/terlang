#include "SyntaxAnalizator.h"
#include "iostream"

void SyntaxAnalizator::ScanExpression(std::istream& input, std::ostream& output)
{
	m_lexer.ScanInput(input);
	m_tokenList = m_lexer.GetTokenList();
	m_tokenIterator = m_tokenList.begin();

	if (IsExpression() && m_errorStack.empty())
	{
		output << "OK" << std::endl;
	}
	else
	{
		output << "ERROR" << std::endl;
	}
}

bool SyntaxAnalizator::IsExpression()
{
	if (IsSimExp())
	{
		if (IsRel())
		{
			if (IsExpression())
			{
				return true;
			}
			return false;
		}
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsRel()
{
	if (!IsTokensEnd() && m_tokenIterator->GetType() == TokenType::LOGICAL)
	{
		m_tokenIterator++;
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsSimExp()
{
	if (IsSimTerm())
	{
		if (IsPluso())
		{
			if (IsSimExp())
			{
				return true;
			}

			return false;
		}
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsPluso()
{
	if (IsTokensEnd())
	{
		return false;
	}
	auto type = m_tokenIterator->GetType();
	if (type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::OR)
	{
		m_tokenIterator++;
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsSimTerm()
{
	if (IsTerm())
	{
		if (IsMulo())
		{
			if (IsSimTerm())
			{
				return true;
			}

			return false;
		}
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsMulo()
{
	if (IsTokensEnd())
	{
		return false;
	}
	auto type = m_tokenIterator->GetType();
	if (type == TokenType::AND || type == TokenType::MULTIPLICATION || type == TokenType::DIVISION || type == TokenType::MOD || type == TokenType::DIV)
	{
		m_tokenIterator++;
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsTerm()
{
	if (IsTokensEnd())
	{
		return false;
	}
	auto type = m_tokenIterator->GetType();
	if (type == TokenType::PLUS || type == TokenType::MINUS)
	{
		m_tokenIterator++;
		if (IsTerm())
		{
			return true;
		}
		return false;
	}
	else if (!IsTokensEnd() && m_tokenIterator->GetData() == "(")
	{
		m_tokenIterator++;
		if (IsExpression())
		{
			if (!IsTokensEnd() && m_tokenIterator->GetData() == ")")
			{
				m_tokenIterator++;
				return true;
			}

			m_errorStack.push(Error::MISSING_CLOSING_BRACKET);
			return false;
		}
		m_errorStack.push(Error::MISSING_IDENTIFIER);
		return false;
	}
	else if (!IsTokensEnd() && m_tokenIterator->GetType() == TokenType::NOT)
	{
		m_tokenIterator++;
		if (IsTerm())
		{
			return true;
		}
	}
	else if (IsIdentifier())
	{
		return true;
	}
	else if (!IsTokensEnd() && m_tokenIterator->GetType() == TokenType::NUMBER)
	{
		m_tokenIterator++;
		return true;
	}
	else if (!IsTokensEnd() && m_tokenIterator->GetType() == TokenType::STRING)
	{
		m_tokenIterator++;
		return true;
	}
	else if (!IsTokensEnd() && m_tokenIterator->GetData() == "true" || m_tokenIterator->GetData() == "false")
	{
		m_tokenIterator++;
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsIdentifier()
{
	if (!IsTokensEnd() && m_tokenIterator->GetType() == TokenType::IDENTIFIER)
	{
		m_tokenIterator++;
		if (IsListInd())
		{
			return true;
		}
		else if (IsCallFunc())
		{
			return true;
		}
		return true;
	}

	return false;
}

bool SyntaxAnalizator::IsListInd()
{
	if (!IsTokensEnd() && m_tokenIterator->GetData() == "[")
	{
		m_tokenIterator++;
		if (IsExpression())
		{
			if (!IsTokensEnd() && m_tokenIterator->GetData() == "]")
			{
				m_tokenIterator++;
				if (IsListInd())
				{
					return true;
				}
				return true;
			}
			m_errorStack.push(Error::MISSING_CLOSING_BRACKET);
			return false;
		}
		m_errorStack.push(Error::MISSING_IDENTIFIER);
		return false;
	}

	return false;
}

bool SyntaxAnalizator::IsCallFunc()
{
	if (!IsTokensEnd() && m_tokenIterator->GetData() == "(")
	{
		m_tokenIterator++;
		if (IsListExp())
		{
			if (!IsTokensEnd() && m_tokenIterator->GetData() == ")")
			{
				m_tokenIterator++;
				return true;
			}
		}
	}
	return false;
}

bool SyntaxAnalizator::IsListExp()
{
	if (IsExpression())
	{
		if (!IsTokensEnd() && m_tokenIterator->GetData() == ",")
		{
			m_tokenIterator++;
			if (IsListExp())
			{
				return true;
			}

			m_errorStack.push(Error::MISSING_IDENTIFIER);
			return false;
		}

		return true;
	}
	return false;
}

bool SyntaxAnalizator::IsTokensEnd()
{
	return m_tokenIterator == m_tokenList.end();
}
