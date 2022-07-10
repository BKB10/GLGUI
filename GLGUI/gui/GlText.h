#pragma once
#include "GlComponent.h"
#include <string>

class GlText : public GlComponent {
public:
	GlText(int x, int y, std::string text) : GlComponent(x, y, 0, 0) {}

	void tick();

	void render();
};