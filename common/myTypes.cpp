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
