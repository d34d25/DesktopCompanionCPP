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

class Companion
{
private:

    float moodTimer = 0.0f;
    float moodDuration = 1.2f;

    Mood defaultMood = Mood::IDLE;

    std::vector<Dialog> dialogs;

    NewCanvasX* canvas;

    MoodImages LoadImagesFromFolder(const std::string& folderPath);

    void Initialize(Variant variant);

    std::vector<LoadedImage> loadedImages;

    std::map<Variant, MoodImages> variants;

    Mood currentMood;
    Variant currentVariant;

    Image* currentImage;

public:
   
    Companion(NewCanvasX* canvas,Variant startVariant);

    void ChangeMood(Mood mood, float duration);

    void ChangeVariant(Variant variant);

    void UpdateTimer();

    Image* GetCurrentImage() const;

    Variant GetCurrentVariant() const;
};