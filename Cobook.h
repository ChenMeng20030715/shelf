#pragma once
#include "Book.h"

//�ղ�����
class Cobook :
    public Book

{

private:

    //�鼮�ʼ�
    char note[100];


public:

    Cobook() {

        //��ʼ�鼮�ʼ�Ϊ��
        for (int i = 0; i < 100; i++) note[i] = 0;
    }

    //�����鼮�ʼ�
    void setNote(char* no) {

        strcpy_s(note, no);
    }

    //��ȡ��Ǳʼ�
    char* getNote() { return note; }

};

