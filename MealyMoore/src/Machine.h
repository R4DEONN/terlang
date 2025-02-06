#pragma once

#include <utility>
#include <vector>
#include <string>
#include "IMachine.h"

enum class MachineType
{
	Mealy,
	Moore,
};

class Machine : public IMachine
{
public:
	Machine(std::vector<std::string> inputs, std::vector<std::string> outputs)
	: m_inputs(std::move(inputs)), m_outputs(std::move(outputs))
	{}

	[[nodiscard]] const std::vector<std::string>& GetInputs() const;

	[[nodiscard]] const std::vector<std::string>& GetOutputs() const;
protected:
	~Machine() = default;

private:
	std::vector<std::string> m_inputs;
	std::vector<std::string> m_outputs;
};
