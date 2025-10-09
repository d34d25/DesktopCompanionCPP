#pragma once
#include "myTypes.h"
#include "newCanvasX.h"

extern const std::map<std::string, Mood> moodMap;

struct Dialog
{
    std::string text;
    Mood requiredMood;
};

struct LoadedImage
{
    Image img;
    std::string filename;
};

std::string VariantToFolderName(Variant currentVariant);

class Companion
{
private:

   

    std::vector<Dialog> dialogs;

    NewCanvasX* canvas;

    MoodImages LoadImagesFromFolder(const std::string& folderPath);

    std::vector<LoadedImage> loadedImages;

    std::map<Variant, MoodImages> variants;

    Mood currentMood;
    Variant currentVariant;

    Image* currentImage;

public:
   
    Companion(NewCanvasX* canvas,Variant startVariant);

    void ChangeMood(Mood mood);

    Image* GetCurrentImage();

    Variant GetCurrentVariant() const;
};