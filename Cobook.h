#pragma once
#include "Book.h"

//收藏书类
class Cobook :
    public Book

{

private:

    //书籍笔记
    char note[100];


public:

    Cobook() {

        //初始书籍笔记为空
        for (int i = 0; i < 100; i++) note[i] = 0;
    }

    //设置书籍笔记
    void setNote(char* no) {

        strcpy_s(note, no);
    }

    //获取书记笔记
    char* getNote() { return note; }

};

