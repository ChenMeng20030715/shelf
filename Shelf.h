#ifndef SHELF_H
#define SHELF_H
#include <vector>
#include <algorithm>
#include "Book.h"
using namespace std;


vector<Book> bks; //�鼮����
vector<string> str; //txt�ļ�������
LOGFONT f; //�����ʽ
IMAGE img2; //����
int W = 960, H = 540; //����


//�����鼮�������
bool cm(Book a, Book b) {

	return a.getFlag() < b.getFlag();

}


//�����
class Shelf {

private:

	char name[100]; //�������
	char note[100]; //��ܱ�ע
	int num;        //������
	int size;       //��ܴ�С


public:

	//����
	Shelf() {

		for (int i = 0; i < 100; i++) {
			name[i] = 0;
			note[i] = 0;
		}

		num = 0;
		size = 0;
	}

	//�����������
	void setName(char* na) {
		strcpy_s(name, na);
	}

	//��ȡ�������
	char* getName() { return name; }

	//������ܱ�ע
	void setNote(char* no) {
		strcpy_s(note, no);
	}

	//��ȡ��ܱ�ע
	char* getNote() { return note; }

	//����������
	void setNum(int n) { num = n; }

	//��ȡ��ܴ�С
	int getSize() { return size; }

	//������ܴ�С(һ�㲻����)
	void setSize(int n) { size = n; }

	//��ȡ������
	int getNum() { return num; }

	//��ʾ����
	void show();

	//��ʾ�в�������
	void showBook();

};


//��ʾ����
void Shelf::show() {

	//��ɫ��ң�ģʽ͸��
	settextcolor(DARKGRAY);
	setbkmode(TRANSPARENT);
	//����Ϊf
	settextstyle(&f);

	//����������
	RECT r = { num * 100 + 150, 0, num * 100 + 250, (int)(H * 0.8 / 9) };
	drawtext(name, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}


//��ʾ�в�������
void Shelf::showBook() {

	//���ط��棬��ɫ��ɫ
	loadimage(&img2, _T("����.png"));
	settextcolor(BLACK);

	//��������vector
	str.clear();
	bks.clear();



	//����
	long   hFile = 0;
	_finddata_t fileinfo;
	string p;

	//��txt�ļ����鼮����
	p.assign("C:\\Users\\lenovo\\Desktop\\��\\").append(name).append("\\*.txt*");

	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {

			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				//����txt�ļ���
				str.push_back(fileinfo.name);
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	//������ܴ�С
	size = str.size();

	p.clear();
	//��sdd�ļ����鼮��Ϣ
	p.assign("C:\\Users\\lenovo\\Desktop\\��\\").append(name).append("\\*.sdd*");

	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {

			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

				string t;
				t.assign("C:\\Users\\lenovo\\Desktop\\��\\").append(name).append("\\").append(fileinfo.name);

				//���鼮��Ϣ�����������뵽�鼮vector��

				ifstream is;
				is.open(t.c_str(), ios_base::in | ios_base::binary);

				if (is) {
					//һ�����鼮��
					Book bk;
					is.read(reinterpret_cast<char*>(&bk), sizeof(bk));
					bks.push_back(bk);
				}

				is.close();

			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}


	//�����鼮�������
	sort(bks.begin(), bks.end(), cm);


	//Ϊ����ӵ�txt�ļ�����sdd�ļ�
	for (unsigned int i = 0; i < str.size(); i++) {

		unsigned int j = 0;

		//�����鼮vector
		for (j = 0; j < bks.size(); j++) {

			string t;
			t.assign(bks[j].getName()).append(".txt");

			//�鿴��txt�Ƿ��Ѿ�ӵ��sdd
			if (str[i] == t) {
				break;
			}

		}

		//���û�еĻ�
		if (j == bks.size()) {

			Book bk;

			string st = str[i];
			//�� .txt ɾȥ
			st.erase(st.end() - 4, st.end());

			//������������ţ��򿪵�ҳ��
			bk.setName(st.c_str());
			bk.setFlag(size);
			bk.setPage(1);
			//�����鼮vector
			bks.push_back(bk);

		}

	}


	//�����鼮�������
	sort(bks.begin(), bks.end(), cm);


	//�淶һ���鼮���
	for (unsigned int i = 0; i < bks.size(); i++) {
		bks[i].setFlag(i);
	}


	//������ܴ�С
	size = bks.size();

	//�����ֺ�Ϊ5
	settextstyle(5, 0, _T("System"));

	//��ʼһ��RECT
	RECT r = { 0, 0, W, H };
	//����������
	for (int i = 0; i <= (size - 1) / 8; i++) {

		//һ�в����˱�
		if (i * 8 + 8 > size) {

			for (int j = 0; j < size % 8; j++) {

				//���÷���
				putimage(150 + j * 95, 80 + i * 133, &img2);

				//д����
				r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
				drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
			}

			continue;
		}

		for (int j = 0; j < 8; j++) {

			//���÷���
			putimage(150 + j * 95, 80 + i * 133, &img2);

			//д����
			r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
			drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);

		}

	}

	//����ɫ
	setfillcolor(RGB(137, 149, 154));
	//���ղء���
	solidrectangle(50, 500, 100, 532);
	//����������
	solidrectangle(860, 500, 910, 532);

	//��������
	r = { 50,(int)(H * 8.2 / 9),100,H };
	drawtext("�ղ�", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//�����ܱ�ע
	r = { 150,(int)(H * 8.2 / 9),W - 150,H };
	drawtext(note, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

#endif