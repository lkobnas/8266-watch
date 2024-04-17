#ifndef MAINWATCH_H
#define MAINWATCH_H

#include <string>
#include "Page.h"
#include "Display.h"

class MainWatch : public Page{
public:
    MainWatch(Display* d);
    ~MainWatch();

    void getWeather();
    void getTime();
    void getDate();
    void display() override;
    int getPageNumber() override;
};

#endif // MAINWATCH_H