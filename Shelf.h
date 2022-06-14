#ifndef SHELF_H
#define SHELF_H
#include <vector>
#include <algorithm>
#include "Book.h"
using namespace std;


vector<Book> bks; //书籍向量
vector<string> str; //txt文件名向量
LOGFONT f; //字体格式
IMAGE img2; //封面
int W = 960, H = 540; //长宽


//根据书籍序号排序
bool cm(Book a, Book b) {

	return a.getFlag() < b.getFlag();

}


//书架类
class Shelf {

private:

	char name[100]; //书架名称
	char note[100]; //书架备注
	int num;        //书架序号
	int size;       //书架大小


public:

	//构造
	Shelf() {

		for (int i = 0; i < 100; i++) {
			name[i] = 0;
			note[i] = 0;
		}

		num = 0;
		size = 0;
	}

	//设置书架名称
	void setName(char* na) {
		strcpy_s(name, na);
	}

	//获取书架名称
	char* getName() { return name; }

	//设置书架备注
	void setNote(char* no) {
		strcpy_s(note, no);
	}

	//获取书架备注
	char* getNote() { return note; }

	//设置书架序号
	void setNum(int n) { num = n; }

	//获取书架大小
	int getSize() { return size; }

	//设置书架大小(一般不设置)
	void setSize(int n) { size = n; }

	//获取书架序号
	int getNum() { return num; }

	//显示上栏
	void show();

	//显示中部和下栏
	void showBook();

};


//显示上栏
void Shelf::show() {

	//颜色深灰，模式透明
	settextcolor(DARKGRAY);
	setbkmode(TRANSPARENT);
	//字体为f
	settextstyle(&f);

	//输出书架名称
	RECT r = { num * 100 + 150, 0, num * 100 + 250, (int)(H * 0.8 / 9) };
	drawtext(name, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}


//显示中部和下栏
void Shelf::showBook() {

	//加载封面，颜色黑色
	loadimage(&img2, _T("封面.png"));
	settextcolor(BLACK);

	//清理两个vector
	str.clear();
	bks.clear();



	//找书
	long   hFile = 0;
	_finddata_t fileinfo;
	string p;

	//找txt文件，书籍本身
	p.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\*.txt*");

	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {

			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				//加入txt文件名
				str.push_back(fileinfo.name);
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	//更新书架大小
	size = str.size();

	p.clear();
	//找sdd文件，书籍信息
	p.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\*.sdd*");

	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {

			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

				string t;
				t.assign("C:\\Users\\lenovo\\Desktop\\书\\").append(name).append("\\").append(fileinfo.name);

				//将书籍信息读出来，加入到书籍vector中

				ifstream is;
				is.open(t.c_str(), ios_base::in | ios_base::binary);

				if (is) {
					//一个“书籍”
					Book bk;
					is.read(reinterpret_cast<char*>(&bk), sizeof(bk));
					bks.push_back(bk);
				}

				is.close();

			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}


	//按照书籍序号排序
	sort(bks.begin(), bks.end(), cm);


	//为新添加的txt文件生成sdd文件
	for (unsigned int i = 0; i < str.size(); i++) {

		unsigned int j = 0;

		//遍历书籍vector
		for (j = 0; j < bks.size(); j++) {

			string t;
			t.assign(bks[j].getName()).append(".txt");

			//查看该txt是否已经拥有sdd
			if (str[i] == t) {
				break;
			}

		}

		//如果没有的话
		if (j == bks.size()) {

			Book bk;

			string st = str[i];
			//将 .txt 删去
			st.erase(st.end() - 4, st.end());

			//设置书名，序号，打开的页数
			bk.setName(st.c_str());
			bk.setFlag(size);
			bk.setPage(1);
			//加入书籍vector
			bks.push_back(bk);

		}

	}


	//按照书籍序号排序
	sort(bks.begin(), bks.end(), cm);


	//规范一遍书籍序号
	for (unsigned int i = 0; i < bks.size(); i++) {
		bks[i].setFlag(i);
	}


	//更新书架大小
	size = bks.size();

	//设置字号为5
	settextstyle(5, 0, _T("System"));

	//初始一个RECT
	RECT r = { 0, 0, W, H };
	//输出书架名称
	for (int i = 0; i <= (size - 1) / 8; i++) {

		//一行不满八本
		if (i * 8 + 8 > size) {

			for (int j = 0; j < size % 8; j++) {

				//放置封面
				putimage(150 + j * 95, 80 + i * 133, &img2);

				//写书名
				r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
				drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);
			}

			continue;
		}

		for (int j = 0; j < 8; j++) {

			//放置封面
			putimage(150 + j * 95, 80 + i * 133, &img2);

			//写书名
			r = { 150 + j * 95, 175 + i * 133,217 + j * 95, 220 + i * 133, };
			drawtext(bks[i * 8 + j].getName(), &r, DT_CENTER | DT_WORDBREAK);

		}

	}

	//深蓝色
	setfillcolor(RGB(137, 149, 154));
	//“收藏”框
	solidrectangle(50, 500, 100, 532);
	//“帮助”框
	solidrectangle(860, 500, 910, 532);

	//框中文字
	r = { 50,(int)(H * 8.2 / 9),100,H };
	drawtext("收藏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("帮助", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//输出书架备注
	r = { 150,(int)(H * 8.2 / 9),W - 150,H };
	drawtext(note, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

#endif