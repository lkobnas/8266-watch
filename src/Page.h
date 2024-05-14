#ifndef PAGE_H
#define PAGE_H

class Page {
public:
    virtual void displayScreen() = 0;
    virtual int getPageNumber() = 0;
};
#endif // PAGE_H