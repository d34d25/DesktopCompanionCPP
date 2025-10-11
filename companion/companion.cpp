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

	Initialize(startVariant);
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
}

void Companion::ChangeVariant(Variant variant)
{
	if (variant == currentVariant) return;

	Initialize(variant);
}

void Companion::ChangeMood(Mood mood, float duration)
{
	if (mood != currentMood)
	{
		currentMood = mood;
		currentImage = variants[currentVariant].GetImageForMood(currentMood);
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

