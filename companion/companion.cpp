#include "companion.h"
#include <iostream>
#include <filesystem>
#include <fstream>

const std::map<std::string, Mood> moodMap = {

	{"HAPPY", Mood::HAPPY},
	{"VERY_HAPPY", Mood::VERY_HAPPY},

	{"ANGRY", Mood::ANGRY},
	{"VERY_ANGRY", Mood::VERY_ANGRY},

	{"EMBARRASSED", Mood::EMBARRASSED},
	{"VERY_EMBARRASSED", Mood::VERY_EMBARRASSED},

	{"ANNOYED", Mood::ANNOYED},
	{"VERY_ANNOYED", Mood::VERY_ANNOYED},

	{"SURPRISED", Mood::SURPRISED},

	{"NEUTRAL", Mood::NEUTRAL},

	{"IDLE", Mood::IDLE}
};

const std::map<std::string, std::pair<Mood, Place>> dialogMap = {

	{"HAPPY_HEAD", {Mood::HAPPY, Place::HEAD} },
	{"HAPPY_CHEST", {Mood::HAPPY, Place::CHEST} },
	{"HAPPY_BELLY", {Mood::HAPPY, Place::BELLY} },
	{"HAPPY_THIGHS", {Mood::HAPPY, Place::THIGHS} },
	{"HAPPY_LEGS", {Mood::HAPPY, Place::LEGS} },
	{"HAPPY_NONE", {Mood::HAPPY, Place::NONE} },

	{"VERY_HAPPY_HEAD", {Mood::VERY_HAPPY, Place::HEAD} },
	{"VERY_HAPPY_CHEST", {Mood::VERY_HAPPY, Place::CHEST} },
	{"VERY_HAPPY_BELLY", {Mood::VERY_HAPPY, Place::BELLY} },
	{"VERY_HAPPY_THIGHS", {Mood::VERY_HAPPY, Place::THIGHS} },
	{"VERY_HAPPY_LEGS", {Mood::VERY_HAPPY, Place::LEGS} },
	{"VERY_HAPPY_NONE", {Mood::VERY_HAPPY, Place::NONE} },

	{"ANGRY_HEAD",{Mood::ANGRY, Place::HEAD}},
	{"ANGRY_CHEST",{Mood::ANGRY, Place::CHEST}},
	{"ANGRY_BELLY",{Mood::ANGRY, Place::BELLY}},
	{"ANGRY_THIGHS",{Mood::ANGRY, Place::THIGHS}},
	{"ANGRY_LEGS",{Mood::ANGRY, Place::LEGS}},
	{"ANGRY_NONE",{Mood::ANGRY, Place::NONE}},

	{"VERY_ANGRY_HEAD",{Mood::VERY_ANGRY, Place::HEAD}},
	{"VERY_ANGRY_CHEST",{Mood::VERY_ANGRY, Place::CHEST}},
	{"VERY_ANGRY_BELLY",{Mood::VERY_ANGRY, Place::BELLY}},
	{"VERY_ANGRY_THIGHS",{Mood::VERY_ANGRY, Place::THIGHS}},
	{"VERY_ANGRY_LEGS",{Mood::VERY_ANGRY, Place::LEGS}},
	{"VERY_ANGRY_NONE",{Mood::VERY_ANGRY, Place::NONE}},

	{"EMBARRASSED_HEAD",{Mood::EMBARRASSED, Place::HEAD}},
	{"EMBARRASSED_CHEST",{Mood::EMBARRASSED, Place::CHEST}},
	{"EMBARRASSED_BELLY",{Mood::EMBARRASSED, Place::BELLY}},
	{"EMBARRASSED_THIGHS",{Mood::EMBARRASSED, Place::THIGHS}},
	{"EMBARRASSED_LEGS",{Mood::EMBARRASSED, Place::LEGS}},
	{"EMBARRASSED_NONE",{Mood::EMBARRASSED, Place::NONE}},

	{"VERY_EMBARRASSED_HEAD",{Mood::VERY_EMBARRASSED, Place::HEAD}},
	{"VERY_EMBARRASSED_CHEST",{Mood::VERY_EMBARRASSED, Place::CHEST}},
	{"VERY_EMBARRASSED_BELLY",{Mood::VERY_EMBARRASSED, Place::BELLY}},
	{"VERY_EMBARRASSED_THIGHS",{Mood::VERY_EMBARRASSED, Place::THIGHS}},
	{"VERY_EMBARRASSED_LEGS",{Mood::VERY_EMBARRASSED, Place::LEGS}},
	{"VERY_EMBARRASSED_NONE",{Mood::VERY_EMBARRASSED, Place::NONE}},

	{"ANNOYED_HEAD",{Mood::ANNOYED, Place::HEAD}},
	{"ANNOYED_CHEST",{Mood::ANNOYED, Place::CHEST}},
	{"ANNOYED_BELLY",{Mood::ANNOYED, Place::BELLY}},
	{"ANNOYED_THIGHS",{Mood::ANNOYED, Place::THIGHS}},
	{"ANNOYED_LEGS",{Mood::ANNOYED, Place::LEGS}},
	{"ANNOYED_NONE",{Mood::ANNOYED, Place::NONE}},

	{"VERY_ANNOYED_HEAD",{Mood::VERY_ANNOYED, Place::HEAD}},
	{"VERY_ANNOYED_CHEST",{Mood::VERY_ANNOYED, Place::CHEST}},
	{"VERY_ANNOYED_BELLY",{Mood::VERY_ANNOYED, Place::BELLY}},
	{"VERY_ANNOYED_THIGHS",{Mood::VERY_ANNOYED, Place::THIGHS}},
	{"VERY_ANNOYED_LEGS",{Mood::VERY_ANNOYED, Place::LEGS}},
	{"VERY_ANNOYED_NONE",{Mood::VERY_ANNOYED, Place::NONE}},


	{"SURPRISED_HEAD",{Mood::SURPRISED, Place::HEAD}},
	{"SURPRISED_CHEST",{Mood::SURPRISED, Place::CHEST}},
	{"SURPRISED_BELLY",{Mood::SURPRISED, Place::BELLY}},
	{"SURPRISED_THIGHS",{Mood::SURPRISED, Place::THIGHS}},
	{"SURPRISED_LEGS",{Mood::SURPRISED, Place::LEGS}},
	{"SURPRISED_NONE",{Mood::SURPRISED, Place::NONE}},

	{"NEUTRAL_HEAD",{Mood::NEUTRAL, Place::HEAD}},
	{"NEUTRAL_CHEST",{Mood::NEUTRAL, Place::CHEST}},
	{"NEUTRAL_BELLY",{Mood::NEUTRAL, Place::BELLY}},
	{"NEUTRAL_THIGHS",{Mood::NEUTRAL, Place::THIGHS}},
	{"NEUTRAL_LEGS",{Mood::NEUTRAL, Place::LEGS}},
	{"NEUTRAL_NONE",{Mood::NEUTRAL, Place::NONE}},
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

Companion::Companion(NewCanvasX* canvas, Variant startVariant, ConfigFile* config)
{
	this->canvas = canvas;
	currentVariant = startVariant;
	currentMood = defaultMood;
	currentPlace = defaultPlace;

	configFile = config;

	Initialize(startVariant);
}


void Companion::UpdateRegions(const Shapes::Rectangle& refRect)
{

	switch (this->GetCurrentVariant())
	{
	case Variant::BASE:
		proportion.headX = configFile->base_proportions.headX;
		proportion.chestX = configFile->base_proportions.chestX;
		proportion.bellyX = configFile->base_proportions.bellyX;
		proportion.thighsX = configFile->base_proportions.thighsX;
		proportion.legsX = configFile->base_proportions.legsX;

		offset.headX = configFile->base_offset.headX;
		offset.chestX = configFile->base_offset.chestX;
		offset.bellyX = configFile->base_offset.bellyX;
		offset.thighsX = configFile->base_offset.thighsX;
		offset.legsX = configFile->base_offset.legsX;

		proportion.headY = configFile->base_proportions.headY;
		proportion.chestY = configFile->base_proportions.chestY;
		proportion.bellyY = configFile->base_proportions.bellyY;
		proportion.thighsY = configFile->base_proportions.thighsY;
		proportion.legsY = configFile->base_proportions.legsY;

		offset.headY = configFile->base_offset.headY;
		offset.chestY = configFile->base_offset.chestY;
		offset.bellyY = configFile->base_offset.bellyY;
		offset.thighsY = configFile->base_offset.thighsY;
		offset.legsY = configFile->base_offset.legsY;

		break;
	case Variant::NIGHTWEAR:

		proportion.headX = configFile->night_proportions.headX;
		proportion.chestX = configFile->night_proportions.chestX;
		proportion.bellyX = configFile->night_proportions.bellyX;
		proportion.thighsX = configFile->night_proportions.thighsX;
		proportion.legsX = configFile->night_proportions.legsX;

		offset.headX = configFile->night_offset.headX;
		offset.chestX = configFile->night_offset.chestX;
		offset.bellyX = configFile->night_offset.bellyX;
		offset.thighsX = configFile->night_offset.thighsX;
		offset.legsX = configFile->night_offset.legsX;

		proportion.headY = configFile->night_proportions.headY;
		proportion.chestY = configFile->night_proportions.chestY;
		proportion.bellyY = configFile->night_proportions.bellyY;
		proportion.thighsY = configFile->night_proportions.thighsY;
		proportion.legsY = configFile->night_proportions.legsY;

		offset.headY = configFile->night_offset.headY;
		offset.chestY = configFile->night_offset.chestY;
		offset.bellyY = configFile->night_offset.bellyY;
		offset.thighsY = configFile->night_offset.thighsY;
		offset.legsY = configFile->night_offset.legsY;

		break;
	case Variant::DRESS:

		proportion.headX = configFile->dress_proportions.headX;
		proportion.chestX = configFile->dress_proportions.chestX;
		proportion.bellyX = configFile->dress_proportions.bellyX;
		proportion.thighsX = configFile->dress_proportions.thighsX;
		proportion.legsX = configFile->dress_proportions.legsX;

		offset.headX = configFile->dress_offset.headX;
		offset.chestX = configFile->dress_offset.chestX;
		offset.bellyX = configFile->dress_offset.bellyX;
		offset.thighsX = configFile->dress_offset.thighsX;
		offset.legsX = configFile->dress_offset.legsX;

		proportion.headY = configFile->dress_proportions.headY;
		proportion.chestY = configFile->dress_proportions.chestY;
		proportion.bellyY = configFile->dress_proportions.bellyY;
		proportion.thighsY = configFile->dress_proportions.thighsY;
		proportion.legsY = configFile->dress_proportions.legsY;

		offset.headY = configFile->dress_offset.headY;
		offset.chestY = configFile->dress_offset.chestY;
		offset.bellyY = configFile->dress_offset.bellyY;
		offset.thighsY = configFile->dress_offset.thighsY;
		offset.legsY = configFile->dress_offset.legsY;

		break;
	case Variant::SWIMSUIT:

		proportion.headX = configFile->swim_proportions.headX;
		proportion.chestX = configFile->swim_proportions.chestX;
		proportion.bellyX = configFile->swim_proportions.bellyX;
		proportion.thighsX = configFile->swim_proportions.thighsX;
		proportion.legsX = configFile->swim_proportions.legsX;

		offset.headX = configFile->swim_offset.headX;
		offset.chestX = configFile->swim_offset.chestX;
		offset.bellyX = configFile->swim_offset.bellyX;
		offset.thighsX = configFile->swim_offset.thighsX;
		offset.legsX = configFile->swim_offset.legsX;

		proportion.headY = configFile->swim_proportions.headY;
		proportion.chestY = configFile->swim_proportions.chestY;
		proportion.bellyY = configFile->swim_proportions.bellyY;
		proportion.thighsY = configFile->swim_proportions.thighsY;
		proportion.legsY = configFile->swim_proportions.legsY;

		offset.headY = configFile->swim_offset.headY;
		offset.chestY = configFile->swim_offset.chestY;
		offset.bellyY = configFile->swim_offset.bellyY;
		offset.thighsY = configFile->swim_offset.thighsY;
		offset.legsY = configFile->swim_offset.legsY;

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
	y + offset.headY,
	headWidth,
	headHeight
	};

	regions.chest = {
		x + (width - chestWidth) * 0.5f + offset.chestX,
		y + headHeight + offset.chestY,
		chestWidth,
		chestHeight
	};

	regions.belly = {
		x + (width - bellyWidth) * 0.5f + offset.bellyX,
		y + headHeight + chestHeight + offset.bellyY,
		bellyWidth,
		bellyHeight
	};

	regions.thighs = {
		x + (width - thighsWidth) * 0.5f + offset.thighsX,
		y + headHeight + chestHeight + bellyHeight + offset.thighsY,
		thighsWidth,
		thighsHeight
	};

	regions.legs = {
		x + (width - legsWidth) * 0.5f + offset.legsX,
		y + headHeight + chestHeight + bellyHeight + thighsHeight + offset.legsY,
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

		this->loadedImages.push_back(LoadedImage{ std::move(img), filename });
	}

	for (auto& loadedImg : this->loadedImages)
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

DialogManager Companion::LoadDialogsFromFolder(const std::string& folderPath)
{
	DialogManager dialogManager;

	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (!entry.is_regular_file()) continue;

		if(entry.path().extension() != ".txt") continue;

		std::string filename = entry.path().filename().string();

		std::string prefix = ExtractPrefix(filename);
		std::string key = ToUpper(prefix);

		std::ifstream file(entry.path());

		if (!file.is_open())
		{
			continue;
		}

		std::vector<std::string> lines;
		std::string line;

		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				lines.push_back(line);
			}
		}

		auto dialogIt = dialogMap.find(key);

		if (dialogIt != dialogMap.end())
		{
			const auto& moodPlace = dialogIt->second;

			dialogManager.dialogsByMoodAndPlace[moodPlace].insert(
				dialogManager.dialogsByMoodAndPlace[moodPlace].end(),
				lines.begin(),
				lines.end()
			);

			std::cout << "Loaded " << lines.size() << " dialogs for " << key << "\n";
			continue;
		}


		auto moodIt = moodMap.find(key);
		if (moodIt != moodMap.end())
		{
			Mood mood = moodIt->second;

			dialogManager.globalDialogsByMood[mood].insert(
				dialogManager.globalDialogsByMood[mood].end(),
				lines.begin(), 
				lines.end()
			);

			std::cout << "Loaded " << lines.size() << " dialogs for mood " << key << "\n";
			continue;
		}

		std::cout << "No mapping found for prefix: " << key << " in file " << filename << "\n";
	}



	return dialogManager;
}

