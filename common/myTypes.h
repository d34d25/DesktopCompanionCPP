#pragma once
#include <vector>
#include <functional>
#include <string>
#include <map>
#include <ranges>
#include <algorithm>
#include <cstdlib>
#include <ctime>


template <typename T>
void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

struct Color
{
	float r, g, b, a;
};

struct Vector2D
{
	float x, y;
};

namespace Shapes
{
	struct Rectangle
	{
		float x, y, width, height;
	};

	struct Circle
	{
		float x, y, radius;
	};

	struct Triangle
	{
		float x, y, base, height;
	};

	struct Polygon
	{
		Vector2D* vertices;
		size_t vertexCount;
	};

	struct PolygonVec
	{
		std::vector<Vector2D> vertices;
	};
}


class Image
{
public:

	int width = 0;
	int height = 0;

	int channels = 4;
	std::vector<unsigned char> data;

	float alpha = 1.0f;

	Image() = default;

	Image(Image&& other) noexcept;

	Image& operator=(Image&& other) noexcept
	{
		if (this != &other)
		{
			// Clean up current image if needed
			if (backendHandle && backendCleanup)
				backendCleanup(backendHandle);

			width = other.width;
			height = other.height;
			channels = other.channels;
			data = std::move(other.data);
			alpha = other.alpha;

			backendHandle = other.backendHandle;
			backendCleanup = std::move(other.backendCleanup);

			// Nullify other's ownership
			other.backendHandle = nullptr;
			other.backendCleanup = nullptr;
		}
		return *this;
	}

	Image(int w, int h, int ch) : width(w), height(h), channels(ch), data(w* h* ch), alpha(1.0f) {}

	~Image();

	void* backendHandle = nullptr;

	std::function<void(void*&)> backendCleanup;

	inline bool isValid() const
	{
		return !data.empty() && width > 0 && height > 0;
	}

	inline unsigned char* getPixel(int x, int y)
	{
		if (x < 0 || y < 0 || x >= width || y >= height) return nullptr;
		return &data[(y * width + x) * channels];
	}
};




template<typename T1, typename T2>
constexpr auto m_max(const T1& a, const T2& b)
{
	using CommonType = typename std::common_type<T1, T2>::type;
	return (static_cast<CommonType>(a) > static_cast<CommonType>(b)) ? static_cast<CommonType>(a) : static_cast<CommonType>(b);
}

template<typename T1, typename T2>
constexpr auto m_min(const T1& a, const T2& b)
{
	using CommonType = typename std::common_type<T1, T2>::type;
	return (static_cast<CommonType>(a) < static_cast<CommonType>(b)) ? static_cast<CommonType>(a) : static_cast<CommonType>(b);
}


using ImageList = std::vector<Image*>;


enum class Mood
{
	HAPPY,
	VERYHAPPY,
	ANGRY,
	VERYANGRY,
	EMBARRASSED,
	VERYEMBARRASSED,
	ANNOYED,
	VERYANNOYED,
	SURPRISED,
	NEUTRAL
};

enum class Variant
{
	BASE,
	SWIMSUIT,
	NIGHTWEAR,
	DRESS
};

struct MoodImages
{
	std::map<Mood, ImageList> imagesByMood;

	Image* GetImageForMood(Mood mood)
	{
		auto it = imagesByMood.find(mood);

		if (it != imagesByMood.end() && !it->second.empty())
		{
			static bool seeded = false;

			if (!seeded)
			{
				std::srand(static_cast<unsigned>(std::time(0)));
				seeded = true;
			}

			ImageList& imgList = it->second;

			int randomIndex = std::rand() % imgList.size();

			return imgList[randomIndex];
		}

		return nullptr;
	}
};

std::string ToUpper(const std::string& input);

std::string ExtractPrefix(const std::string& input);