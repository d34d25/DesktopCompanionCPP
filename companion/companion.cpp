#include "companion.h"
#include <iostream>
#include <filesystem>

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

Companion::Companion(NewCanvasX* canvas, Variant startVariant)
{
	this->canvas = canvas;
	currentVariant = startVariant;
	currentMood = defaultMood;
	currentPlace = defaultPlace;


	headMoods = {
		{Mood::HAPPY, 0.3f},
		{Mood::VERY_HAPPY, 0.2f},
		{Mood::ANGRY,0.0f},
		{Mood::VERY_ANGRY, 0.0f},
		{Mood::EMBARRASSED,0.0f},
		{Mood::VERY_EMBARRASSED,0.0f},
		{Mood::ANNOYED, 0.0f},
		{Mood::VERY_ANNOYED, 0.0f},
		{Mood::SURPRISED, 0.1f},
		{Mood::NEUTRAL, 0.4f}
	};

	chestMoods = {
		{Mood::HAPPY, 0.0f},
		{Mood::VERY_HAPPY, 0.0f},
		{Mood::ANGRY, 0.3f},
		{Mood::VERY_ANGRY, 0.1f},
		{Mood::EMBARRASSED, 0.3f},
		{Mood::VERY_EMBARRASSED, 0.3f},
		{Mood::ANNOYED, 0.0f},
		{Mood::VERY_ANNOYED, 0.0f},
		{Mood::SURPRISED, 0.0f},
		{Mood::NEUTRAL, 0.0f}
	};

	bellyMoods = {
		{Mood::HAPPY, 0.0f},
		{Mood::VERY_HAPPY, 0.0f},
		{Mood::ANGRY, 0.0f},
		{Mood::VERY_ANGRY, 0.0f},
		{Mood::EMBARRASSED, 0.3f},
		{Mood::VERY_EMBARRASSED, 0.0f},
		{Mood::ANNOYED, 0.0f},
		{Mood::VERY_ANNOYED, 0.0f},
		{Mood::SURPRISED, 0.0f},
		{Mood::NEUTRAL, 0.7f}
	};

	thighsMoods = {
		{Mood::HAPPY, 0.0f},
		{Mood::VERY_HAPPY, 0.0f},
		{Mood::ANGRY, 0.1f},
		{Mood::VERY_ANGRY, 0.05f},
		{Mood::EMBARRASSED, 0.3f},
		{Mood::VERY_EMBARRASSED, 0.0f},
		{Mood::ANNOYED, 0.2f },
		{Mood::VERY_ANNOYED, 0.05f},
		{Mood::SURPRISED, 0.0f},
		{Mood::NEUTRAL, 0.3f}
	};


	legMoods = {
		{Mood::HAPPY, 0.0f},
		{Mood::VERY_HAPPY, 0.0f},
		{Mood::ANGRY, 0.0f},
		{Mood::VERY_ANGRY, 0.0f},
		{Mood::EMBARRASSED, 0.0f},
		{Mood::VERY_EMBARRASSED, 0.0f},
		{Mood::ANNOYED, 0.0f},
		{Mood::VERY_ANNOYED, 0.0f},
		{Mood::SURPRISED, 0.0f},
		{Mood::NEUTRAL, 1.0f}
	};

	Initialize(startVariant);

	variantDialog[currentVariant].InitializeTestDialogs();
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

std::string Companion::LoadDialogFromFile(const std::string& filepath)
{
	/*
		Extract the prefixes as it was done with images
	 
		File Structure for Variant:

		variant/
			mood1_place1_variant.txt
			mood1_place2_variant.txt
			mood2_place1_variant.txt
			mood2_place2_variant.txt

			global_mood1.txt
			global_mood2.txt
	*/


	return "";
}

DialogManager Companion::LoadDialogsFromFolder(const std::string& folderPath)
{
	DialogManager dialogManager;

	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	

	return DialogManager();
}

void Companion::Initialize(Variant variant)
{
	currentVariant = variant;

	loadedImages.clear();

	std::string folderName = VariantToFolderName(currentVariant);

	std::string path = "../assets/images/" + folderName;

	variantsImg[currentVariant] = LoadImagesFromFolder(path);

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

void Companion::ChangeMood(Mood mood, Place place, float duration)
{
	if (mood != currentMood && (moodTimer <= 0.0f))
	{
		currentMood = mood;
		currentPlace = place;

		Image* nImage = variantsImg[currentVariant].GetImageForMood(currentMood);

		std::string nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(currentMood, currentPlace);

		if (nDialog == "")
		{
			nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(Mood::NEUTRAL, currentPlace);

			if (nDialog == "")
			{
				nDialog = variantDialog[currentVariant].GetRandDialogForMoodAndPlace(Mood::NEUTRAL, defaultPlace);
			}
		}

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

		std::cout << currentDialog <<" " << " is global dialog?: " << variantDialog[currentVariant].isGlobalDialog << "\n";

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

std::string Companion::PickDialog(Mood mood, Place place)
{
	

	return "";
}