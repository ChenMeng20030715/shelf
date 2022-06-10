#pragma once
#include "Book.h"
class Cobook :
    public Book
{
private:
    char note[100];
public:
    void setNote(char* no) {
        strcpy_s(note, no);
    }
    char* getNote() { return note; }

};

