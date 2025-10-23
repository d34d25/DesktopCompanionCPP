#include "myTypes.h"
#include <fstream>
#include <iostream>

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

ConfigFile LoadConfigFile(std::string path)
{
	ConfigFile config;

	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "Failed to open config file: " << path << "\n";
		return config;
	}

	std::unordered_map<std::string, float*> fieldMap =
	{
		//base 
		
		//head
		{"base.happy_head_prob", &config.base.happy_head_prob},
		{"base.very_happy_head_prob", &config.base.very_happy_head_prob},
		{"base.angry_head_prob", &config.base.angry_head_prob},
		{"base.very_angry_head_prob", &config.base.very_angry_head_prob},
		{"base.embarrassed_head_prob", &config.base.embarrassed_head_prob},
		{"base.very_embarrassed_head_prob", &config.base.very_embarrassed_head_prob},
		{"base.annoyed_head_prob", &config.base.annoyed_head_prob},
		{"base.very_annoyed_head_prob", &config.base.very_annoyed_head_prob},
		{"base.surprised_head_prob", &config.base.surprised_head_prob},
		{"base.neutral_head_prob", &config.base.neutral_head_prob},

		//chest
		{"base.happy_chest_prob", &config.base.happy_chest_prob},
		{"base.very_happy_chest_prob", &config.base.very_happy_chest_prob},
		{"base.angry_chest_prob", &config.base.angry_chest_prob},
		{"base.very_angry_chest_prob", &config.base.very_angry_chest_prob},
		{"base.embarrassed_chest_prob", &config.base.embarrassed_chest_prob},
		{"base.very_embarrassed_chest_prob", &config.base.very_embarrassed_chest_prob},
		{"base.annoyed_chest_prob", &config.base.annoyed_chest_prob},
		{"base.very_annoyed_chest_prob", &config.base.very_annoyed_chest_prob},
		{"base.surprised_chest_prob", &config.base.surprised_chest_prob},
		{"base.neutral_chest_prob", &config.base.neutral_chest_prob},

		//belly
		{"base.happy_belly_prob", &config.base.happy_belly_prob},
		{"base.very_happy_belly_prob", &config.base.very_happy_belly_prob},
		{"base.angry_belly_prob", &config.base.angry_belly_prob},
		{"base.very_angry_belly_prob", &config.base.very_angry_belly_prob},
		{"base.embarrassed_belly_prob", &config.base.embarrassed_belly_prob},
		{"base.very_embarrassed_belly_prob", &config.base.very_embarrassed_belly_prob},
		{"base.annoyed_belly_prob", &config.base.annoyed_belly_prob},
		{"base.very_annoyed_belly_prob", &config.base.very_annoyed_belly_prob},
		{"base.surprised_belly_prob", &config.base.surprised_belly_prob},
		{"base.neutral_belly_prob", &config.base.neutral_belly_prob},

		//thighs
		{"base.happy_thighs_prob", &config.base.happy_thighs_prob},
		{"base.very_happy_thighs_prob", &config.base.very_happy_thighs_prob},
		{"base.angry_thighs_prob", &config.base.angry_thighs_prob},
		{"base.very_angry_thighs_prob", &config.base.very_angry_thighs_prob},
		{"base.embarrassed_thighs_prob", &config.base.embarrassed_thighs_prob},
		{"base.very_embarrassed_thighs_prob", &config.base.very_embarrassed_thighs_prob},
		{"base.annoyed_thighs_prob", &config.base.annoyed_thighs_prob},
		{"base.very_annoyed_thighs_prob", &config.base.very_annoyed_thighs_prob},
		{"base.surprised_thighs_prob", &config.base.surprised_thighs_prob},
		{"base.neutral_thighs_prob", &config.base.neutral_thighs_prob},

		//legs
		{"base.happy_legs_prob", &config.base.happy_legs_prob},
		{"base.very_happy_legs_prob", &config.base.very_happy_legs_prob},
		{"base.angry_legs_prob", &config.base.angry_legs_prob},
		{"base.very_angry_legs_prob", &config.base.very_angry_legs_prob},
		{"base.embarrassed_legs_prob", &config.base.embarrassed_legs_prob},
		{"base.very_embarrassed_legs_prob", &config.base.very_embarrassed_legs_prob},
		{"base.annoyed_legs_prob", &config.base.annoyed_legs_prob},
		{"base.very_annoyed_legs_prob", &config.base.very_annoyed_legs_prob},
		{"base.surprised_legs_prob", &config.base.surprised_legs_prob},
		{"base.neutral_legs_prob", &config.base.neutral_legs_prob},

		//nightwear

		//head
		{"night.happy_head_prob", &config.night.happy_head_prob},
		{"night.very_happy_head_prob", &config.night.very_happy_head_prob},
		{"night.angry_head_prob", &config.night.angry_head_prob},
		{"night.very_angry_head_prob", &config.night.very_angry_head_prob},
		{"night.embarrassed_head_prob", &config.night.embarrassed_head_prob},
		{"night.very_embarrassed_head_prob", &config.night.very_embarrassed_head_prob},
		{"night.annoyed_head_prob", &config.night.annoyed_head_prob},
		{"night.very_annoyed_head_prob", &config.night.very_annoyed_head_prob},
		{"night.surprised_head_prob", &config.night.surprised_head_prob},
		{"night.neutral_head_prob", &config.night.neutral_head_prob},

		//chest
		{"night.happy_chest_prob", &config.night.happy_chest_prob},
		{"night.very_happy_chest_prob", &config.night.very_happy_chest_prob},
		{"night.angry_chest_prob", &config.night.angry_chest_prob},
		{"night.very_angry_chest_prob", &config.night.very_angry_chest_prob},
		{"night.embarrassed_chest_prob", &config.night.embarrassed_chest_prob},
		{"night.very_embarrassed_chest_prob", &config.night.very_embarrassed_chest_prob},
		{"night.annoyed_chest_prob", &config.night.annoyed_chest_prob},
		{"night.very_annoyed_chest_prob", &config.night.very_annoyed_chest_prob},
		{"night.surprised_chest_prob", &config.night.surprised_chest_prob},
		{"night.neutral_chest_prob", &config.night.neutral_chest_prob},

		//belly
		{"night.happy_belly_prob", &config.night.happy_belly_prob},
		{"night.very_happy_belly_prob", &config.night.very_happy_belly_prob},
		{"night.angry_belly_prob", &config.night.angry_belly_prob},
		{"night.very_angry_belly_prob", &config.night.very_angry_belly_prob},
		{"night.embarrassed_belly_prob", &config.night.embarrassed_belly_prob},
		{"night.very_embarrassed_belly_prob", &config.night.very_embarrassed_belly_prob},
		{"night.annoyed_belly_prob", &config.night.annoyed_belly_prob},
		{"night.very_annoyed_belly_prob", &config.night.very_annoyed_belly_prob},
		{"night.surprised_belly_prob", &config.night.surprised_belly_prob},
		{"night.neutral_belly_prob", &config.night.neutral_belly_prob},

		//thighs
		{"night.happy_thighs_prob", &config.night.happy_thighs_prob},
		{"night.very_happy_thighs_prob", &config.night.very_happy_thighs_prob},
		{"night.angry_thighs_prob", &config.night.angry_thighs_prob},
		{"night.very_angry_thighs_prob", &config.night.very_angry_thighs_prob},
		{"night.embarrassed_thighs_prob", &config.night.embarrassed_thighs_prob},
		{"night.very_embarrassed_thighs_prob", &config.night.very_embarrassed_thighs_prob},
		{"night.annoyed_thighs_prob", &config.night.annoyed_thighs_prob},
		{"night.very_annoyed_thighs_prob", &config.night.very_annoyed_thighs_prob},
		{"night.surprised_thighs_prob", &config.night.surprised_thighs_prob},
		{"night.neutral_thighs_prob", &config.night.neutral_thighs_prob},

		//legs
		{"night.happy_legs_prob", &config.night.happy_legs_prob},
		{"night.very_happy_legs_prob", &config.night.very_happy_legs_prob},
		{"night.angry_legs_prob", &config.night.angry_legs_prob},
		{"night.very_angry_legs_prob", &config.night.very_angry_legs_prob},
		{"night.embarrassed_legs_prob", &config.night.embarrassed_legs_prob},
		{"night.very_embarrassed_legs_prob", &config.night.very_embarrassed_legs_prob},
		{"night.annoyed_legs_prob", &config.night.annoyed_legs_prob},
		{"night.very_annoyed_legs_prob", &config.night.very_annoyed_legs_prob},
		{"night.surprised_legs_prob", &config.night.surprised_legs_prob},
		{"night.neutral_legs_prob", &config.night.neutral_legs_prob},



		//dress
		
		//head
		{"dress.happy_head_prob", &config.dress.happy_head_prob},
		{"dress.very_happy_head_prob", &config.dress.very_happy_head_prob},
		{"dress.angry_head_prob", &config.dress.angry_head_prob},
		{"dress.very_angry_head_prob", &config.dress.very_angry_head_prob},
		{"dress.embarrassed_head_prob", &config.dress.embarrassed_head_prob},
		{"dress.very_embarrassed_head_prob", &config.dress.very_embarrassed_head_prob},
		{"dress.annoyed_head_prob", &config.dress.annoyed_head_prob},
		{"dress.very_annoyed_head_prob", &config.dress.very_annoyed_head_prob},
		{"dress.surprised_head_prob", &config.dress.surprised_head_prob},
		{"dress.neutral_head_prob", &config.dress.neutral_head_prob},

		//chest
		{"dress.happy_chest_prob", &config.dress.happy_chest_prob},
		{"dress.very_happy_chest_prob", &config.dress.very_happy_chest_prob},
		{"dress.angry_chest_prob", &config.dress.angry_chest_prob},
		{"dress.very_angry_chest_prob", &config.dress.very_angry_chest_prob},
		{"dress.embarrassed_chest_prob", &config.dress.embarrassed_chest_prob},
		{"dress.very_embarrassed_chest_prob", &config.dress.very_embarrassed_chest_prob},
		{"dress.annoyed_chest_prob", &config.dress.annoyed_chest_prob},
		{"dress.very_annoyed_chest_prob", &config.dress.very_annoyed_chest_prob},
		{"dress.surprised_chest_prob", &config.dress.surprised_chest_prob},
		{"dress.neutral_chest_prob", &config.dress.neutral_chest_prob},

		//belly
		{"dress.happy_belly_prob", &config.dress.happy_belly_prob},
		{"dress.very_happy_belly_prob", &config.dress.very_happy_belly_prob},
		{"dress.angry_belly_prob", &config.dress.angry_belly_prob},
		{"dress.very_angry_belly_prob", &config.dress.very_angry_belly_prob},
		{"dress.embarrassed_belly_prob", &config.dress.embarrassed_belly_prob},
		{"dress.very_embarrassed_belly_prob", &config.dress.very_embarrassed_belly_prob},
		{"dress.annoyed_belly_prob", &config.dress.annoyed_belly_prob},
		{"dress.very_annoyed_belly_prob", &config.dress.very_annoyed_belly_prob},
		{"dress.surprised_belly_prob", &config.dress.surprised_belly_prob},
		{"dress.neutral_belly_prob", &config.dress.neutral_belly_prob},

		//thighs
		{"dress.happy_thighs_prob", &config.dress.happy_thighs_prob},
		{"dress.very_happy_thighs_prob", &config.dress.very_happy_thighs_prob},
		{"dress.angry_thighs_prob", &config.dress.angry_thighs_prob},
		{"dress.very_angry_thighs_prob", &config.dress.very_angry_thighs_prob},
		{"dress.embarrassed_thighs_prob", &config.dress.embarrassed_thighs_prob},
		{"dress.very_embarrassed_thighs_prob", &config.dress.very_embarrassed_thighs_prob},
		{"dress.annoyed_thighs_prob", &config.dress.annoyed_thighs_prob},
		{"dress.very_annoyed_thighs_prob", &config.dress.very_annoyed_thighs_prob},
		{"dress.surprised_thighs_prob", &config.dress.surprised_thighs_prob},
		{"dress.neutral_thighs_prob", &config.dress.neutral_thighs_prob},

		//legs
		{"dress.happy_legs_prob", &config.dress.happy_legs_prob},
		{"dress.very_happy_legs_prob", &config.dress.very_happy_legs_prob},
		{"dress.angry_legs_prob", &config.dress.angry_legs_prob},
		{"dress.very_angry_legs_prob", &config.dress.very_angry_legs_prob},
		{"dress.embarrassed_legs_prob", &config.dress.embarrassed_legs_prob},
		{"dress.very_embarrassed_legs_prob", &config.dress.very_embarrassed_legs_prob},
		{"dress.annoyed_legs_prob", &config.dress.annoyed_legs_prob},
		{"dress.very_annoyed_legs_prob", &config.dress.very_annoyed_legs_prob},
		{"dress.surprised_legs_prob", &config.dress.surprised_legs_prob},
		{"dress.neutral_legs_prob", &config.dress.neutral_legs_prob},



		//swimsuit
		
		//head
		{"swim.happy_head_prob", &config.swim.happy_head_prob},
		{"swim.very_happy_head_prob", &config.swim.very_happy_head_prob},
		{"swim.angry_head_prob", &config.swim.angry_head_prob},
		{"swim.very_angry_head_prob", &config.swim.very_angry_head_prob},
		{"swim.embarrassed_head_prob", &config.swim.embarrassed_head_prob},
		{"swim.very_embarrassed_head_prob", &config.swim.very_embarrassed_head_prob},
		{"swim.annoyed_head_prob", &config.swim.annoyed_head_prob},
		{"swim.very_annoyed_head_prob", &config.swim.very_annoyed_head_prob},
		{"swim.surprised_head_prob", &config.swim.surprised_head_prob},
		{"swim.neutral_head_prob", &config.swim.neutral_head_prob},

		//chest
		{"swim.happy_chest_prob", &config.swim.happy_chest_prob},
		{"swim.very_happy_chest_prob", &config.swim.very_happy_chest_prob},
		{"swim.angry_chest_prob", &config.swim.angry_chest_prob},
		{"swim.very_angry_chest_prob", &config.swim.very_angry_chest_prob},
		{"swim.embarrassed_chest_prob", &config.swim.embarrassed_chest_prob},
		{"swim.very_embarrassed_chest_prob", &config.swim.very_embarrassed_chest_prob},
		{"swim.annoyed_chest_prob", &config.swim.annoyed_chest_prob},
		{"swim.very_annoyed_chest_prob", &config.swim.very_annoyed_chest_prob},
		{"swim.surprised_chest_prob", &config.swim.surprised_chest_prob},
		{"swim.neutral_chest_prob", &config.swim.neutral_chest_prob},

		//belly
		{"swim.happy_belly_prob", &config.swim.happy_belly_prob},
		{"swim.very_happy_belly_prob", &config.swim.very_happy_belly_prob},
		{"swim.angry_belly_prob", &config.swim.angry_belly_prob},
		{"swim.very_angry_belly_prob", &config.swim.very_angry_belly_prob},
		{"swim.embarrassed_belly_prob", &config.swim.embarrassed_belly_prob},
		{"swim.very_embarrassed_belly_prob", &config.swim.very_embarrassed_belly_prob},
		{"swim.annoyed_belly_prob", &config.swim.annoyed_belly_prob},
		{"swim.very_annoyed_belly_prob", &config.swim.very_annoyed_belly_prob},
		{"swim.surprised_belly_prob", &config.swim.surprised_belly_prob},
		{"swim.neutral_belly_prob", &config.swim.neutral_belly_prob},

		//thighs
		{"swim.happy_thighs_prob", &config.swim.happy_thighs_prob},
		{"swim.very_happy_thighs_prob", &config.swim.very_happy_thighs_prob},
		{"swim.angry_thighs_prob", &config.swim.angry_thighs_prob},
		{"swim.very_angry_thighs_prob", &config.swim.very_angry_thighs_prob},
		{"swim.embarrassed_thighs_prob", &config.swim.embarrassed_thighs_prob},
		{"swim.very_embarrassed_thighs_prob", &config.swim.very_embarrassed_thighs_prob},
		{"swim.annoyed_thighs_prob", &config.swim.annoyed_thighs_prob},
		{"swim.very_annoyed_thighs_prob", &config.swim.very_annoyed_thighs_prob},
		{"swim.surprised_thighs_prob", &config.swim.surprised_thighs_prob},
		{"swim.neutral_thighs_prob", &config.swim.neutral_thighs_prob},

		//legs
		{"swim.happy_legs_prob", &config.swim.happy_legs_prob},
		{"swim.very_happy_legs_prob", &config.swim.very_happy_legs_prob},
		{"swim.angry_legs_prob", &config.swim.angry_legs_prob},
		{"swim.very_angry_legs_prob", &config.swim.very_angry_legs_prob},
		{"swim.embarrassed_legs_prob", &config.swim.embarrassed_legs_prob},
		{"swim.very_embarrassed_legs_prob", &config.swim.very_embarrassed_legs_prob},
		{"swim.annoyed_legs_prob", &config.swim.annoyed_legs_prob},
		{"swim.very_annoyed_legs_prob", &config.swim.very_annoyed_legs_prob},
		{"swim.surprised_legs_prob", &config.swim.surprised_legs_prob},
		{"swim.neutral_legs_prob", &config.swim.neutral_legs_prob},



		//companion proportions

		//base
		{"base_proportions.headX", &config.base_proportions.headX},
		{"base_proportions.chestX", &config.base_proportions.chestX},
		{"base_proportions.bellyX", &config.base_proportions.bellyX},
		{"base_proportions.thighsX", &config.base_proportions.thighsX},
		{"base_proportions.legsX", &config.base_proportions.legsX},

		{"base_proportions.headY", &config.base_proportions.headY},
		{"base_proportions.chestY", &config.base_proportions.chestY},
		{"base_proportions.bellyY", &config.base_proportions.bellyY},
		{"base_proportions.thighsY", &config.base_proportions.thighsY},
		{"base_proportions.legsY", &config.base_proportions.legsY},
	
		//nightwear
		{"night_proportions.headX", &config.night_proportions.headX},
		{"night_proportions.chestX", &config.night_proportions.chestX},
		{"night_proportions.bellyX", &config.night_proportions.bellyX},
		{"night_proportions.thighsX", &config.night_proportions.thighsX},
		{"night_proportions.legsX", &config.night_proportions.legsX},

		{"night_proportions.headY", &config.night_proportions.headY},
		{"night_proportions.chestY", &config.night_proportions.chestY},
		{"night_proportions.bellyY", &config.night_proportions.bellyY},
		{"night_proportions.thighsY", &config.night_proportions.thighsY},
		{"night_proportions.legsY", &config.night_proportions.legsY},

		//dress

		{"dress_proportions.headX", &config.dress_proportions.headX},
		{"dress_proportions.chestX", &config.dress_proportions.chestX},
		{"dress_proportions.bellyX", &config.dress_proportions.bellyX},
		{"dress_proportions.thighsX", &config.dress_proportions.thighsX},
		{"dress_proportions.legsX", &config.dress_proportions.legsX},

		{"dress_proportions.headY", &config.dress_proportions.headY},
		{"dress_proportions.chestY", &config.dress_proportions.chestY},
		{"dress_proportions.bellyY", &config.dress_proportions.bellyY},
		{"dress_proportions.thighsY", &config.dress_proportions.thighsY},
		{"dress_proportions.legsY", &config.dress_proportions.legsY},

		//swimsuit

		{"swim_proportions.headX", &config.swim_proportions.headX},
		{"swim_proportions.chestX", &config.swim_proportions.chestX},
		{"swim_proportions.bellyX", &config.swim_proportions.bellyX},
		{"swim_proportions.thighsX", &config.swim_proportions.thighsX},
		{"swim_proportions.legsX", &config.swim_proportions.legsX},

		{"swim_proportions.headY", &config.swim_proportions.headY},
		{"swim_proportions.chestY", &config.swim_proportions.chestY},
		{"swim_proportions.bellyY", &config.swim_proportions.bellyY},
		{"swim_proportions.thighsY", &config.swim_proportions.thighsY},
		{"swim_proportions.legsY", &config.swim_proportions.legsY},

		//companion offsets

		//base 

		//X
		{"base_offset.headX", &config.base_offset.headX},
		{"base_offset.chestX", &config.base_offset.chestX},
		{"base_offset.bellyX", &config.base_offset.bellyX},
		{"base_offset.thighsX", &config.base_offset.thighsX},
		{"base_offset.legsX", &config.base_offset.legsX},

		//Y
		{ "base_offset.headY", &config.base_offset.headY},
		{ "base_offset.chestY", &config.base_offset.chestY},
		{ "base_offset.bellyY", &config.base_offset.bellyY},
		{ "base_offset.thighsY", &config.base_offset.thighsY},
		{ "base_offset.legsY", &config.base_offset.legsY},

		//nightwear

		//X
		{"night_offset.headX", &config.night_offset.headX},
		{"night_offset.chestX", &config.night_offset.chestX},
		{"night_offset.bellyX", &config.night_offset.bellyX},
		{"night_offset.thighsX", &config.night_offset.thighsX},
		{"night_offset.legsX", &config.night_offset.legsX},

		//Y
		{"night_offset.headY", &config.night_offset.headY},
		{"night_offset.chestY", &config.night_offset.chestY},
		{"night_offset.bellyY", &config.night_offset.bellyY},
		{"night_offset.thighsY", &config.night_offset.thighsY},
		{"night_offset.legsY", &config.night_offset.legsY},

		//dress

		//X
		{"dress_offset.headX", &config.dress_offset.headX},
		{"dress_offset.chestX", &config.dress_offset.chestX},
		{"dress_offset.bellyX", &config.dress_offset.bellyX},
		{"dress_offset.thighsX", &config.dress_offset.thighsX},
		{"dress_offset.legsX", &config.dress_offset.legsX},

		//Y
		{ "dress_offset.headY", &config.dress_offset.headY},
		{ "dress_offset.chestY", &config.dress_offset.chestY},
		{ "dress_offset.bellyY", &config.dress_offset.bellyY},
		{ "dress_offset.thighsY", &config.dress_offset.thighsY},
		{ "dress_offset.legsY", &config.dress_offset.legsY},

		//swimsuit

		//X
		{"swim_offset.headX", &config.swim_offset.headX},
		{"swim_offset.chestX", &config.swim_offset.chestX},
		{"swim_offset.bellyX", &config.swim_offset.bellyX},
		{"swim_offset.thighsX", &config.swim_offset.thighsX},
		{"swim_offset.legsX", &config.swim_offset.legsX},

		//Y
		{"swim_offset.headY", &config.swim_offset.headY},
		{"swim_offset.chestY", &config.swim_offset.chestY},
		{"swim_offset.bellyY", &config.swim_offset.bellyY},
		{"swim_offset.thighsY", &config.swim_offset.thighsY},
		{"swim_offset.legsY", &config.swim_offset.legsY},

		//clickable area scale and offsets
		
		//base
		{"base_clRect_offset_x", &config.base_clRect_offset_x},
		{"base_clRect_offset_y", &config.base_clRect_offset_y},
		{"base_clRect_scale_x", &config.base_clRect_scale_x},
		{"base_clRect_scale_y", &config.base_clRect_scale_y},

		//nightwear
		{"night_clRect_offset_x", &config.night_clRect_offset_x},
		{"night_clRect_offset_y", &config.night_clRect_offset_y},
		{"night_clRect_scale_x", &config.night_clRect_scale_x},
		{"night_clRect_scale_y", &config.night_clRect_scale_y},

		//dress
		{"dress_clRect_offset_x", &config.dress_clRect_offset_x},
		{"dress_clRect_offset_y", &config.dress_clRect_offset_y},
		{"dress_clRect_scale_x", &config.dress_clRect_scale_x},
		{"dress_clRect_scale_y", &config.dress_clRect_scale_y},

		//swimsuit
		{"swim_clRect_offset_x", &config.swim_clRect_offset_x},
		{"swim_clRect_offset_y", &config.swim_clRect_offset_y},
		{"swim_clRect_scale_x", &config.swim_clRect_scale_x},
		{"swim_clRect_scale_y", &config.swim_clRect_scale_y}
	};

	std::string line;

	while (std::getline(file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

		if (line.empty() || line.rfind("//", 0) == 0) continue;

		size_t eqPos = line.find("=");
		if (eqPos == std::string::npos) continue;

		std::string key = line.substr(0, eqPos);
		std::string valueStr = line.substr(eqPos + 1);

		try
		{
			if (key == "debug_draw")
			{
				config.debug_draw = (valueStr == "1");
				std::cout << "debu_draw: " << config.debug_draw << "\n";
			}
			else
			{
				float value = std::stof(valueStr);

				auto it = fieldMap.find(key);
				if (it != fieldMap.end())
				{
					*(it->second) = value;
				}
			}
			
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error parsing value for key " << key << ": " << e.what() << "\n";
		}
	}

	return config;
}

