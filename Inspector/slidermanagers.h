#ifndef SLIDERMANAGER_H
#define SLIDERMANAGER_H

#include <QSlider>

#define DOUBLE_SLIDER_SENSITIVITY 25000

class IntSliderManager
{
public:
    IntSliderManager();
    IntSliderManager(QSlider *slider, int min_value, int max_value);
    int ConvertFromRaw(int value);
    int ConvertToRaw(int value);
};

class DoubleSliderManager
{
private:
    double shift;
    double ratio;
public:
    DoubleSliderManager();
    DoubleSliderManager(QSlider *slider, double min_value, double max_value);
    double ConvertFromRaw(int value);
    int ConvertToRaw(double value);
};

#endif // SLIDERMANAGER_H
