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



class Companion
{
private:

    float moodTimer = 0.0f;
    float moodDuration = 1.2f;

    Mood defaultMood = Mood::IDLE;
    Place defaultPlace = Place::NONE;

    NewCanvasX* canvas;

    MoodImages LoadImagesFromFolder(const std::string& folderPath);

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

    ConfigFile* configFile = nullptr;

public:

    std::string currentDialog;
    //DialogManager dialogManager;

    std::vector<std::pair<Mood, float>> headMoods;
    std::vector<std::pair<Mood, float>> chestMoods;
    std::vector<std::pair<Mood, float>> bellyMoods;
    std::vector<std::pair<Mood, float>> thighsMoods;
    std::vector<std::pair<Mood, float>> legMoods;

    CompanionRegion regions;

    Companion(NewCanvasX* canvas, Variant startVariant, ConfigFile* config);

    ~Companion() = default;

    void UpdateRegions(const Shapes::Rectangle& refRect);

    void ChangeMood(Mood mood, Place place, float duration);

    void ChangeVariant(Variant variant);

    Mood PickMoodByProbabilty(const std::vector<std::pair<Mood, float>>& moodList);

    void UpdateTimer(Place* place);

    Image* GetCurrentImage() const;

    Variant GetCurrentVariant() const;

    void DrawDialog(Shapes::Rectangle* refRect, float scale);
};