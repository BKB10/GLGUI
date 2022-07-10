#pragma once
#include "GlComponent.h"

class GlDrawSurface : public GlComponent {
public:
	static const int RENDER_ACTION = 0;

	GlDrawSurface() : GlComponent(0, 0, 0, 0) {}

	void initialize();

	void tick();

	void render();
};