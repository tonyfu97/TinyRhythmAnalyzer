#pragma once

// Failure records the circumstances of a failing test.

#include <string>
#include <ostream>

class WFailure
{
public:
    WFailure(
        std::wstring theCondition,
        std::string theFileName,
        long theLineNumber);

	std::wstring condition;
    std::string fileName;
    long lineNumber;
};

std::wostream& operator<< (std::wostream& stream, WFailure& failure);