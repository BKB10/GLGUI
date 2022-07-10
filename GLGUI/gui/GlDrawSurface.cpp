#include "GlDrawSurface.h"
#include <iostream>

void GlDrawSurface::initialize() {
}

void GlDrawSurface::tick() {

}

void GlDrawSurface::render() {
    if (actions[RENDER_ACTION] != nullptr) {
        actions[RENDER_ACTION]->action();
    }
}