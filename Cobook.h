#pragma once
#include "Book.h"
class Cobook :
    public Book
{
private:
    char note[100];
public:
    Cobook() {
        for (int i = 0; i < 100; i++) note[i] = 0;
    }
    void setNote(char* no) {
        strcpy_s(note, no);
    }
    char* getNote() { return note; }

};

