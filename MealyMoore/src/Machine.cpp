//
// Created by admin on 08.10.2024.
//

#include "Machine.h"

const std::vector<std::string>& Machine::GetInputs() const
{
	return m_inputs;
}

const std::vector<std::string>& Machine::GetOutputs() const
{
	return m_outputs;
}
