#pragma once

#include <string>
#include "json.hpp"
using json = nlohmann::json; 

class WmaskConfig
{
public:
	enum GeometrySize {
		GS_FIXED, 
		GS_FIT, 
		GS_FILL, 
	};

	//NLOHMANN_JSON_SERIALIZE_ENUM(GeometrySize, {
	//	{GS_FIXED, L"Fixed"}, 
	//	{GS_FIT, L"Fit"}, 
	//	{GS_FILL, L"Fill"}, 
	//})

	enum GeometryHorizon {
		GH_CENTER, 
		GH_LEFT, 
		GH_RIGHT, 
	};

	//NLOHMANN_JSON_SERIALIZE_ENUM(GeometryHorizon, {
	//	{GH_CENTER, L"Center"}, 
	//	{GH_LEFT, L"Left"}, 
	//	{GH_RIGHT, L"Right"}, 
	//})

	enum GeometryVertical {
		GV_CENTER, 
		GV_UP, 
		GV_DOWN, 
	};

	//NLOHMANN_JSON_SERIALIZE_ENUM(GeometryVertical, {
	//	{GV_CENTER, L"Center"}, 
	//	{GV_UP, L"Up"}, 
	//	{GV_DOWN, L"Down"}, 
	//})

	enum PlayMode {
		PM_REPEAT, 
		PM_LOOP, 
		PM_SHUFFLE, 
	};

	//NLOHMANN_JSON_SERIALIZE_ENUM(PlayMode, {
	//	{PM_REPEAT, L"Repeat"}, 
	//	{PM_LOOP, L"Loop"}, 
	//	{PM_SHUFFLE, L"Shuffle"}, 
	//})

	bool active; 
	std::wstring name; 
	std::wstring exepath; 
	std::wstring dirpath; 
	std::wstring preview; 

	GeometrySize size; 
	int scale; 
	GeometryHorizon horizon; 
	int xshift; 
	GeometryVertical vertical; 
	int yshift; 

	PlayMode playmode; 
	int duration; 
	int fadeduration; 
	int opacity; 

	WmaskConfig(); 
	WmaskConfig(const WmaskConfig* w); 
};

void to_json(json& j, const WmaskConfig& w); 
void from_json(const json& j, WmaskConfig& w); 
