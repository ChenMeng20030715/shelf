#include <easyx.h>
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <algorithm>
#include "Shelf.h"
using namespace std;

IMAGE img1;
vector<Shelf> shs;
string DATA_DIR = "C:\\Users\\lenovo\\Desktop\\书";
vector <_finddata_t> ff;
int num = 0;
int Flag = 1;

bool cmp(_finddata_t a, _finddata_t b) {
	return a.time_write < b.time_write;
}

void bkg() {
	putimage(0, 0, &img1);
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, H * 0.8 / 9);
	solidrectangle(0, H * 8.2 / 9, W, H);
}

//背景
void init() {
	loadimage(&img1, _T("背景.jpg"));
	initgraph(W, H);
	gettextstyle(&f);
	bkg();
}

//删除书架
void RemoveAllFiles(string path)
{
	if (path.empty())
	{
		return;
	}
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	string wstrTempDir = path + ("\\*");
	hFind = FindFirstFile(wstrTempDir.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// 忽略"."和".."两个结果
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
		{
			continue;
		}
		string wstrFileName;
		wstrFileName.assign(path);
		wstrFileName.append("\\");
		wstrFileName.append(findData.cFileName);
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)// 是否是目录
		{
			RemoveAllFiles(wstrFileName.c_str());
		}
		else
		{
			DeleteFile(wstrFileName.c_str());
		}
	} while (FindNextFile(hFind, &findData));
	FindClose(hFind);
	RemoveDirectory(path.c_str());
}


