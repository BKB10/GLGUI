#pragma once
#include "GlAction.h"
#include <map>

class GlComponent {
public:
	int x;
	int y;
	int width;
	int height;

	GlComponent* parent;

	std::map<int, GlAction*> actions;

	GlComponent(int _x, int _y, int _width, int _height) { x = _x; y = _y; width = _width; height = _height; }

	virtual void initialize() = 0;

	virtual void tick() = 0;

	virtual void render() = 0;
};