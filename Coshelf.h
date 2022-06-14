#pragma once
#include "Shelf.h"
#include "Cobook.h"
vector<Cobook>cbs;

bool cm1(Cobook a, Cobook b) {
	return a.getFlag() < b.getFlag();
}

class Coshelf :
    public Shelf
{
public:
	void getCoBook();
	void showCoBook();
};

void Coshelf::getCoBook() {
	cbs.clear();
	long   hFile = 0;
	_finddata_t fileinfo;
	string p;
	p.clear();
	p.assign("C:\\Users\\lenovo\\Desktop\\Êé\\").append(getName()).append("\\*.co*");
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				string t;
				t.assign("C:\\Users\\lenovo\\Desktop\\Êé\\").append(getName()).append("\\").append(fileinfo.name);
				/*FILE* fp;
				fopen_s(&fp, t.c_str(), "rb");
				if (fp != 0) {
					Cobook cb;
					fread(&cb, sizeof(cb), 1, fp);
					cbs.push_back(cb);
					fclose(fp);
				}*/

				ifstream is;
				is.open(t.c_str(), ios_base::in | ios_base::binary);
				if (is) {
					Cobook cb;
					is.read(reinterpret_cast<char*>(&cb), sizeof(cb));
					cbs.push_back(cb);
				}
				is.close();
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	sort(cbs.begin(), cbs.end(), cm1);
	for (unsigned int i = 0; i < cbs.size(); i++) {
		cbs[i].setFlag(i);
	}

	setSize(cbs.size());
}

void Coshelf::showCoBook() {

	getCoBook();

	RECT r = { 150, 0, 250, (int)(H * 0.8 / 9) };
	drawtext(getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	settextcolor(BLACK);
	
	settextstyle(5, 0, _T("System"));

	for (int i = 0; i <= (getSize() - 1) / 4; i++) {
		if (i * 4 + 4 > getSize()) {
			for (int j = 0; j < getSize() % 4; j++) {
				putimage(150 + j * 190, 80 + i * 133, &img2);
				r = { 150 + j * 190, 175 + i * 133,217 + j * 190 , 220 + i * 133 };
				drawtext(cbs[i * 4 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
				r = { 220 + j * 190, 85 + i * 133,312 + j * 190 , 175 + i * 133 };
				drawtext(cbs[i * 4 + j].getNote(), &r, DT_CENTER | DT_WORDBREAK);
			}
			continue;
		}
		for (int j = 0; j < 4; j++) {
			putimage(150 + j * 190, 80 + i * 133, &img2);
			r = { 150 + j * 190, 175 + i * 133,217 + j * 190, 220 + i * 133, };
			drawtext(cbs[i * 4 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
			r = { 220 + j * 190, 85 + i * 133,312 + j * 190 , 175 + i * 133 };
			drawtext(cbs[i * 4 + j].getNote(), &r, DT_CENTER | DT_WORDBREAK);
		}
	}

	setfillcolor(RGB(137, 149, 154));
	solidrectangle(50, 500, 100, 532);
	r = { 50,(int)(H * 8.2 / 9),100,H };
	drawtext("·µ»Ø", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 0,(int)(H * 8.2 / 9),W,H };
	drawtext(getNote(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}