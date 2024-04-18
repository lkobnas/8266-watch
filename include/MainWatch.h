#ifndef MAINWATCH_H
#define MAINWATCH_H

#include <string>
#include "Page.h"
#include "Display.h"

class MainWatch : public Page{
public:
    MainWatch(Display* d);

    void getInformation();
    void display() override;
    int getPageNumber() override;
};

#endif // MAINWATCH_H