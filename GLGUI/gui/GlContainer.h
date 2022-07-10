#pragma once
#include "GlComponent.h"
#include <vector>

class GlContainer : public GlComponent {
public:
	std::vector<GlComponent*> components;

	GlContainer(int _x, int _y, int _width, int _height) : GlComponent(_x, _y, _width, _height) {}

	void add(GlComponent* component);
};