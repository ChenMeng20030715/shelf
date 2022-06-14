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
#include "Coshelf.h"
using namespace std;

IMAGE img1;
vector<Shelf> shs;
vector<Coshelf> csh;
string DATA_DIR = "C:\\Users\\lenovo\\Desktop\\书";
vector <_finddata_t> ff;
int num = 0;
int Flag = 0;

bool cmp1(_finddata_t a, _finddata_t b) {
	return a.time_write < b.time_write;
}
bool cmp2(Shelf a, Shelf b) {
	return a.getNum() < b.getNum();
}

void bkg() {
	putimage(0, 0, &img1);
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));
	solidrectangle(0, (int)(H * 8.2 / 9), W, H);
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
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));
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

	sort(ff.begin(), ff.end(), cmp1);

	for (unsigned int i = 0; i < ff.size(); i++) {
		Shelf sh;
		string t;
		t.assign(DATA_DIR).append("\\").append(ff[i].name);

		/*errno_t err;
		err = fopen_s(&fp, t.c_str(), "rb");
		if (err) {
			puts("打开文件失败");
			exit(0);
		}
		fread(&sh, sizeof(sh), 1, fp);

		shs.push_back(sh);
		fclose(fp);*/
		ifstream is;
		is.open(t.c_str(), ios_base::in | ios_base::binary);
		if (is) {
			is.read(reinterpret_cast<char*>(&sh), sizeof(sh));
			shs.push_back(sh);
		}
		is.close();

	}

	sort(shs.begin(), shs.end(),cmp2);
	num = shs.size();
	for (int i = 0; i < num; i++) {

		Coshelf cs;
		cs.setName(shs[i].getName());
		cs.setNum(shs[i].getNum());
		cs.setNote(shs[i].getNote());
		csh.push_back(cs);

		shs[i].show();
		if (shs[i].getNum() == flag) {
			shs[i].showBook();
			setfillcolor(RGB(137, 149, 154));
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));
			settextcolor(BLACK);
			settextstyle(&f);
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

//获取书架
void getShe(int flag) {
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));
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
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));
			settextcolor(BLACK);
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	
}

//看书
void readBook(int temp) {
	cleardevice();
	bkg();
	solidrectangle(50, 0, 100, (int)(H * 0.8 / 9));
	solidrectangle(50, (int)(H * 8.2 / 9), 100, H);
	solidrectangle(860, (int)(H * 8.2 / 9), 910, H);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ExMessage e;

	FILE* fp;
	char ch[1000];
	int num = 0;
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\");
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
		solidrectangle(50, 0, 100, (int)(H * 0.8 / 9));
		solidrectangle(50, (int)(H * 8.2 / 9), 100, H);
		solidrectangle(860, (int)(H * 8.2 / 9), 910, H);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
					csh[Flag].getCoBook();
					for (unsigned int i = 0; i < cbs.size(); i++) {
						if (strcmp(cbs[i].getName(), bks[temp].getName())==0) {
							cbs[i].setPage(num);
						}
					}

					for (unsigned int i = 0; i < bks.size(); i++) {
						string ttt;
						ttt = tt;
						ttt.append(bks[i].getName()).append(".sdd");
						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/

						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));
						os.close();

					}
					for (unsigned int i = 0; i < cbs.size(); i++) {
						string ttt;
						ttt = tt;
						ttt.append(cbs[i].getName()).append(".co");
						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/

						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();

					}

					bkg();
					getShe(Flag);
					return;
				}
				

				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {
					fclose(fp);
					num = 0;
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}
					if (bks[temp].getPage() > 1)
						bks[temp].setPage(1);
					break;
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
					if (bks[temp].getPage() > 1)
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


