#ifndef WIDGET_H
#define WIDGET_H

#include "engine/surface.h"

typedef enum
{
    WIDGET_ICON,
    WIDGET_WINDOW,
    WIDGET_BUTTON,
    WIDGET_LABEL,
    WIDGET_PANEL

} WidgetType;

typedef struct Widget
{
    WidgetType type;

    int x;
    int y;

    int width;
    int height;

    int visible;

    Surface surface;

} Widget;

int Widget_Init(
    Widget *widget,
    WidgetType type,
    int x,
    int y,
    int width,
    int height);

void Widget_Destroy(
    Widget *widget);

#endif