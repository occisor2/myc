#pragma once

#include <exception>
#include <string>

namespace error
{

/**
 * Exception for when compilation can not continue.
 */
class FatalError : std::exception
{
public:
	FatalError() = default;
	const char* what() const noexcept override
	{
		return "Compilation stopped due to fatal errors.";
	}
};

/**
 * Writes the error message to stdout along with the location and then
 * throws an error::Error exception.
 */
[[noreturn]] void fatal(const std::string& message, const std::string& fileName, int line);

}
