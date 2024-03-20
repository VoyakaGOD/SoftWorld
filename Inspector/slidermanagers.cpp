#include "slidermanagers.h"

IntSliderManager::IntSliderManager() {}

IntSliderManager::IntSliderManager(QSlider *slider, int min_value, int max_value)
{
    slider->setMinimum(min_value);
    slider->setMaximum(max_value);
}

int IntSliderManager::ConvertFromRaw(int value)
{
    return value;
}

int IntSliderManager::ConvertToRaw(int value)
{
    return value;
}

DoubleSliderManager::DoubleSliderManager() {}

DoubleSliderManager::DoubleSliderManager(QSlider *slider, double min_value, double max_value)
{
    slider->setMinimum(0);
    slider->setMaximum(DOUBLE_SLIDER_SENSITIVITY);
    ratio = (max_value - min_value) / DOUBLE_SLIDER_SENSITIVITY;
    shift = min_value;
}

double DoubleSliderManager::ConvertFromRaw(int value)
{
    return value * ratio + shift;
}

int DoubleSliderManager::ConvertToRaw(double value)
{
    return (value - shift) / ratio;
}
