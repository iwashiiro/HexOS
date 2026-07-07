#include "ui/widget.h"

int Widget_Init(
    Widget *widget,
    WidgetType type,
    int x,
    int y,
    int width,
    int height)
{
    widget->type = type;

    widget->x = x;
    widget->y = y;

    widget->width = width;
    widget->height = height;

    widget->visible = 1;

    return Surface_Init(
        &widget->surface,
        width,
        height);
}

void Widget_Destroy(
    Widget *widget)
{
    Surface_Destroy(
        &widget->surface);
}