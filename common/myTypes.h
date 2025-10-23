#pragma once
#include <vector>
#include <functional>
#include <string>
#include <map>
#include <ranges>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include<iostream>

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
	VERY_HAPPY,
	ANGRY,
	VERY_ANGRY,
	EMBARRASSED,
	VERY_EMBARRASSED,
	ANNOYED,
	VERY_ANNOYED,
	SURPRISED,
	NEUTRAL,
	IDLE
};

enum class Variant
{
	BASE,
	SWIMSUIT,
	NIGHTWEAR,
	DRESS
};

enum class Place
{
	HEAD,
	CHEST,
	BELLY,
	THIGHS,
	LEGS,
	NONE
};

using DialogList = std::vector<std::string>;

struct DialogManager
{
	std::map<std::pair<Mood, Place>, DialogList> dialogsByMoodAndPlace;
	std::map<Mood, DialogList> globalDialogsByMood;

	bool isGlobalDialog;

	std::string GetRandDialogForMoodAndPlace(Mood mood, Place place)
	{
		static bool dSeeded = false;

		if (!dSeeded)
		{
			std::srand(static_cast<unsigned>(std::time(0)));
			dSeeded = true;
		}

		bool pickGlobalDialog = (std::rand() % 2 == 0);

		if (pickGlobalDialog)
		{
			isGlobalDialog = true;

			auto globalIt = globalDialogsByMood.find(mood);

			if (globalIt != globalDialogsByMood.end() && !globalIt->second.empty())
			{
				const DialogList& dialogs = globalIt->second;

				int randomIndex = std::rand() % dialogs.size();

				return dialogs[randomIndex];
			}

		}
		else
		{
			isGlobalDialog = false;

			auto it = dialogsByMoodAndPlace.find({ mood,place });

			if (it != dialogsByMoodAndPlace.end() && !it->second.empty())
			{
				const DialogList& dialogs = it->second;

				int randomIndex = std::rand() % dialogs.size();

				return dialogs[randomIndex];
			}
			else
			{
				isGlobalDialog = true;

				auto globalIt = globalDialogsByMood.find(mood);

				if (globalIt != globalDialogsByMood.end() && !globalIt->second.empty())
				{
					const DialogList& dialogs = globalIt->second;

					int randomIndex = std::rand() % dialogs.size();

					return dialogs[randomIndex];
				}
			}
		}

		return "";
	}

	~DialogManager()
	{
	}
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

struct CompanionRegionProportions
{
	float headX = 0.0f;
	float chestX = 0.0f;
	float bellyX = 0.0f;
	float thighsX = 0.0f;
	float legsX = 0.0f;

	float headY = 0.0f;
	float chestY = 0.0f;
	float bellyY = 0.0f;
	float thighsY = 0.0f;
	float legsY = 0.0f;
};

struct CompanionMoodProbabilities
{
	//head moods
	float happy_head_prob;
	float very_happy_head_prob;

	float angry_head_prob;
	float very_angry_head_prob;

	float embarrassed_head_prob;
	float very_embarrassed_head_prob;

	float annoyed_head_prob;
	float very_annoyed_head_prob;

	float surprised_head_prob;

	float neutral_head_prob;

	//chest moods
	float happy_chest_prob;
	float very_happy_chest_prob;

	float angry_chest_prob;
	float very_angry_chest_prob;

	float embarrassed_chest_prob;
	float very_embarrassed_chest_prob;

	float annoyed_chest_prob;
	float very_annoyed_chest_prob;

	float surprised_chest_prob;

	float neutral_chest_prob;

	//belly moods
	float happy_belly_prob;
	float very_happy_belly_prob;

	float angry_belly_prob;
	float very_angry_belly_prob;

	float embarrassed_belly_prob;
	float very_embarrassed_belly_prob;

	float annoyed_belly_prob;
	float very_annoyed_belly_prob;

	float surprised_belly_prob;

	float neutral_belly_prob;

	//thighs moods
	float happy_thighs_prob;
	float very_happy_thighs_prob;

	float angry_thighs_prob;
	float very_angry_thighs_prob;

	float embarrassed_thighs_prob;
	float very_embarrassed_thighs_prob;

	float annoyed_thighs_prob;
	float very_annoyed_thighs_prob;

	float surprised_thighs_prob;

	float neutral_thighs_prob;

	//legs moods
	float happy_legs_prob;
	float very_happy_legs_prob;

	float angry_legs_prob;
	float very_angry_legs_prob;

	float embarrassed_legs_prob;
	float very_embarrassed_legs_prob;

	float annoyed_legs_prob;
	float very_annoyed_legs_prob;

	float surprised_legs_prob;

	float neutral_legs_prob;
};

struct ConfigFile
{

	//companion mood probabilities

	CompanionMoodProbabilities base;

	CompanionMoodProbabilities night;

	CompanionMoodProbabilities dress;

	CompanionMoodProbabilities swim;

	//companion proportions & offsets 

	CompanionRegionProportions base_proportions;

	CompanionRegionProportions night_proportions;

	CompanionRegionProportions dress_proportions;

	CompanionRegionProportions swim_proportions;


	CompanionRegionProportions base_offset;

	CompanionRegionProportions night_offset;

	CompanionRegionProportions dress_offset;

	CompanionRegionProportions swim_offset;

	//main.cpp offsets

	//base
	float base_clRect_offset_x;
	float base_clRect_offset_y;

	float base_clRect_scale_x;
	float base_clRect_scale_y;

	//nightwear
	float night_clRect_offset_x;
	float night_clRect_offset_y;

	float night_clRect_scale_x;
	float night_clRect_scale_y;

	//dress
	float dress_clRect_offset_x;
	float dress_clRect_offset_y;

	float dress_clRect_scale_x;
	float dress_clRect_scale_y;

	//swimsuit
	float swim_clRect_offset_x;
	float swim_clRect_offset_y;

	float swim_clRect_scale_x;
	float swim_clRect_scale_y;

	//debug
	bool debug_draw;
};

ConfigFile LoadConfigFile(std::string path);