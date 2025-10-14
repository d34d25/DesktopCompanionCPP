#pragma once
#include "myTypes.h"
#include "newCanvasX.h"

extern const std::map<std::string, Mood> moodMap;

struct LoadedImage
{
    Image img;
    std::string filename;
};

std::string VariantToFolderName(Variant currentVariant);

struct CompanionRegion
{
    Shapes::Rectangle head;
    Shapes::Rectangle chest;
    Shapes::Rectangle belly;
    Shapes::Rectangle thighs;
    Shapes::Rectangle legs;
};

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

class Companion
{
private:

    float moodTimer = 0.0f;
    float moodDuration = 1.2f;

    Mood defaultMood = Mood::IDLE;
    Place defaultPlace = Place::NONE;

    NewCanvasX* canvas;

    MoodImages LoadImagesFromFolder(const std::string& folderPath);

    std::string LoadDialogFromFile(const std::string& filepath);

    DialogManager LoadDialogsFromFolder(const std::string& folderPath);

    void Initialize(Variant variant);

    std::vector<LoadedImage> loadedImages;

    std::map<Variant, MoodImages> variantsImg;
    std::map<Variant, DialogManager> variantDialog;

    Mood currentMood;
    Variant currentVariant;

    Image* currentImage;
    
    Place currentPlace;

    CompanionRegionProportions proportion = {};
    CompanionRegionProportions offset = {};

    Image* fallbackImage = nullptr;

public:

    std::string currentDialog;
    //DialogManager dialogManager;

    std::vector<std::pair<Mood, float>> headMoods;
    std::vector<std::pair<Mood, float>> chestMoods;
    std::vector<std::pair<Mood, float>> bellyMoods;
    std::vector<std::pair<Mood, float>> thighsMoods;
    std::vector<std::pair<Mood, float>> legMoods;

    CompanionRegion regions;

    Companion(NewCanvasX* canvas,Variant startVariant);

    void UpdateRegions(const Shapes::Rectangle& refRect);

    void ChangeMood(Mood mood, Place place, float duration);

    void ChangeVariant(Variant variant);

    Mood PickMoodByProbabilty(const std::vector<std::pair<Mood, float>>& moodList);

    void UpdateTimer(Place* place);

    Image* GetCurrentImage() const;

    Variant GetCurrentVariant() const;

    std::string PickDialog(Mood mood, Place place);
};