void Companion::Initialize(Variant variant)
{
	currentVariant = variant;

	switch (currentVariant)
	{
	case Variant::BASE:

		headMoods = {
			{Mood::HAPPY, configFile->base.happy_head_prob},
			{Mood::VERY_HAPPY,configFile->base.very_happy_head_prob},
			{Mood::ANGRY,configFile->base.angry_head_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_head_prob},
			{Mood::EMBARRASSED,configFile->base.embarrassed_head_prob},
			{Mood::VERY_EMBARRASSED,configFile->base.very_embarrassed_head_prob},
			{Mood::ANNOYED, configFile->base.annoyed_head_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_head_prob},
			{Mood::SURPRISED, configFile->base.surprised_head_prob},
			{Mood::NEUTRAL, configFile->base.neutral_head_prob}
		};

		chestMoods = {
			{Mood::HAPPY, configFile->base.happy_chest_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_chest_prob},
			{Mood::ANGRY, configFile->base.angry_chest_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_chest_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_chest_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_chest_prob},
			{Mood::ANNOYED, configFile->base.annoyed_chest_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_chest_prob},
			{Mood::SURPRISED, configFile->base.surprised_chest_prob},
			{Mood::NEUTRAL, configFile->base.neutral_chest_prob}
		};

		bellyMoods = {
			{Mood::HAPPY, configFile->base.happy_belly_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_belly_prob},
			{Mood::ANGRY, configFile->base.angry_belly_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_belly_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_belly_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_belly_prob},
			{Mood::ANNOYED, configFile->base.annoyed_belly_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_belly_prob},
			{Mood::SURPRISED, configFile->base.surprised_belly_prob},
			{Mood::NEUTRAL, configFile->base.neutral_belly_prob}
		};

		thighsMoods = {
			{Mood::HAPPY, configFile->base.happy_thighs_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_thighs_prob},
			{Mood::ANGRY, configFile->base.angry_thighs_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_thighs_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_thighs_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_thighs_prob},
			{Mood::ANNOYED, configFile->base.annoyed_thighs_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_thighs_prob},
			{Mood::SURPRISED, configFile->base.surprised_thighs_prob},
			{Mood::NEUTRAL, configFile->base.neutral_thighs_prob}
		};


		legMoods = {
			{Mood::HAPPY, configFile->base.happy_legs_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_legs_prob},
			{Mood::ANGRY, configFile->base.angry_legs_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_legs_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_legs_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_legs_prob},
			{Mood::ANNOYED, configFile->base.annoyed_legs_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_legs_prob},
			{Mood::SURPRISED, configFile->base.surprised_legs_prob},
			{Mood::NEUTRAL, configFile->base.neutral_legs_prob}
		};

		break;

	case Variant::NIGHTWEAR:

		headMoods = {
			{Mood::HAPPY, configFile->night.happy_head_prob},
			{Mood::VERY_HAPPY,configFile->night.very_happy_head_prob},
			{Mood::ANGRY,configFile->night.angry_head_prob},
			{Mood::VERY_ANGRY, configFile->night.very_angry_head_prob},
			{Mood::EMBARRASSED,configFile->night.embarrassed_head_prob},
			{Mood::VERY_EMBARRASSED,configFile->night.very_embarrassed_head_prob},
			{Mood::ANNOYED, configFile->night.annoyed_head_prob},
			{Mood::VERY_ANNOYED, configFile->night.very_annoyed_head_prob},
			{Mood::SURPRISED, configFile->night.surprised_head_prob},
			{Mood::NEUTRAL, configFile->night.neutral_head_prob}
		};

		chestMoods = {
			{Mood::HAPPY, configFile->night.happy_chest_prob},
			{Mood::VERY_HAPPY, configFile->night.very_happy_chest_prob},
			{Mood::ANGRY, configFile->night.angry_chest_prob},
			{Mood::VERY_ANGRY, configFile->night.very_angry_chest_prob},
			{Mood::EMBARRASSED, configFile->night.embarrassed_chest_prob},
			{Mood::VERY_EMBARRASSED, configFile->night.very_embarrassed_chest_prob},
			{Mood::ANNOYED, configFile->night.annoyed_chest_prob},
			{Mood::VERY_ANNOYED, configFile->night.very_annoyed_chest_prob},
			{Mood::SURPRISED, configFile->night.surprised_chest_prob},
			{Mood::NEUTRAL, configFile->night.neutral_chest_prob}
		};

		bellyMoods = {
			{Mood::HAPPY, configFile->night.happy_belly_prob},
			{Mood::VERY_HAPPY, configFile->night.very_happy_belly_prob},
			{Mood::ANGRY, configFile->night.angry_belly_prob},
			{Mood::VERY_ANGRY, configFile->night.very_angry_belly_prob},
			{Mood::EMBARRASSED, configFile->night.embarrassed_belly_prob},
			{Mood::VERY_EMBARRASSED, configFile->night.very_embarrassed_belly_prob},
			{Mood::ANNOYED, configFile->night.annoyed_belly_prob},
			{Mood::VERY_ANNOYED, configFile->night.very_annoyed_belly_prob},
			{Mood::SURPRISED, configFile->night.surprised_belly_prob},
			{Mood::NEUTRAL, configFile->night.neutral_belly_prob}
		};

		thighsMoods = {
			{Mood::HAPPY, configFile->night.happy_thighs_prob},
			{Mood::VERY_HAPPY, configFile->night.very_happy_thighs_prob},
			{Mood::ANGRY, configFile->night.angry_thighs_prob},
			{Mood::VERY_ANGRY, configFile->night.very_angry_thighs_prob},
			{Mood::EMBARRASSED, configFile->night.embarrassed_thighs_prob},
			{Mood::VERY_EMBARRASSED, configFile->night.very_embarrassed_thighs_prob},
			{Mood::ANNOYED, configFile->night.annoyed_thighs_prob},
			{Mood::VERY_ANNOYED, configFile->night.very_annoyed_thighs_prob},
			{Mood::SURPRISED, configFile->night.surprised_thighs_prob},
			{Mood::NEUTRAL, configFile->night.neutral_thighs_prob}
		};


		legMoods = {
			{Mood::HAPPY, configFile->night.happy_legs_prob},
			{Mood::VERY_HAPPY, configFile->night.very_happy_legs_prob},
			{Mood::ANGRY, configFile->night.angry_legs_prob},
			{Mood::VERY_ANGRY, configFile->night.very_angry_legs_prob},
			{Mood::EMBARRASSED, configFile->night.embarrassed_legs_prob},
			{Mood::VERY_EMBARRASSED, configFile->night.very_embarrassed_legs_prob},
			{Mood::ANNOYED, configFile->night.annoyed_legs_prob},
			{Mood::VERY_ANNOYED, configFile->night.very_annoyed_legs_prob},
			{Mood::SURPRISED, configFile->night.surprised_legs_prob},
			{Mood::NEUTRAL, configFile->night.neutral_legs_prob}
		};

		break;

	case Variant::DRESS:

		headMoods = {
			{Mood::HAPPY, configFile->dress.happy_head_prob},
			{Mood::VERY_HAPPY,configFile->dress.very_happy_head_prob},
			{Mood::ANGRY,configFile->dress.angry_head_prob},
			{Mood::VERY_ANGRY, configFile->dress.very_angry_head_prob},
			{Mood::EMBARRASSED,configFile->dress.embarrassed_head_prob},
			{Mood::VERY_EMBARRASSED,configFile->dress.very_embarrassed_head_prob},
			{Mood::ANNOYED, configFile->dress.annoyed_head_prob},
			{Mood::VERY_ANNOYED, configFile->dress.very_annoyed_head_prob},
			{Mood::SURPRISED, configFile->dress.surprised_head_prob},
			{Mood::NEUTRAL, configFile->dress.neutral_head_prob}
		};

		chestMoods = {
			{Mood::HAPPY, configFile->dress.happy_chest_prob},
			{Mood::VERY_HAPPY, configFile->dress.very_happy_chest_prob},
			{Mood::ANGRY, configFile->dress.angry_chest_prob},
			{Mood::VERY_ANGRY, configFile->dress.very_angry_chest_prob},
			{Mood::EMBARRASSED, configFile->dress.embarrassed_chest_prob},
			{Mood::VERY_EMBARRASSED, configFile->dress.very_embarrassed_chest_prob},
			{Mood::ANNOYED, configFile->dress.annoyed_chest_prob},
			{Mood::VERY_ANNOYED, configFile->dress.very_annoyed_chest_prob},
			{Mood::SURPRISED, configFile->dress.surprised_chest_prob},
			{Mood::NEUTRAL, configFile->dress.neutral_chest_prob}
		};

		bellyMoods = {
			{Mood::HAPPY, configFile->dress.happy_belly_prob},
			{Mood::VERY_HAPPY, configFile->dress.very_happy_belly_prob},
			{Mood::ANGRY, configFile->dress.angry_belly_prob},
			{Mood::VERY_ANGRY, configFile->dress.very_angry_belly_prob},
			{Mood::EMBARRASSED, configFile->dress.embarrassed_belly_prob},
			{Mood::VERY_EMBARRASSED, configFile->dress.very_embarrassed_belly_prob},
			{Mood::ANNOYED, configFile->dress.annoyed_belly_prob},
			{Mood::VERY_ANNOYED, configFile->dress.very_annoyed_belly_prob},
			{Mood::SURPRISED, configFile->dress.surprised_belly_prob},
			{Mood::NEUTRAL, configFile->dress.neutral_belly_prob}
		};

		thighsMoods = {
			{Mood::HAPPY, configFile->dress.happy_thighs_prob},
			{Mood::VERY_HAPPY, configFile->dress.very_happy_thighs_prob},
			{Mood::ANGRY, configFile->dress.angry_thighs_prob},
			{Mood::VERY_ANGRY, configFile->dress.very_angry_thighs_prob},
			{Mood::EMBARRASSED, configFile->dress.embarrassed_thighs_prob},
			{Mood::VERY_EMBARRASSED, configFile->dress.very_embarrassed_thighs_prob},
			{Mood::ANNOYED, configFile->dress.annoyed_thighs_prob},
			{Mood::VERY_ANNOYED, configFile->dress.very_annoyed_thighs_prob},
			{Mood::SURPRISED, configFile->dress.surprised_thighs_prob},
			{Mood::NEUTRAL, configFile->dress.neutral_thighs_prob}
		};


		legMoods = {
			{Mood::HAPPY, configFile->dress.happy_legs_prob},
			{Mood::VERY_HAPPY, configFile->dress.very_happy_legs_prob},
			{Mood::ANGRY, configFile->dress.angry_legs_prob},
			{Mood::VERY_ANGRY, configFile->dress.very_angry_legs_prob},
			{Mood::EMBARRASSED, configFile->dress.embarrassed_legs_prob},
			{Mood::VERY_EMBARRASSED, configFile->dress.very_embarrassed_legs_prob},
			{Mood::ANNOYED, configFile->dress.annoyed_legs_prob},
			{Mood::VERY_ANNOYED, configFile->dress.very_annoyed_legs_prob},
			{Mood::SURPRISED, configFile->dress.surprised_legs_prob},
			{Mood::NEUTRAL, configFile->dress.neutral_legs_prob}
		};

		break;

	case Variant::SWIMSUIT:

		headMoods = {
			{Mood::HAPPY, configFile->swim.happy_head_prob},
			{Mood::VERY_HAPPY,configFile->swim.very_happy_head_prob},
			{Mood::ANGRY,configFile->swim.angry_head_prob},
			{Mood::VERY_ANGRY, configFile->swim.very_angry_head_prob},
			{Mood::EMBARRASSED,configFile->swim.embarrassed_head_prob},
			{Mood::VERY_EMBARRASSED,configFile->swim.very_embarrassed_head_prob},
			{Mood::ANNOYED, configFile->swim.annoyed_head_prob},
			{Mood::VERY_ANNOYED, configFile->swim.very_annoyed_head_prob},
			{Mood::SURPRISED, configFile->swim.surprised_head_prob},
			{Mood::NEUTRAL, configFile->swim.neutral_head_prob}
		};

		chestMoods = {
			{Mood::HAPPY, configFile->swim.happy_chest_prob},
			{Mood::VERY_HAPPY, configFile->swim.very_happy_chest_prob},
			{Mood::ANGRY, configFile->swim.angry_chest_prob},
			{Mood::VERY_ANGRY, configFile->swim.very_angry_chest_prob},
			{Mood::EMBARRASSED, configFile->swim.embarrassed_chest_prob},
			{Mood::VERY_EMBARRASSED, configFile->swim.very_embarrassed_chest_prob},
			{Mood::ANNOYED, configFile->swim.annoyed_chest_prob},
			{Mood::VERY_ANNOYED, configFile->swim.very_annoyed_chest_prob},
			{Mood::SURPRISED, configFile->swim.surprised_chest_prob},
			{Mood::NEUTRAL, configFile->swim.neutral_chest_prob}
		};

		bellyMoods = {
			{Mood::HAPPY, configFile->swim.happy_belly_prob},
			{Mood::VERY_HAPPY, configFile->swim.very_happy_belly_prob},
			{Mood::ANGRY, configFile->swim.angry_belly_prob},
			{Mood::VERY_ANGRY, configFile->swim.very_angry_belly_prob},
			{Mood::EMBARRASSED, configFile->swim.embarrassed_belly_prob},
			{Mood::VERY_EMBARRASSED, configFile->swim.very_embarrassed_belly_prob},
			{Mood::ANNOYED, configFile->swim.annoyed_belly_prob},
			{Mood::VERY_ANNOYED, configFile->swim.very_annoyed_belly_prob},
			{Mood::SURPRISED, configFile->swim.surprised_belly_prob},
			{Mood::NEUTRAL, configFile->swim.neutral_belly_prob}
		};

		thighsMoods = {
			{Mood::HAPPY, configFile->swim.happy_thighs_prob},
			{Mood::VERY_HAPPY, configFile->swim.very_happy_thighs_prob},
			{Mood::ANGRY, configFile->swim.angry_thighs_prob},
			{Mood::VERY_ANGRY, configFile->swim.very_angry_thighs_prob},
			{Mood::EMBARRASSED, configFile->swim.embarrassed_thighs_prob},
			{Mood::VERY_EMBARRASSED, configFile->swim.very_embarrassed_thighs_prob},
			{Mood::ANNOYED, configFile->swim.annoyed_thighs_prob},
			{Mood::VERY_ANNOYED, configFile->swim.very_annoyed_thighs_prob},
			{Mood::SURPRISED, configFile->swim.surprised_thighs_prob},
			{Mood::NEUTRAL, configFile->swim.neutral_thighs_prob}
		};


		legMoods = {
			{Mood::HAPPY, configFile->swim.happy_legs_prob},
			{Mood::VERY_HAPPY, configFile->swim.very_happy_legs_prob},
			{Mood::ANGRY, configFile->swim.angry_legs_prob},
			{Mood::VERY_ANGRY, configFile->swim.very_angry_legs_prob},
			{Mood::EMBARRASSED, configFile->swim.embarrassed_legs_prob},
			{Mood::VERY_EMBARRASSED, configFile->swim.very_embarrassed_legs_prob},
			{Mood::ANNOYED, configFile->swim.annoyed_legs_prob},
			{Mood::VERY_ANNOYED, configFile->swim.very_annoyed_legs_prob},
			{Mood::SURPRISED, configFile->swim.surprised_legs_prob},
			{Mood::NEUTRAL, configFile->swim.neutral_legs_prob}
		};

		break;

	default:
		headMoods = {
		{Mood::HAPPY, configFile->base.happy_head_prob},
		{Mood::VERY_HAPPY,configFile->base.very_happy_head_prob},
		{Mood::ANGRY,configFile->base.angry_head_prob},
		{Mood::VERY_ANGRY, configFile->base.very_angry_head_prob},
		{Mood::EMBARRASSED,configFile->base.embarrassed_head_prob},
		{Mood::VERY_EMBARRASSED,configFile->base.very_embarrassed_head_prob},
		{Mood::ANNOYED, configFile->base.annoyed_head_prob},
		{Mood::VERY_ANNOYED, configFile->base.very_annoyed_head_prob},
		{Mood::SURPRISED, configFile->base.surprised_head_prob},
		{Mood::NEUTRAL, configFile->base.neutral_head_prob}
		};

		chestMoods = {
			{Mood::HAPPY, configFile->base.happy_chest_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_chest_prob},
			{Mood::ANGRY, configFile->base.angry_chest_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_chest_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_chest_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_chest_prob},
			{Mood::ANNOYED, configFile->base.annoyed_chest_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_chest_prob},
			{Mood::SURPRISED, configFile->base.surprised_chest_prob},
			{Mood::NEUTRAL, configFile->base.neutral_chest_prob}
		};

		bellyMoods = {
			{Mood::HAPPY, configFile->base.happy_belly_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_belly_prob},
			{Mood::ANGRY, configFile->base.angry_belly_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_belly_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_belly_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_belly_prob},
			{Mood::ANNOYED, configFile->base.annoyed_belly_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_belly_prob},
			{Mood::SURPRISED, configFile->base.surprised_belly_prob},
			{Mood::NEUTRAL, configFile->base.neutral_belly_prob}
		};

		thighsMoods = {
			{Mood::HAPPY, configFile->base.happy_thighs_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_thighs_prob},
			{Mood::ANGRY, configFile->base.angry_thighs_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_thighs_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_thighs_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_thighs_prob},
			{Mood::ANNOYED, configFile->base.annoyed_thighs_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_thighs_prob},
			{Mood::SURPRISED, configFile->base.surprised_thighs_prob},
			{Mood::NEUTRAL, configFile->base.neutral_thighs_prob}
		};


		legMoods = {
			{Mood::HAPPY, configFile->base.happy_legs_prob},
			{Mood::VERY_HAPPY, configFile->base.very_happy_legs_prob},
			{Mood::ANGRY, configFile->base.angry_legs_prob},
			{Mood::VERY_ANGRY, configFile->base.very_angry_legs_prob},
			{Mood::EMBARRASSED, configFile->base.embarrassed_legs_prob},
			{Mood::VERY_EMBARRASSED, configFile->base.very_embarrassed_legs_prob},
			{Mood::ANNOYED, configFile->base.annoyed_legs_prob},
			{Mood::VERY_ANNOYED, configFile->base.very_annoyed_legs_prob},
			{Mood::SURPRISED, configFile->base.surprised_legs_prob},
			{Mood::NEUTRAL, configFile->base.neutral_legs_prob}
		};
		break;
	}

	loadedImages.clear();

	std::string imgFolderName = VariantToFolderName(currentVariant);

	std::string imgPath = "../assets/images/" + imgFolderName;

	variantsImg[currentVariant] = LoadImagesFromFolder(imgPath);

	auto it = variantsImg[currentVariant].imagesByMood.find(currentMood);

	if (it != variantsImg[currentVariant].imagesByMood.end() && !it->second.empty())
	{
		currentImage = it->second.front();

	}
	else
	{

		currentImage = nullptr;
	}

	auto fallbackIt = variantsImg[currentVariant].imagesByMood.find(Mood::IDLE);
	if (fallbackIt != variantsImg[currentVariant].imagesByMood.end() && !fallbackIt->second.empty())
	{
		fallbackImage = fallbackIt->second.front();
	}
	else
	{
		fallbackImage = nullptr;
	}

	std::string dialogFolderName = VariantToFolderName(currentVariant);
	std::string dialogPath = "../assets/dialogs/" + dialogFolderName;

	variantDialog[currentVariant] = LoadDialogsFromFolder(dialogPath);

}

