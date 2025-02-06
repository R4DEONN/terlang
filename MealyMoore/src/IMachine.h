#pragma once
#include "string"

class IMachine
{
public:
	virtual IMachine* Minimize() = 0;
	virtual void OutputAsIs(std::ofstream& ofs) = 0;
	virtual void OutputAsAnotherMachineType(std::ofstream& ofs) = 0;
	virtual void Visualize(const std::string& filename) = 0;
protected:
	~IMachine() = default;
};