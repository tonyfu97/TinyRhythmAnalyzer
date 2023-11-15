#include "WFailure.h"

#include <utility>

WFailure::WFailure(
    std::wstring theCondition,
    std::string theFileName,
    const long theLineNumber)
    : condition(std::move(theCondition)),
    fileName(std::move(theFileName)),
    lineNumber(theLineNumber)
{
}

std::wostream& operator<<(
    std::wostream& stream,
    WFailure& failure)
{
    const std::wstring wFileName(failure.fileName.begin(), failure.fileName.end());

    stream << std::endl
           << wFileName
           << L"(" << failure.lineNumber << L") : "
           << L"Failure: "
           << failure.condition;
    return stream;
}
