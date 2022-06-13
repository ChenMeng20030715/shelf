#ifndef SHELF_H
#define SHELF_H
#include <vector>
#include <algorithm>
#include "Book.h"
using namespace std;

vector<Book> bks;
vector<string> str;
LOGFONT f;
IMAGE img2;
int W = 960, H = 540;

bool cm(Book a, Book b) {
	return a.getFlag() < b.getFlag();
}


class Shelf {
private:
	char name[100];
	char note[100];
	int num;
	int size;
public:
	Shelf() {
		for (int i = 0; i < 100; i++) name[i] = 0, note[i] = 0;
		num = 0;
		size = 0;
	}
	void setName(char* na) {
		strcpy_s(name, na);
	}
	void setNote(char* no) {
		strcpy_s(note, no);
	}
	char* getName() { return name; }
	char* getNote() { return note; }
	void setNum(int n) { num = n; }
	int getSize() { return size; }
	void setSize(int n) { size = n; }
	int getNum() { return num; }
	void show();
	void showBook();
};

void Shelf::show() {
	settextcolor(DARKGRAY);
	setbkmode(TRANSPARENT);
	settextstyle(&f);
	RECT r = { num * 100 + 150, 0, num * 100 + 250, H * 0.8 / 9 };
	drawtext(name, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void Shelf::showBook() {

	loadimage(&img2, _T("封面.png"));
	settextcolor(BLACK);
	str.clear();
	bks.clear();
	long   hFile = 0;
	_finddata_t fileinfo;
	string p;
	p.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\*.txt*");
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				str.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	size = str.size();

	p.clear();
	p.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\*.sdd*");
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				string t;
				t.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\").append(fileinfo.name);
				//FILE* fp;
				//fopen_s(&fp, t.c_str(), "rb");
				/*if (fp != 0) {
					Book bk;
					fread(&bk, sizeof(bk), 1, fp);
					bks.push_back(bk);
					fclose(fp);
				}*/
				ifstream is;
				is.open(t.c_str(), ios_base::in | ios_base::binary);
				if (is) {
					Book bk;
					is.read(reinterpret_cast<char*>(&bk), sizeof(bk));
					bks.push_back(bk);

				}
				is.close();

			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	sort(bks.begin(), bks.end(), cm);

	for (int i = 0; i < str.size(); i++) {
		int j = 0;
		for (j = 0; j < bks.size(); j++) {
			string t;
			t.assign(bks[j].getName()).append(".txt");
			if (str[i] == t) {
				break;
			}
		}
		if (j == bks.size()) {
			Book bk;
			string st = str[i];
			st.erase(st.end() - 4, st.end());
			bk.setName(st.c_str());
			bk.setFlag(size);
			bk.setPage(1);
			bks.push_back(bk);
		}
	}

	sort(bks.begin(), bks.end(), cm);
	for (int i = 0; i < bks.size(); i++) {
		bks[i].setFlag(i);
	}

	size = bks.size();
	settextstyle(5, 0, _T("System"));
	RECT r = { 0, 0, W, H };
	for (int i = 0; i <= (size - 1) / 8; i++) {
		if (i * 8 + 8 > size) {
			for (int j = 0; j < size % 8; j++) {
				putimage(150 + j * 95, 80 + i * 133, &img2);
				r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
				drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
			}
			continue;
		}
		for (int j = 0; j < 8; j++) {
			putimage(150 + j * 95, 80 + i * 133, &img2);
			r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
			drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
		}
	}


	setfillcolor(RGB(137, 149, 154));
	solidrectangle(50, 500, 100, 532);
	solidrectangle(860, 500, 910, 532);
	r = { 50,(int)(H * 8.2 / 9),100,H };
	drawtext("收藏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("帮助", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 150,(int)(H * 8.2 / 9),W - 150,H };
	drawtext(note, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

#endif