void Companion::ChangeVariant(Variant variant)
{
	if (variant == currentVariant) return;

	if (currentMood == Mood::IDLE)
	{
		Initialize(variant);
	}
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

void Companion::ChangeMood(Mood mood, Place place, float duration)
{
	if (mood != currentMood && (moodTimer <= 0.0f))
	{
		currentMood = mood;
		currentPlace = place;

		Image* nImage = variantsImg[currentVariant].GetImageForMood(currentMood);

		std::string nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(currentMood, currentPlace);


		if (!nImage)
		{
			currentPlace = Place::NONE;
			currentMood = Mood::NEUTRAL;
			nImage = variantsImg[currentVariant].GetImageForMood(currentMood);

			nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(currentMood, currentPlace);

			if (nDialog == "")
			{
				nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(Mood::NEUTRAL, currentPlace);

				if (nDialog == "")
				{
					nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(Mood::NEUTRAL, defaultPlace);
				}
			}

			currentDialog = nDialog;

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
			currentDialog = nDialog;
			currentImage = nImage;
		}

		std::cout << currentDialog << " " << " is global dialog?: " << variantDialog[currentVariant].isGlobalDialog << "\n";

		moodTimer = duration;
		moodDuration = duration;
	}
}

void Companion::UpdateTimer(Place* place)
{
	if (moodTimer > 0.0f)
	{
		moodTimer -= 0.02f;

		if (moodTimer <= 0.0f)
		{
			currentMood = defaultMood;

			place = &defaultPlace;

			auto it = variantsImg[currentVariant].imagesByMood.find(currentMood);

			if (it != variantsImg[currentVariant].imagesByMood.end() && !it->second.empty())
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

void Companion::DrawDialog(Shapes::Rectangle* refRect, float scale)
{
	if (moodTimer >= 0.0f)
	{
		if (currentDialog != "" || !currentDialog.empty())
		{
			Shapes::Rectangle dialogBox;

			float textSize = currentDialog.size();

			dialogBox.width = 600.0f * scale * scale;
			dialogBox.height = (100.0f + textSize) * scale * scale;

			dialogBox.x = refRect->x + refRect->width * 0.5f - dialogBox.width * 0.5f;
			dialogBox.y = refRect->y - dialogBox.height;//refRect->y + refRect->height - dialogBox.height;

			//top text  dialogBox.y = refRect->y - dialogBox.height;
			//bottom text dialogBox.y = refRect->y + refRect->height - dialogBox.height;

			canvas->DrawRect(dialogBox, { 40,40,255,50 });

			canvas->DrawTxt(currentDialog, dialogBox, { 255,255,255,255 });
		}
	}
}

