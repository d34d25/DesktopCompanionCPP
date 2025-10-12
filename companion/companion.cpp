#include "companion.h"
#include <iostream>
#include <filesystem>

const std::map<std::string, Mood> moodMap = {
	{"HAPPY", Mood::HAPPY},
	{"VERYHAPPY", Mood::VERYHAPPY},
	{"ANGRY", Mood::ANGRY},
	{"VERYANGRY", Mood::VERYANGRY},
	{"EMBARRASSED", Mood::EMBARRASSED},
	{"VERYEMBARRASSED", Mood::VERYEMBARRASSED},
	{"ANNOYED", Mood::ANNOYED},
	{"VERYANNOYED", Mood::VERYANNOYED},
	{"SURPRISED", Mood::SURPRISED},
	{"NEUTRAL", Mood::NEUTRAL},
	{"IDLE", Mood::IDLE}
};

std::string VariantToFolderName(Variant currentVariant)
{
	switch (currentVariant)
	{
		case Variant::BASE: return "base";
		case Variant::NIGHTWEAR: return "nightwear";
		case Variant::SWIMSUIT: return "swimsuit";
		case Variant::DRESS: return "dress";
		default: return "unknown";
	}
}

Companion::Companion(NewCanvasX* canvas, Variant startVariant)
{
	this->canvas = canvas;
	currentVariant = startVariant;
	currentMood = defaultMood;

	headMoods = {
		{Mood::SURPRISED, 0.1f},
		{Mood::HAPPY, 0.3f},
		{Mood::VERYHAPPY, 0.2f},
		{Mood::NEUTRAL, 0.4f}
	};

	chestMoods = {
		{Mood::EMBARRASSED, 0.3f},
		{Mood::VERYEMBARRASSED, 0.3f},
		{Mood::ANGRY, 0.3f},
		{Mood::VERYANGRY, 0.1f}
	};

	bellyMoods = {
		{Mood::NEUTRAL, 0.7f},
		{Mood::EMBARRASSED, 0.3f},
	};

	thighsMoods = {
	{Mood::NEUTRAL, 0.3f},     
	{Mood::EMBARRASSED, 0.3f},
	{Mood::ANNOYED, 0.2f},
	{Mood::ANGRY, 0.1f},
	{Mood::VERYANGRY, 0.05f},
	{Mood::VERYANNOYED, 0.05f}
	};


	legMoods = {
		{Mood::NEUTRAL, 1.0f},
	};

	Initialize(startVariant);
}

void Companion::UpdateRegions(const Shapes::Rectangle& refRect)
{

	switch (this->GetCurrentVariant())
	{
	case Variant::BASE:
		proportion.headX = 0.6f;
		proportion.chestX = 0.52f;
		proportion.bellyX = 0.52f;
		proportion.thighsX = 0.62f;
		proportion.legsX = 0.72f;

		offset.headX = -5.0f;
		offset.chestX = -10.0f;
		offset.bellyX = -10.0f;
		offset.thighsX = -14.0f;
		offset.legsX = -24.0f;

		proportion.headY = 0.22f;
		proportion.chestY = 0.075f;
		proportion.bellyY = 0.08f;
		proportion.thighsY = 0.15f;
		proportion.legsY = 0.475f;
		break;
	case Variant::NIGHTWEAR:

		proportion.headX = 0.62f;
		proportion.chestX = 0.62f;
		proportion.bellyX = 0.62f;
		proportion.thighsX = 0.64f;
		proportion.legsX = 0.62f;

		offset.headX = -15.0f;
		offset.chestX = -15.0f;
		offset.bellyX = -15.0f;
		offset.thighsX = -10.0f;
		offset.legsX = -0.0f;

		proportion.headY = 0.22f;
		proportion.chestY = 0.1f;
		proportion.bellyY = 0.12f;
		proportion.thighsY = 0.15f;
		proportion.legsY = 0.41f;
		break;
	case Variant::DRESS:
		break;
	case Variant::SWIMSUIT:
		break;
	}

	float x = refRect.x;
	float y = refRect.y;
	float width = refRect.width;
	float height = refRect.height;

	float headWidth = width * proportion.headX;
	float chestWidth = width * proportion.chestX;
	float bellyWidth = width * proportion.bellyX;
	float thighsWidth = width * proportion.thighsX;
	float legsWidth = width * proportion.legsX;

	float headHeight = height * proportion.headY;
	float chestHeight = height * proportion.chestY;
	float bellyHeight = height * proportion.bellyY;
	float thighsHeight = height * proportion.thighsY;
	float legsHeight = height * proportion.legsY;


	regions.head = {
	x + (width - headWidth) * 0.5f + offset.headX,
	y,
	headWidth,
	headHeight
	};

	regions.chest = {
		x + (width - chestWidth) * 0.5f + offset.chestX,
		y + headHeight,
		chestWidth,
		chestHeight
	};

	regions.belly = {
		x + (width - bellyWidth) * 0.5f + offset.bellyX,
		y + headHeight + chestHeight,
		bellyWidth,
		bellyHeight
	};

	regions.thighs = {
		x + (width - thighsWidth) * 0.5f + offset.thighsX,
		y + headHeight + chestHeight + bellyHeight,
		thighsWidth,
		thighsHeight
	};

	regions.legs = {
		x + (width - legsWidth) * 0.5f + offset.legsX,
		y + headHeight + chestHeight + bellyHeight + thighsHeight,
		legsWidth,
		legsHeight
	};


}

