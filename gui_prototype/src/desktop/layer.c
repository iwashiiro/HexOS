#include "desktop/layer.h"

int Layer_Init(
    Layer *layer,
    int width,
    int height,
    int z)
{
    if (!Surface_Init(
            &layer->surface,
            width,
            height))
    {
        return 0;
    }

    layer->visible = 1;

    layer->z_index = z;

    return 1;
}

void Layer_Destroy(
    Layer *layer)
{
    Surface_Destroy(
        &layer->surface);
}