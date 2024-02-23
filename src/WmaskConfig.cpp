#include "WmaskConfig.h"

WmaskConfig::WmaskConfig() {
	this->active = false; 
	this->name = L""; 
	this->exepath = L""; 
	this->dirpath = L""; 
	this->preview = L""; 
	this->size = WmaskConfig::GeometrySize::GS_FIXED; 
	this->scale = 100; 
	this->horizon = WmaskConfig::GeometryHorizon::GH_RIGHT; 
	this->xshift = 0; 
	this->vertical = WmaskConfig::GeometryVertical::GV_DOWN; 
	this->yshift = 0; 
	this->playmode = WmaskConfig::PlayMode::PM_REPEAT; 
	this->duration = 10000; 
	this->opacity = 240; 
}

WmaskConfig::WmaskConfig(const WmaskConfig* w) {
	this->active = w->active; 
	this->name = w->name; 
	this->exepath = w->exepath; 
	this->dirpath = w->dirpath; 
	this->preview = w->preview; 
	this->size = w->size; 
	this->scale = w->scale; 
	this->horizon = w->horizon; 
	this->xshift = w->xshift; 
	this->vertical = w->vertical; 
	this->yshift = w->yshift; 
	this->playmode = w->playmode; 
	this->duration = w->duration; 
	this->opacity = w->opacity; 
}

void to_json(json& j, const WmaskConfig& w) {
	j["active"] = w.active; 
	j["name"] = w.name; 
	j["exepath"] = w.exepath; 
	j["dirpath"] = w.dirpath; 
	j["preview"] = w.preview; 
	j["size"] = w.size; 
	j["scale"] = w.scale; 
	j["horizon"] = w.horizon; 
	j["xshift"] = w.xshift; 
	j["vertical"] = w.vertical; 
	j["yshift"] = w.yshift; 
	j["playmode"] = w.playmode; 
	j["duration"] = w.duration; 
	j["opacity"] = w.opacity; 
}

void from_json(const json& j, WmaskConfig& w) {
	w.active = j["active"]; 
	w.name = j["name"].template get<std::wstring>();
	w.exepath = j["exepath"].template get<std::wstring>(); 
	w.dirpath = j["dirpath"].template get<std::wstring>(); 
	w.preview = j["preview"].template get<std::wstring>(); 
	w.size = j["size"].template get<WmaskConfig::GeometrySize>();
	w.scale = j["scale"]; 
	w.horizon = j["horizon"].template get<WmaskConfig::GeometryHorizon>(); 
	w.xshift = j["xshift"]; 
	w.vertical = j["vertical"].template get<WmaskConfig::GeometryVertical>(); 
	w.yshift = j["yshift"]; 
	w.playmode = j["playmode"].template get<WmaskConfig::PlayMode>(); 
	w.duration = j["duration"]; 
	w.opacity = j["opacity"]; 
}
