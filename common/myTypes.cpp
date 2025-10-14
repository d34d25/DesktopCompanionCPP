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

void DialogManager::InitializeTestDialogs()
{
	// Happy HEAD
	dialogsByMoodAndPlace[{Mood::HAPPY, Place::HEAD}].push_back("Happy Head Dialog 1");
	dialogsByMoodAndPlace[{Mood::HAPPY, Place::HEAD}].push_back("Happy Head Dialog 2");

	// Very Happy HEAD
	dialogsByMoodAndPlace[{Mood::VERY_HAPPY, Place::HEAD}].push_back("Very Happy Head Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_HAPPY, Place::HEAD}].push_back("Very Happy Head Dialog 2");

	// Angry CHEST
	dialogsByMoodAndPlace[{Mood::ANGRY, Place::CHEST}].push_back("Angry Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANGRY, Place::CHEST}].push_back("Angry Chest Dialog 2");

	// Surprised BELLY
	dialogsByMoodAndPlace[{Mood::SURPRISED, Place::BELLY}].push_back("Surprised Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::SURPRISED, Place::BELLY}].push_back("Surprised Belly Dialog 2");

	// Neutral THIGHS
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::THIGHS}].push_back("Neutral Thighs Dialog 1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::THIGHS}].push_back("Neutral Thighs Dialog 2");

	// Happy CHEST
	dialogsByMoodAndPlace[{Mood::HAPPY, Place::CHEST}].push_back("Happy Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::HAPPY, Place::CHEST}].push_back("Happy Chest Dialog 2");

	// Very Happy CHEST
	dialogsByMoodAndPlace[{Mood::VERY_HAPPY, Place::CHEST}].push_back("Very Happy Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_HAPPY, Place::CHEST}].push_back("Very Happy Chest Dialog 2");

	// Angry BELLY
	dialogsByMoodAndPlace[{Mood::ANGRY, Place::BELLY}].push_back("Angry Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANGRY, Place::BELLY}].push_back("Angry Belly Dialog 2");

	// Surprised THIGHS
	dialogsByMoodAndPlace[{Mood::SURPRISED, Place::THIGHS}].push_back("Surprised Thighs Dialog 1");
	dialogsByMoodAndPlace[{Mood::SURPRISED, Place::THIGHS}].push_back("Surprised Thighs Dialog 2");

	// Neutral CHEST
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::CHEST}].push_back("Neutral Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::CHEST}].push_back("Neutral Chest Dialog 2");

	// Neutral BELLY
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::BELLY}].push_back("Neutral Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::BELLY}].push_back("Neutral Belly Dialog 2");

	// Very Angry HEAD
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::HEAD}].push_back("Very Angry Head Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::HEAD}].push_back("Very Angry Head Dialog 2");

	// Very Angry CHEST
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::CHEST}].push_back("Very Angry Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::CHEST}].push_back("Very Angry Chest Dialog 2");

	// Very Angry BELLY
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::BELLY}].push_back("Very Angry Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::BELLY}].push_back("Very Angry Belly Dialog 2");

	// Very Angry THIGHS
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::THIGHS}].push_back("Very Angry Thighs Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::THIGHS}].push_back("Very Angry Thighs Dialog 2");

	// Very Angry LEGS
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::LEGS}].push_back("Very Angry Legs Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::LEGS}].push_back("Very Angry Legs Dialog 2");

	// Very Angry NONE
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::NONE}].push_back("Very Angry None Dialog 1");
	dialogsByMoodAndPlace[{Mood::VERY_ANGRY, Place::NONE}].push_back("Very Angry None Dialog 2");

	// Embarrassed HEAD
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::HEAD}].push_back("Embarrassed Head Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::HEAD}].push_back("Embarrassed Head Dialog 2");

	// Embarrassed CHEST
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::CHEST}].push_back("Embarrassed Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::CHEST}].push_back("Embarrassed Chest Dialog 2");

	// Embarrassed BELLY
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::BELLY}].push_back("Embarrassed Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::BELLY}].push_back("Embarrassed Belly Dialog 2");

	// Embarrassed THIGHS
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::THIGHS}].push_back("Embarrassed Thighs Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::THIGHS}].push_back("Embarrassed Thighs Dialog 2");

	// Embarrassed LEGS
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::LEGS}].push_back("Embarrassed Legs Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::LEGS}].push_back("Embarrassed Legs Dialog 2");

	// Embarrassed NONE
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::NONE}].push_back("Embarrassed None Dialog 1");
	dialogsByMoodAndPlace[{Mood::EMBARRASSED, Place::NONE}].push_back("Embarrassed None Dialog 2");

	// Very Embarrassed - Head
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::HEAD}].push_back("Very Embarrassed Head dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::HEAD}].push_back("Very Embarrassed Head dialog #2");

	// Very Embarrassed - Chest
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::CHEST}].push_back("Very Embarrassed Chest dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::CHEST}].push_back("Very Embarrassed Chest dialog #2");

	// Very Embarrassed - Belly
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::BELLY}].push_back("Very Embarrassed Belly dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::BELLY}].push_back("Very Embarrassed Belly dialog #2");

	// Very Embarrassed - Thighs
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::THIGHS}].push_back("Very Embarrassed Thighs dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::THIGHS}].push_back("Very Embarrassed Thighs dialog #2");

	// Very Embarrassed - Legs
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::LEGS}].push_back("Very Embarrassed Legs dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_EMBARRASSED, Place::LEGS}].push_back("Very Embarrassed Legs dialog #2");


	// Annoyed HEAD
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::HEAD}].push_back("Annoyed Head Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::HEAD}].push_back("Annoyed Head Dialog 2");

	// Annoyed CHEST
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::CHEST}].push_back("Annoyed Chest Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::CHEST}].push_back("Annoyed Chest Dialog 2");

	// Annoyed BELLY
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::BELLY}].push_back("Annoyed Belly Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::BELLY}].push_back("Annoyed Belly Dialog 2");

	// Annoyed THIGHS
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::THIGHS}].push_back("Annoyed Thighs Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::THIGHS}].push_back("Annoyed Thighs Dialog 2");

	// Annoyed LEGS
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::LEGS}].push_back("Annoyed Legs Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::LEGS}].push_back("Annoyed Legs Dialog 2");

	// Annoyed NONE
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::NONE}].push_back("Annoyed None Dialog 1");
	dialogsByMoodAndPlace[{Mood::ANNOYED, Place::NONE}].push_back("Annoyed None Dialog 2");

	// Very Annoyed - Head
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::HEAD}].push_back("Very Annoyed Head dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::HEAD}].push_back("Very Annoyed Head dialog #2");

	// Very Annoyed - Chest
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::CHEST}].push_back("Very Annoyed Chest dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::CHEST}].push_back("Very Annoyed Chest dialog #2");

	// Very Annoyed - Belly
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::BELLY}].push_back("Very Annoyed Belly dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::BELLY}].push_back("Very Annoyed Belly dialog #2");

	// Very Annoyed - Thighs
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::THIGHS}].push_back("Very Annoyed Thighs dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::THIGHS}].push_back("Very Annoyed Thighs dialog #2");

	// Very Annoyed - Legs
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::LEGS}].push_back("Very Annoyed Legs dialog #1");
	dialogsByMoodAndPlace[{Mood::VERY_ANNOYED, Place::LEGS}].push_back("Very Annoyed Legs dialog #2");


	// Neutral - Head
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::HEAD}].push_back("Neutral Head dialog #1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::HEAD}].push_back("Neutral Head dialog #2");

	// Neutral - Chest
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::CHEST}].push_back("Neutral Chest dialog #1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::CHEST}].push_back("Neutral Chest dialog #2");

	// Neutral - Belly
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::BELLY}].push_back("Neutral Belly dialog #1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::BELLY}].push_back("Neutral Belly dialog #2");

	// Neutral - Thighs
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::THIGHS}].push_back("Neutral Thighs dialog #1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::THIGHS}].push_back("Neutral Thighs dialog #2");

	// Neutral - Legs
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::LEGS}].push_back("Neutral Legs dialog #1");
	dialogsByMoodAndPlace[{Mood::NEUTRAL, Place::LEGS}].push_back("Neutral Legs dialog #2");


	// Global dialogs

	// Global Happy
	globalDialogsByMood[{Mood::HAPPY}].push_back("Global Happy Dialog 1");
	globalDialogsByMood[{Mood::HAPPY}].push_back("Global Happy Dialog 2");

	// Global Very Happy
	globalDialogsByMood[{Mood::VERY_HAPPY}].push_back("Global Very Happy Dialog 1");
	globalDialogsByMood[{Mood::VERY_HAPPY}].push_back("Global Very Happy Dialog 2");

	// Global Angry
	globalDialogsByMood[{Mood::ANGRY}].push_back("Global Angry Dialog 1");
	globalDialogsByMood[{Mood::ANGRY}].push_back("Global Angry Dialog 2");

	// Global Surprised
	globalDialogsByMood[{Mood::SURPRISED}].push_back("Global Surprised Dialog 1");
	globalDialogsByMood[{Mood::SURPRISED}].push_back("Global Surprised Dialog 2");

	// Global Embarrassed
	globalDialogsByMood[{Mood::EMBARRASSED}].push_back("Global Embarrassed Dialog 1");
	globalDialogsByMood[{Mood::EMBARRASSED}].push_back("Global Embarrassed Dialog 2");

	// Global Very Embarrassed
	globalDialogsByMood[{Mood::VERY_EMBARRASSED}].push_back("Global Very Embarrassed dialog 1");
	globalDialogsByMood[{Mood::VERY_EMBARRASSED}].push_back("Global Very Embarrassed dialog 2");


	// Global Annoyed
	globalDialogsByMood[{Mood::ANNOYED}].push_back("Global Annoyed Dialog 1");
	globalDialogsByMood[{Mood::ANNOYED}].push_back("Global Annoyed Dialog 2");

	// Global Very Annoyed
	globalDialogsByMood[{Mood::VERY_ANNOYED}].push_back("Global Very Annoyed dialog 1");
	globalDialogsByMood[{Mood::VERY_ANNOYED}].push_back("Global Very Annoyed dialog 2");


	// Global Neutral
	globalDialogsByMood[{Mood::NEUTRAL}].push_back("Global Neutral dialog 1");
	globalDialogsByMood[{Mood::NEUTRAL}].push_back("Global Neutral dialog 2");

}