void readCoBook(int temp) {
	cleardevice();
	bkg();
	solidrectangle(50, 0, 100, (int)(H * 0.8 / 9));
	solidrectangle(50, (int)(H * 8.2 / 9), 100, H);
	solidrectangle(860, (int)(H * 8.2 / 9), 910, H);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ExMessage e;

	FILE* fp;
	char ch[1000];
	int num = 0;
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(csh[Flag].getName()).append("\\");
	tt = t;
	t.append(cbs[temp].getName()).append(".txt");
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("打开文件失败");
		exit(0);
	}

	while (1) {
		num++;
		if (num < cbs[temp].getPage()) {
			memset(ch, sizeof(ch), 0);
			for (int k = 0; k < 13; k++) {
				if ((fgets(ch, 101, fp)) != NULL)
					memset(ch, sizeof(ch), 0);
			}
			continue;
		}


		bkg();
		solidrectangle(50, 0, 100, (int)(H * 0.8 / 9));
		solidrectangle(50, (int)(H * 8.2 / 9), 100, H);
		solidrectangle(860, (int)(H * 8.2 / 9), 910, H);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
						cbs[i].setFlag(i + 1);
					}

					cbs[temp].setFlag(0);
					cbs[temp].setPage(num);
					for (unsigned int i = 0; i < bks.size(); i++) {
						if (strcmp(bks[i].getName(),cbs[temp].getName())==0) {
							bks[i].setPage(num);
						}
					}


					for (unsigned int i = 0; i < bks.size(); i++) {
						string ttt;
						ttt = tt;
						ttt.append(bks[i].getName()).append(".sdd");
						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));
						os.close();
					}
					for (unsigned int i = 0; i < cbs.size(); i++) {
						string ttt;
						ttt = tt;
						ttt.append(cbs[i].getName()).append(".co");
						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();
					}

					bkg();
					csh[Flag].showCoBook();
					return;
				}


				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {
					fclose(fp);
					num = 0;
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}
					if (cbs[temp].getPage() > 1)
						cbs[temp].setPage(1);
					break;
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
					if (cbs[temp].getPage() > 1)
						cbs[temp].setPage(cbs[temp].getPage() - 1);
					break;
				}

				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {
					cbs[temp].setPage(cbs[temp].getPage() + 1);
					break;
				}
			}

		}
	}
}

//获取收藏
void getCo(int flag) {

	cleardevice();
	bkg();
	for (int i = 0; i < num; i++) {
		if (csh[i].getNum() == flag) {
			csh[i].showCoBook();
		}
	}

	ExMessage e;
	while (1) {
		e = getmessage(EM_MOUSE | EM_KEY);

		if (e.message == WM_LBUTTONDOWN) {

			if (e.x > 50 && e.x < 100 && e.y>500 && e.y < 532) {
				cleardevice();
				getShe(Flag);
				return;
			}

			for (int i = 0; i < 3; i++) {
				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {
					for (int j = 0; j < 84; j++) {
						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {
							unsigned int temp = i * 4 + j;

							if (temp < cbs.size()) {
								readCoBook(temp);
							}

						}
					}
				}
			}


		}

		if (e.message == WM_RBUTTONDOWN) {
			for (int i = 0; i < 3; i++) {
				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {
					for (int j = 0; j < 84; j++) {
						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {
							int temp = i * 4 + j;

							
							if (e.ctrl) {
								string te = DATA_DIR;
								te.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[temp].getName()).append(".co");
								remove(te.c_str());

								vector<Cobook> ::iterator ite = cbs.begin();
								advance(ite, temp);
								cbs.erase(ite);


								int tt = cbs.size();
								for (int j = i; j < tt; j++) {
									cbs[j].setFlag(cbs[j].getFlag() - 1);

									string temp = DATA_DIR;
									temp.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[j].getName()).append(".co");
									/*FILE* fp;
									fopen_s(&fp, temp.c_str(), "wb+");
									if (fp != 0) {
										fwrite(&cbs[j], sizeof(cbs[j]), 1, fp);
										fclose(fp);
									}*/
									ofstream os;
									os.open(temp.c_str(), ios_base::out | ios_base::binary);
									os.write(reinterpret_cast<char*>(&cbs[j]), sizeof(cbs[j]));
									os.close();
								}

								bkg();
								csh[Flag].showCoBook();
							}

							else {
								char s[1000] = { 0 };
								InputBox(s, 200, "请输入书籍笔记：");
								cbs[temp].setNote(s);

								string te;
								te.assign(DATA_DIR).append("\\").append(csh[Flag].getName()).append("\\").append(cbs[temp].getName()).append(".co");
								/*FILE* fp;
								fopen_s(&fp, te.c_str(), "wb+");
								if (fp != 0) {
									fwrite(&cbs[temp], sizeof(cbs[temp]), 1, fp);
									fclose(fp);
								}*/
								ofstream os;
								os.open(te.c_str(), ios_base::out | ios_base::binary);
								os.write(reinterpret_cast<char*>(&cbs[temp]), sizeof(cbs[temp]));
								os.close();

								bkg();
								csh[Flag].showCoBook();

							}



						}
					}
				}
			}
		}

	}



}