MoodImages Companion::LoadImagesFromFolder(const std::string& folderPath)
{
	MoodImages moodImages;

	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (!this->canvas) return {};

		if (!entry.is_regular_file()) continue;

		std::string filename = entry.path().filename().string();

		Image img = this->canvas->LoadImg(entry.path().string());

		this->loadedImages.push_back(LoadedImage{std::move(img), filename});
	}

	for(auto& loadedImg : this->loadedImages)
	{
		const std::string& filename = loadedImg.filename;

		std::string prefix = ExtractPrefix(filename);
		std::string moodKey = ToUpper(prefix);

		auto it = moodMap.find(moodKey);

		if (it != moodMap.end())
		{
			Mood mood = it->second;

			moodImages.imagesByMood[mood].push_back(&loadedImg.img);

			std::cout << "assigned image: " << filename << "\n";
		}
		else
		{
			std::cout << "no mood found for image: " << filename << "\n";
		}

	}

	std::cout << "\n";

	std::cout << "MoodImages structure: \n";
	for (const auto& pair : moodImages.imagesByMood)
	{
		std::cout << "Mood: " << static_cast<int>(pair.first) << " has " << pair.second.size() << " images.\n";
	}

	std::cout << "" << std::endl;

	return moodImages;
}

void Companion::Initialize(Variant variant)
{
	currentVariant = variant;

	loadedImages.clear();

	std::string folderName = VariantToFolderName(currentVariant);

	std::string path = "../assets/images/" + folderName;

	variants[currentVariant] = LoadImagesFromFolder(path);

	auto it = variants[currentVariant].imagesByMood.find(currentMood);

	if (it != variants[currentVariant].imagesByMood.end() && !it->second.empty())
	{
		currentImage = it->second.front();
		
	}
	else
	{
		
		currentImage = nullptr;
	}

	auto fallbackIt = variants[currentVariant].imagesByMood.find(Mood::IDLE);
	if (fallbackIt != variants[currentVariant].imagesByMood.end() && !fallbackIt->second.empty())
	{
		fallbackImage = fallbackIt->second.front();
	}
	else
	{
		fallbackImage = nullptr;
	}
}

void Companion::ChangeVariant(Variant variant)
{
	if (variant == currentVariant) return;

	Initialize(variant);
}

Mood Companion::PickMoodByProbabilty(const std::vector<std::pair<Mood, float>>& moodList)
{
	float r = static_cast<float>(rand()) / RAND_MAX;
	float accum = 0.0f;

	for (const auto& [mood, probability] : moodList)
	{
		accum += probability;
		if (r <= accum)
		{
			//std::cout << "returning mood: " << static_cast<int>(mood) << "\n";
			return mood;
		}
	}

	return Mood::NEUTRAL;
}

void Companion::ChangeMood(Mood mood, float duration)
{
	if (mood != currentMood && (moodTimer <= 0.0f))
	{
		currentMood = mood;

		Image* nImage = variants[currentVariant].GetImageForMood(currentMood);

		if (!nImage)
		{
			currentMood = Mood::NEUTRAL;
			nImage = variants[currentVariant].GetImageForMood(currentMood);

			if (!nImage)
			{
				currentImage = fallbackImage;
			}
			else
			{
				currentImage = nImage;
			}	
		}
		else
		{
			currentImage = nImage;
		}

		currentImage = nImage;
		moodTimer = duration;
		moodDuration = duration;
	}
}

void Companion::UpdateTimer()
{
	if (moodTimer > 0.0f)
	{
		moodTimer -= 0.02f;

		if (moodTimer <= 0.0f)
		{
			currentMood = defaultMood;

			auto it = variants[currentVariant].imagesByMood.find(currentMood);

			if (it != variants[currentVariant].imagesByMood.end() && !it->second.empty())
			{
				currentImage = it->second.front();
			}
			else
			{
				currentImage = nullptr;
			}
		}
	}
}

Image* Companion::GetCurrentImage() const
{
	return this->currentImage;
}

Variant Companion::GetCurrentVariant() const
{
	return this->currentVariant;
}

