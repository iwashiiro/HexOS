#ifndef LAYER_H
#define LAYER_H

#include "engine/surface.h"

typedef struct Layer
{
    Surface surface;

    int visible;

    int z_index;

} Layer;

int Layer_Init(
    Layer *layer,
    int width,
    int height,
    int z);

void Layer_Destroy(
    Layer *layer);

#endif