//帮助
void help() {
	cleardevice();
	bkg();
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	setfillcolor(RGB(137, 149, 154));
	solidrectangle(860, 500, 910, 532);
	RECT r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	solidrectangle(150, 0, 250, (int)(H * 0.8 / 9));
	r = { 150, 0, 250, (int)(H * 0.8 / 9) };
	drawtext("帮助", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	string t;

	r = { 150,80,860,100};
	drawtext("一，首页。", &r, DT_LEFT);
	r = { 150,100,860,120 };
	drawtext("1，上栏：为书架名称，左击书架名称可切换书架，按住CTRL并右击删除书架。", &r, DT_LEFT);
	r = { 150,120,860,140 };
	drawtext("2，下栏：左为“收藏”，左击“收藏”进入对应收藏页面。中间为书架备注，右击此区域修改备注。", &r, DT_LEFT);
	r = { 150,140,860,160 };
	drawtext("3，中部：主体为书籍，左击封面进入看书界面，右击封面收藏书籍。", &r, DT_LEFT);
	r = { 150,160,860,180 };
	drawtext("4，其他：在首页空白处右击，可以创建新书架。", &r, DT_LEFT);
	r = { 150,200,860,220 };
	drawtext("二，收藏。", &r, DT_LEFT);
	r = { 150,220,860,240 };
	drawtext("1，上栏：对应首页的书架名称。", &r, DT_LEFT);
	r = { 150,240,860,260 };
	drawtext("2，下栏：左侧为“返回”，左击返回首页。", &r, DT_LEFT);
	r = { 150,260,860,280 };
	drawtext("3，中部：左击封面看书，右击封面修改书籍备注，按住CTRL并右击封面取消收藏。", &r, DT_LEFT);
	r = { 150,300,860,320 };
	drawtext("三，看书。", &r, DT_LEFT);
	r = { 150,320,860,340 };
	drawtext("1，左击“返回”推出看书界面。", &r, DT_LEFT);
	r = { 150,340,860,360 };
	drawtext("2，左击“第一页”，“上一页”和“下一页”。", &r, DT_LEFT);
	r = { 150,380,860,400 };
	drawtext("四，添加或删除书籍。", &r, DT_LEFT);
	r = { 150,400,860,420 };
	drawtext("1，存书路径为“C:\\Users\\lenovo\\Desktop\\书”", &r, DT_LEFT);
	r = { 150,420,860,440 };
	drawtext("2，通过修改文件夹中文件，添加或者删除书籍", &r, DT_LEFT);

		
	ExMessage e;
	while (1) {
		e = getmessage(EM_MOUSE | EM_KEY);

		if (e.message == WM_LBUTTONDOWN) {
			if (e.x > 860 && e.x<910 && e.y>H * 8.2 / 9 && e.y < H) {
				cleardevice();
				getShe(Flag);
				return;
			}
		}
	}
}

//创建书架
void build() {

	ExMessage m;
	while (1) {
		m = getmessage(EM_MOUSE | EM_KEY);

		if (m.message == WM_RBUTTONDOWN) {
			if (m.y > 0 && m.y < H * 0.8 / 9 && m.ctrl) {

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
						vector<Coshelf> ::iterator ite = csh.begin();
						advance(ite, i);
						csh.erase(ite);


						num = shs.size();
						for (int j = i; j < num; j++) {
							shs[j].setNum(shs[j].getNum() - 1);
							csh[j].setNum(csh[j].getNum() - 1);

							string temp = DATA_DIR;
							temp.append("\\").append(shs[j].getName()).append(".txt");
							/*FILE* fp;
							fopen_s(&fp, temp.c_str(), "wb+");
							if (fp != 0) {
								fwrite(&shs[j], sizeof(shs[j]), 1, fp);
								fclose(fp);
							}*/
							ofstream os;
							os.open(temp.c_str(), ios_base::out | ios_base::binary);
							os.write(reinterpret_cast<char*>(&shs[j]), sizeof(shs[j]));
							os.close();
						}

						Flag = 0;
						getShe(Flag);
					}
				}
				continue;
			}

			if (m.y > H * 8.2 / 9 && m.y < H) {
				char s[100] = { 0 };
				InputBox(s, 20, "请输入书架备注：");
				shs[Flag].setNote(s);

				string p;
				p.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append(".txt");
				/*FILE* fp;
				fopen_s(&fp, p.c_str(), "wb+");
				if (fp != 0) {
					fwrite(&shs[Flag], sizeof(shs[Flag]), 1, fp);
					fclose(fp);
				}*/
				ofstream os;
				os.open(p.c_str(), ios_base::out | ios_base::binary);
				os.write(reinterpret_cast<char*>(&shs[Flag]), sizeof(shs[Flag]));
				os.close();

				getShe(Flag);

				continue;
			}

			int t = 0;
			for (int i = 0; i < 3; i++) {
				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {
					for (int j = 0; j < 8; j++) {
						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {
							unsigned int temp = i * 8 + j;
							Cobook cb;
							cb.setName(bks[temp].getName());
							cb.setPage(bks[temp].getPage());
							cb.setFlag(csh[Flag].getSize());
							cb.setNote("");

							string te;
							te.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\").append(bks[temp].getName()).append(".co");

							if (temp < bks.size()) {
								/*errno_t err;
								err = fopen_s(&fps, te.c_str(), "wb+");
								if (err) {
									puts("打开文件失败");
									exit(0);
								}
								fwrite(&cb, sizeof(cb), 1, fps);
								fclose(fps);*/
								ofstream os;
								os.open(te.c_str(), ios_base::out | ios_base::binary);
								os.write(reinterpret_cast<char*>(&cb), sizeof(cb));
								os.close();
							}

							csh[Flag].getCoBook();
							t = 1;
						}
					}
				}
			}
			if (t == 1)
				continue;


			char s[100] = { 0 };
			InputBox(s, 20, "请输入书架名称：");


			string p;
			p.assign(DATA_DIR).append("\\").append(s);
			if (0 != _access(p.c_str(), 0))
			{
				int iRst = _mkdir(p.c_str());
			}

			Shelf sh;
			sh.setName(s);
			sh.setNum(num);
			sh.setSize(0);
			num++;

			p.append(".txt");
			/*FILE* fp;
			fopen_s(&fp, p.c_str(), "wb+");
			if (fp != 0) {
				fwrite(&sh, sizeof(sh), 1, fp);
				fclose(fp);
			}
			shs.push_back(sh);*/
			ofstream os;
			os.open(p.c_str(), ios_base::out | ios_base::binary);
			os.write(reinterpret_cast<char*>(&sh), sizeof(sh));
			shs.push_back(sh);
			os.close();

			Flag = num - 1;
			getSh(DATA_DIR, Flag);
		}

		if (m.message == WM_LBUTTONDOWN) {
			if (m.y > 0 && m.y < H * 0.8 / 9) {
				for (int i = 0; i < num; i++) {
					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {
						Flag = i;
						getShe(Flag);
					}
				}
				continue;
			}

			if (m.x > 50 && m.y > 500 && m.x < 100 && m.y < 532) {

				getCo(Flag);

				continue;
			}

			if (m.x > 860 && m.y > 500 && m.x < 910 && m.y < 532) {

				help();

				continue;
			}

			for (int i = 0; i < 3; i++) {
				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {
					for (int j = 0; j < 8; j++) {
						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {
							unsigned int temp = i * 8 + j;

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
	build();
	system("pause");

	closegraph();
}