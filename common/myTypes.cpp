#include "myTypes.h"

Image::Image(Image&& other) noexcept
{
	*this = std::move(other);
}

Image::~Image()
{
	if (backendHandle && backendCleanup)
	{
		backendCleanup(backendHandle);
		backendHandle = nullptr;
	}
}

std::string ToUpper(const std::string& input)
{
	std::string result = input;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}

std::string ExtractPrefix(const std::string& input)
{
	size_t i = 0;
	while (i < input.size() && !std::isdigit(input[i])) 
	{
		++i;
	}

	return input.substr(0, i);
}