//初始书架
void getSh(string path, int flag) {
	ff.clear();
	shs.clear();
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, H * 0.8 / 9);
	long   hFile = 0;
	_finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					//shs.push_back(fileinfo.name);
				}
			}
			else {
				ff.push_back(fileinfo);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	else {
		printf("Failed(%d)\n", GetLastError());
	}

	sort(ff.begin(), ff.end(), cmp);

	for (int i = 0; i < ff.size(); i++) {
		Shelf sh;
		FILE* fp;
		string t;
		t.assign(DATA_DIR).append("\\").append(ff[i].name);

		errno_t err;
		err = fopen_s(&fp, t.c_str(), "rb");
		if (err) {
			puts("打开文件失败");
			exit(0);
		}
		fread(&sh, sizeof(sh), 1, fp);
		shs.push_back(sh);
		fclose(fp);
	}

	num = shs.size();
	for (int i = 0; i < num; i++) {
		shs[i].show();
		if (shs[i].getNum() == flag) {
			shs[i].showBook();
			setfillcolor(RGB(137, 149, 154));
			solidrectangle(shs[i].getNum() * 100 + 50, 0, shs[i].getNum() * 100 + 150, H * 0.8 / 9);
			settextcolor(BLACK);
			settextstyle(&f);
			RECT r = { shs[i].getNum() * 100 + 50, 0, shs[i].getNum() * 100 + 150, H * 0.8 / 9 };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

//获取书架
void getShe(int flag) {
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, H * 0.8 / 9);
	num = shs.size();
	settextstyle(&f);

	for (int i = 0; i < num; i++) {
		if (shs[i].getNum() == flag) {
			bkg();
			shs[i].showBook();
		}
	}


	for (int i = 0; i < num; i++) {
		shs[i].show();
	}

	for (int i = 0; i < num; i++) {
		if (shs[i].getNum() == flag) {
			setfillcolor(RGB(137, 149, 154));
			solidrectangle(shs[i].getNum() * 100 + 50, 0, shs[i].getNum() * 100 + 150, H * 0.8 / 9);
			settextcolor(BLACK);
			RECT r = { shs[i].getNum() * 100 + 50, 0, shs[i].getNum() * 100 + 150, H * 0.8 / 9 };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	
}

//看书
void readBook(int temp) {
	cleardevice();
	bkg();
	solidrectangle(50, 0, 100, H * 0.8 / 9);
	solidrectangle(50, H * 8.2 / 9, 100, H);
	solidrectangle(860, H * 8.2 / 9, 910, H);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT r = { 50, 0, 100, H * 0.8 / 9 };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ExMessage e;

	FILE* fp, * fpp;
	char ch[1000];
	int num = 0;
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(shs[Flag - 1].getName()).append("\\");
	tt = t;
	t.append(bks[temp].getName()).append(".txt");
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("打开文件失败");
		exit(0);
	}

	while (1) {
		num++;
		if (num < bks[temp].getPage()) {
			memset(ch, sizeof(ch), 0);
			for (int k = 0; k < 13; k++) {
				if ((fgets(ch, 101, fp)) != NULL)
					memset(ch, sizeof(ch), 0);
			}
			continue;
		}


		bkg();
		solidrectangle(50, 0, 100, H * 0.8 / 9);
		solidrectangle(50, H * 8.2 / 9, 100, H);
		solidrectangle(860, H * 8.2 / 9, 910, H);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT r = { 50, 0, 100, H * 0.8 / 9 };
		drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		r = { 50,(int)(H * 8.2 / 9), 100, H };
		drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		r = { 860,(int)(H * 8.2 / 9), 910, H };
		drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		settextcolor(BLACK);
		memset(ch, sizeof(ch), 0);
		for (int k = 0; k < 13; k++) {
			if ((fgets(ch, 101, fp)) != NULL) {
				RECT r = { 100, k * 30 + 80, 950, k * 30 + 110 };
				drawtext(ch, &r, DT_LEFT);
				memset(ch, sizeof(ch), 0);
			}
		}

		while (1) {
			e = getmessage(EM_MOUSE | EM_KEY);

			if (e.message == WM_LBUTTONDOWN) {
				if (e.x > 50 && e.x < 100 && e.y>0 && e.y < H * 0.8 / 9) {
					fclose(fp);
					for (int i = 0; i < temp; i++) {
						bks[i].setFlag(i+1);
					}

					bks[temp].setFlag(0);
					bks[temp].setPage(num);

					for (int i = 0; i < bks.size(); i++) {
						string ttt;
						ttt = tt;
						ttt.append(bks[i].getName()).append(".sdd");
						errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);
					}

					bkg();
					getShe(Flag);
					return;
				}
				

				if (e.x > 50 && e.x < 100 && e.y>H * 8.2 / 9 && e.y < H) {
					fclose(fp);
					num = 0;
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}
					bks[temp].setPage(bks[temp].getPage() - 1);
					break;
				}

				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {
					bks[temp].setPage(bks[temp].getPage() + 1);
					break;
				}
			}

		}
	}
}

//创建书架
void bulid() {

	ExMessage m;
	while (1) {
		m = getmessage(EM_MOUSE | EM_KEY);
		
		if (m.message == WM_RBUTTONDOWN) {
			if (m.y > 0 && m.y < H * 0.8 / 9 && m.ctrl) {
				char s[100] = { 0 };
				for (int i = 0; i < num; i++) {
					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {

						string temp = DATA_DIR;
						temp.append("\\").append(shs[i].getName());
						RemoveAllFiles(temp);
						temp.append(".txt");
						remove(temp.c_str());

						vector<Shelf> ::iterator it = shs.begin();
						advance(it, i);
						shs.erase(it);
						num = shs.size();
						for (int j = i; j < num; j++) {
							shs[j].setNum(shs[j].getNum() - 1);

							string temp = DATA_DIR;
							temp.append("\\").append(shs[i].getName()).append(".txt");
							FILE* fp;
							fopen_s(&fp, temp.c_str(), "wb+");
							if (fp != 0) {
								fwrite(&shs[j], sizeof(shs[j]), 1, fp);
								fclose(fp);
							}
						}

						Flag = 1;
						getShe(Flag);
					}
				}
				continue;
			}

			if (m.y > H * 8.2 / 9 && m.y < H) {
				char s[100] = { 0 };
				InputBox(s, 20, "请输入书架备注：");
				shs[Flag - 1].setNote(s);

				string p;
				p.assign(DATA_DIR).append("\\").append(shs[Flag - 1].getName()).append(".txt");
				FILE* fp;
				fopen_s(&fp, p.c_str(), "wb+");
				if (fp != 0) {
					fwrite(&shs[Flag - 1], sizeof(shs[Flag - 1]), 1, fp);
					fclose(fp);
				}

				getShe(Flag);

				continue;
			}


			char s[100] = { 0 };
			InputBox(s, 20, "请输入书架名称：");

			string p;
			p.assign(DATA_DIR).append("\\").append(s);
			if (0 != _access(p.c_str(), 0))
			{
				int iRst = _mkdir(p.c_str());
			}

			num++;
			Shelf sh;
			sh.setName(s);
			sh.setNum(num);

			p.append(".txt");
			FILE* fp;
			fopen_s(&fp, p.c_str(), "wb+");
			if (fp != 0) {
				fwrite(&sh, sizeof(sh), 1, fp);
				fclose(fp);
			}
			shs.push_back(sh);

			Flag = num;
			getShe(Flag);
		}

		if (m.message == WM_LBUTTONDOWN) {
			if (m.y > 0 && m.y < H * 0.8 / 9) {
				for (int i = 1; i <= num; i++) {
					if (m.x > i * 100 + 50 && m.x < i * 100 + 150) {
						Flag = i;
						getShe(Flag);
					}

				}
				continue;
			}

			for (int i = 0; i < 3; i++) {
				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {
					for (int j = 0; j < 8; j++) {
						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {
							int temp = i * 8 + j;

							if (temp < bks.size()) {
								readBook(temp);
							}

						}
					}
				}
			}
		}
	}
}

int main()
{
	init();
	getSh(DATA_DIR, Flag);
	bulid();
	system("pause");

	closegraph();
}