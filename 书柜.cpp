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


//背景图
IMAGE img1;
//书架vector
vector<Shelf> shs;
//收藏架vector
vector<Coshelf> csh;
//常用路径
string DATA_DIR = "C:\\Users\\lenovo\\Desktop\\书";
//文件vector
vector <_finddata_t> ff;
//当前书架数
int num = 0;
//当前书架序号
int Flag = 0;


//按照时间顺序
bool cmp1(_finddata_t a, _finddata_t b) {

	return a.time_write < b.time_write;
}


//按照书架序号
bool cmp2(Shelf a, Shelf b) {

	return a.getNum() < b.getNum();
}


//背景
void bkg() {

	//放置背景图
	putimage(0, 0, &img1);

	//浅蓝色
	setfillcolor(RGB(173, 186, 192));

	//绘制上栏
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));

	//绘制下栏
	solidrectangle(0, (int)(H * 8.2 / 9), W, H);

}


//初始化
void init() {

	//加载背景图
	loadimage(&img1, _T("背景.jpg"));

	//打开画布
	initgraph(W, H);

	//获取字体
	gettextstyle(&f);

	//初始化背景
	bkg();

}


//删除书架
void RemoveAllFiles(string path)
{

	//判断是否有路径
	if (path.empty())
	{
		return;
	}



	//搜索文件夹

	//句柄
	HANDLE hFind;
	//文件名
	WIN32_FIND_DATA findData;
	//路径
	string wstrTempDir = path + ("\\*");

	//搜索第一个文件
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


		//建一个文件或者文件夹名称
		string wstrFileName;
		wstrFileName.assign(path);
		wstrFileName.append("\\");
		wstrFileName.append(findData.cFileName);


		//判断是否是文件夹
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//递归删除文件夹中文件
			RemoveAllFiles(wstrFileName.c_str());
		}

		else
		{
			//删除文件
			DeleteFile(wstrFileName.c_str());
		}
	}
	//搜索下一个文件
	while (FindNextFile(hFind, &findData));

	FindClose(hFind);

	//删除空文件夹
	RemoveDirectory(path.c_str());

}


//初始书架
void getSh(string path, int flag) {

	//清空文件vector
	ff.clear();
	//清空书架vector
	shs.clear();
	//绘制上栏
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));


	//搜索文件
	long hFile = 0;
	_finddata_t fileinfo;

	//文件路径
	string p;
	p.assign(path).append("\\*.*");


	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {
			//忽略文件夹
			if ((fileinfo.attrib & _A_SUBDIR)) {

				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					//忽略文件夹
				}
			}

			else {
				//推入文件（文件夹信息）
				ff.push_back(fileinfo);
			}
		} 
		while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	else {
		printf("Failed(%d)\n", GetLastError());
	}


	//按照时间顺序
	sort(ff.begin(), ff.end(), cmp1);


	//获取书架信息
	for (unsigned int i = 0; i < ff.size(); i++) {

		//创建一个新书架
		Shelf sh;
		//书架信息路径
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


		//使用文件流读出
		ifstream is;
		is.open(t.c_str(), ios_base::in | ios_base::binary);

		if (is) {
			is.read(reinterpret_cast<char*>(&sh), sizeof(sh));
			shs.push_back(sh);
		}

		is.close();

	}


	//按照书架序号排序
	sort(shs.begin(), shs.end(), cmp2);


	//获取当前书架数量
	num = shs.size();


	//生成收藏夹
	for (int i = 0; i < num; i++) {

		//创建一个收藏架
		Coshelf cs;
		//复制对应书架的信息
		cs.setName(shs[i].getName());
		cs.setNum(shs[i].getNum());
		cs.setNote(shs[i].getNote());
		//推入收藏架vector
		csh.push_back(cs);


		//显示上栏
		shs[i].show();

		//搜索当前书架序号
		if (shs[i].getNum() == flag) {

			//显示书籍和下栏
			shs[i].showBook();

			//深蓝色
			setfillcolor(RGB(137, 149, 154));
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));

			//黑色字体
			settextcolor(BLACK);
			settextstyle(&f);

			//强调当前书架
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}

	}

}


//获取书架
void getShe(int flag) {

	//绘制上栏
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));

	//获取书架数量
	num = shs.size();
	settextstyle(&f);
	

	//搜索当前书架
	for (int i = 0; i < num; i++) {

		if (shs[i].getNum() == flag) {

			//铺背景
			bkg();

			//显示中部和下栏
			shs[i].showBook();

		}
	}


	//显示上栏
	for (int i = 0; i < num; i++) {
		shs[i].show();
	}


	//搜索当前书架
	for (int i = 0; i < num; i++) {

		if (shs[i].getNum() == flag) {

			//填充深蓝
			setfillcolor(RGB(137, 149, 154));
			//强调当前书架
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));

			//黑色字体
			settextcolor(BLACK);
			//强调当前书架名称
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}

	}

}


//看书
void readBook(int temp) {

	//清空窗口
	cleardevice();
	//铺背景
	bkg();


	//深蓝色
	setfillcolor(RGB(137, 149, 154));
	//模式透明
	setbkmode(TRANSPARENT);
	//黑色字体
	settextcolor(BLACK);


	//绘制四个功能框
	solidrectangle(50, 8, 100, 40);
	solidrectangle(50, 500, 100, 532);
	solidrectangle(860, 8, 910, 40);
	solidrectangle(860, 500, 910, 532);

	//“返回”框
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//“第一页”框
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//“上一页”框
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//"下一页“框
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//鼠标键盘信息
	ExMessage e;


	//用来读取txt
	FILE* fp;
	//按照行读取
	char ch[1000];
	//读取的页数
	int num = 0;


	//存放路径
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\");

	//书籍路径
	tt = t;
	t.append(bks[temp].getName()).append(".txt");

	//打开书籍
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("打开文件失败");
		exit(0);
	}



	while (1) {

		//页数增加
		num++;

		//小于书籍停留页数，不显示
		if (num < bks[temp].getPage()) {

			//初始化文本行
			memset(ch, sizeof(ch), 0);

			//读取十三行
			for (int k = 0; k < 13; k++) {

				if ((fgets(ch, 101, fp)) != NULL)
					memset(ch, sizeof(ch), 0);

			}

			continue;

		}



		//铺背景
		bkg();
		//深蓝色
		setfillcolor(RGB(137, 149, 154));
		//模式透明
		setbkmode(TRANSPARENT);
		//黑色字体
		settextcolor(BLACK);


		//绘制四个功能框
		solidrectangle(50, 8, 100, 40);
		solidrectangle(50, 500, 100, 532);
		solidrectangle(860, 8, 910, 40);
		solidrectangle(860, 500, 910, 532);

		//“返回”框
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//“第一页”框
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//“上一页”框
		r = { 50,(int)(H * 8.2 / 9), 100, H };
		drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//"下一页“框
		r = { 860,(int)(H * 8.2 / 9), 910, H };
		drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		//初始化文本行
		memset(ch, sizeof(ch), 0);
		//显示十三行
		for (int k = 0; k < 13; k++) {

			if ((fgets(ch, 101, fp)) != NULL) {

				RECT r = { 100, k * 30 + 80, 950, k * 30 + 110 };
				drawtext(ch, &r, DT_LEFT);
				memset(ch, sizeof(ch), 0);

			}

		}


		//获取鼠标信息
		while (1) {

			e = getmessage(EM_MOUSE | EM_KEY);

			//如果是左键
			if (e.message == WM_LBUTTONDOWN) {


				//点击”返回“
				if (e.x > 50 && e.x < 100 && e.y>0 && e.y < H * 0.8 / 9) {

					//关闭书籍
					fclose(fp);

					//将此书前方的书籍后移
					for (int i = 0; i < temp; i++) {

						bks[i].setFlag(i + 1);

					}

					//将此书移至最前方
					bks[temp].setFlag(0);
					//更新停留页数
					bks[temp].setPage(num);
					//更新收藏书停留页数
					csh[Flag].getCoBook();

					//如果此书已被收藏
					for (unsigned int i = 0; i < cbs.size(); i++) {

						if (strcmp(cbs[i].getName(), bks[temp].getName()) == 0) {

							//同步停留页数
							cbs[i].setPage(num);

						}

					}


					//重新生成书籍信息
					for (unsigned int i = 0; i < bks.size(); i++) {

						string ttt;
						ttt = tt;
						//sdd为书籍信息
						ttt.append(bks[i].getName()).append(".sdd");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/


						//使用文件流写入
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));

						os.close();

					}


					//重新生成收藏书信息
					for (unsigned int i = 0; i < cbs.size(); i++) {

						string ttt;
						ttt = tt;

						//co为收藏书信息
						ttt.append(cbs[i].getName()).append(".co");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/


						//使用文件流写入
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();

					}


					//铺背景
					bkg();
					//回到首页
					getShe(Flag);

					return;

				}


				//点击第一页
				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {

					//关闭书籍
					fclose(fp);

					//将页数改到最开始
					num = 0;

					//重新打开书籍
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}

					//将停留页数设置为一
					if (bks[temp].getPage() > 1)
						bks[temp].setPage(1);

					break;

				}


				//点击”上一页“
				if (e.x > 50 && e.x < 100 && e.y>H * 8.2 / 9 && e.y < H) {

					fclose(fp);

					//将页数设置在开头
					num = 0;

					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}

					//将停留页数设置在上一页
					if (bks[temp].getPage() > 1)
						bks[temp].setPage(bks[temp].getPage() - 1);

					break;

				}


				//点击”下一页“
				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {

					//将页数设置在下一页
					bks[temp].setPage(bks[temp].getPage() + 1);

					break;

				}

			}

		}

	}

}


//看收藏书
void readCoBook(int temp) {

	//清空窗口
	cleardevice();
	//铺背景
	bkg();


	//深蓝色
	setfillcolor(RGB(137, 149, 154));
	//模式透明
	setbkmode(TRANSPARENT);
	//黑色字体
	settextcolor(BLACK);


	//绘制四个功能框
	solidrectangle(50, 8, 100, 40);
	solidrectangle(50, 500, 100, 532);
	solidrectangle(860, 8, 910, 40);
	solidrectangle(860, 500, 910, 532);

	//“返回”框
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//“第一页”框
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//“上一页”框
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//"下一页“框
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//鼠标键盘信息
	ExMessage e;


	//用来读取txt
	FILE* fp;
	//按照行读取
	char ch[1000];
	//读取的页数
	int num = 0;


	//存放路径
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(csh[Flag].getName()).append("\\");

	//书籍路径
	tt = t;
	t.append(cbs[temp].getName()).append(".txt");

	//打开书籍
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("打开文件失败");
		exit(0);
	}



	while (1) {

		//页数增加
		num++;

		//小于书籍停留页数，不显示
		if (num < cbs[temp].getPage()) {

			//初始化文本行
			memset(ch, sizeof(ch), 0);

			//读取十三行
			for (int k = 0; k < 13; k++) {

				if ((fgets(ch, 101, fp)) != NULL)

					memset(ch, sizeof(ch), 0);

			}

			continue;
		}


		//铺背景
		bkg();
		//深蓝色
		setfillcolor(RGB(137, 149, 154));
		//模式透明
		setbkmode(TRANSPARENT);
		//黑色字体
		settextcolor(BLACK);


		//绘制四个功能框
		solidrectangle(50, 8, 100, 40);
		solidrectangle(50, 500, 100, 532);
		solidrectangle(860, 8, 910, 40);
		solidrectangle(860, 500, 910, 532);

		//“返回”框
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//“第一页”框
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("第一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//“上一页”框
		r = { 50,(int)(H * 8.2 / 9), 100, H };
		drawtext("上一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//"下一页“框
		r = { 860,(int)(H * 8.2 / 9), 910, H };
		drawtext("下一页", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		//初始化文本行
		memset(ch, sizeof(ch), 0);

		//显示十三行
		for (int k = 0; k < 13; k++) {

			if ((fgets(ch, 101, fp)) != NULL) {

				RECT r = { 100, k * 30 + 80, 950, k * 30 + 110 };
				drawtext(ch, &r, DT_LEFT);
				memset(ch, sizeof(ch), 0);

			}

		}


		//获取鼠标信息
		while (1) {

			e = getmessage(EM_MOUSE | EM_KEY);

			//如果是左键
			if (e.message == WM_LBUTTONDOWN) {

				//点击”返回“
				if (e.x > 50 && e.x < 100 && e.y>0 && e.y < H * 0.8 / 9) {

					//关闭书籍
					fclose(fp);

					//将此书前方的书籍后移
					for (int i = 0; i < temp; i++) {

						cbs[i].setFlag(i + 1);

					}


					//将此书移至最前方
					cbs[temp].setFlag(0);
					//更新停留页数
					cbs[temp].setPage(num);
					//更新收藏书停留页数
					for (unsigned int i = 0; i < bks.size(); i++) {

						if (strcmp(bks[i].getName(), cbs[temp].getName()) == 0) {

							bks[i].setPage(num);

						}

					}


					//重新生成书籍信息
					for (unsigned int i = 0; i < bks.size(); i++) {

						string ttt;
						ttt = tt;

						//sdd为书籍信息
						ttt.append(bks[i].getName()).append(".sdd");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/

						//使用文件流写入
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));
						os.close();

					}


					//重新生成收藏书信息
					for (unsigned int i = 0; i < cbs.size(); i++) {

						string ttt;
						ttt = tt;
						//co为收藏书信息
						ttt.append(cbs[i].getName()).append(".co");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("打开文件失败");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/

						//使用文件流写入
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();

					}

					//铺背景
					bkg();
					//回到首页
					csh[Flag].showCoBook();

					return;

				}


				//点击第一页
				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {

					//关闭书籍
					fclose(fp);

					//将页数改到最开始
					num = 0;

					//重新打开书籍
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}

					//将停留页数设置为一
					if (cbs[temp].getPage() > 1)
						cbs[temp].setPage(1);

					break;

				}


				//点击”上一页“
				if (e.x > 50 && e.x < 100 && e.y>H * 8.2 / 9 && e.y < H) {

					fclose(fp);

					//将页数设置在开头
					num = 0;

					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("打开文件失败");
						exit(0);
					}

					if (cbs[temp].getPage() > 1)

						//将停留页数设置在上一页
						cbs[temp].setPage(cbs[temp].getPage() - 1);

					break;

				}


				//点击”下一页“
				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {

					//将页数设置在下一页
					cbs[temp].setPage(cbs[temp].getPage() + 1);

					break;

				}

			}

		}

	}

}


//获取收藏
void getCo(int flag) {

	//清空窗口
	cleardevice();
	//铺背景
	bkg();

	//显示当前收藏夹中部和下栏
	for (int i = 0; i < num; i++) {
		if (csh[i].getNum() == flag) {
			csh[i].showCoBook();
		}
	}


	//获取鼠标信息
	ExMessage e;


	
	while (1) {

		e = getmessage(EM_MOUSE | EM_KEY);


		//如果是左键
		if (e.message == WM_LBUTTONDOWN) {

			//点击“返回”
			if (e.x > 50 && e.x < 100 && e.y>500 && e.y < 532) {

				//清空窗口
				cleardevice();

				//回到首页
				getShe(Flag);

				return;

			}


			//点击书籍
			for (int i = 0; i < 3; i++) {

				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {

					for (int j = 0; j < 84; j++) {

						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {

							unsigned int temp = i * 4 + j;

							if (temp < cbs.size()) {

								//看收藏书
								readCoBook(temp);

							}

						}

					}

				}

			}

		}


		//如果是右键
		if (e.message == WM_RBUTTONDOWN) {

			//点击书籍
			for (int i = 0; i < 3; i++) {

				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {

					for (int j = 0; j < 84; j++) {

						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {

							int temp = i * 4 + j;

							//如果按CTRL
							if (e.ctrl) {


								string te = DATA_DIR;
								te.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[temp].getName()).append(".co");
								//删除对应co文件
								remove(te.c_str());


								//使用迭代器删除对应收藏书
								vector<Cobook> ::iterator ite = cbs.begin();
								advance(ite, temp);
								cbs.erase(ite);


								int tt = cbs.size();

								//重新写入变化的co
								for (int j = i; j < tt; j++) {

									//收藏书序号前移
									cbs[j].setFlag(cbs[j].getFlag() - 1);

									string temp = DATA_DIR;
									temp.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[j].getName()).append(".co");

									/*FILE* fp;
									fopen_s(&fp, temp.c_str(), "wb+");
									if (fp != 0) {
										fwrite(&cbs[j], sizeof(cbs[j]), 1, fp);
										fclose(fp);
									}*/

									//使用文件流写入
									ofstream os;
									os.open(temp.c_str(), ios_base::out | ios_base::binary);
									os.write(reinterpret_cast<char*>(&cbs[j]), sizeof(cbs[j]));
									os.close();

								}

								//铺背景
								bkg();
								//初始化页面
								csh[Flag].showCoBook();
							}


							//没有按CTRL
							else {

								//输入书籍笔记
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

								//更新co文件
								ofstream os;
								os.open(te.c_str(), ios_base::out | ios_base::binary);
								os.write(reinterpret_cast<char*>(&cbs[temp]), sizeof(cbs[temp]));
								os.close();

								//铺背景
								bkg();
								//初始化收藏
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

	//清理空窗口
	cleardevice();
	//铺背景
	bkg();
	//模式透明
	setbkmode(TRANSPARENT);
	//黑色字体
	settextcolor(BLACK);

	//深蓝色
	setfillcolor(RGB(137, 149, 154));

	//绘制“返回”框
	solidrectangle(860, 500, 910, 532);
	RECT r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//绘制帮助框
	solidrectangle(150, 0, 250, (int)(H * 0.8 / 9));
	r = { 150, 0, 250, (int)(H * 0.8 / 9) };
	drawtext("帮助", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	string t;


	//帮助文档内容
	r = { 150,80,860,100 };
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
	r = { 150,440,860,460 };
	drawtext("3，最多可以添加8个书架，每个书架24本书，每个收藏12本书", &r, DT_LEFT);


	//获取鼠标和键盘信息
	ExMessage e;

	while (1) {

		e = getmessage(EM_MOUSE | EM_KEY);

		//点击“返回”
		if (e.message == WM_LBUTTONDOWN) {

			if (e.x > 860 && e.x<910 && e.y>H * 8.2 / 9 && e.y < H) {

				//清空窗口
				cleardevice();

				//返回首页
				getShe(Flag);

				return;

			}

		}

	}

}


//首页操作
void build() {

	//获取鼠标信息
	ExMessage m;


	while (1) {

		m = getmessage(EM_MOUSE | EM_KEY);


		//如果是右键
		if (m.message == WM_RBUTTONDOWN) {

			//点击书架并按住ctrl删除书架
			if (m.y > 0 && m.y < H * 0.8 / 9 && m.ctrl) {

				for (int i = 0; i < num; i++) {

					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {

						//移除书架
						string temp = DATA_DIR;
						temp.append("\\").append(shs[i].getName());
						RemoveAllFiles(temp);

						//移除书架信息
						temp.append(".txt");
						remove(temp.c_str());

						//使用迭代器在vector中移除书架
						vector<Shelf> ::iterator it = shs.begin();
						advance(it, i);
						shs.erase(it);

						vector<Coshelf> ::iterator ite = csh.begin();
						advance(ite, i);
						csh.erase(ite);


						num = shs.size();


						for (int j = i; j < num; j++) {

							//更新书架序号
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

							//重新写入
							ofstream os;
							os.open(temp.c_str(), ios_base::out | ios_base::binary);
							os.write(reinterpret_cast<char*>(&shs[j]), sizeof(shs[j]));
							os.close();
						}

						//当前页面为第一个书架
						Flag = 0;

						getShe(Flag);

					}

				}

				continue;

			}


			//点击书架
			if (m.y > H * 8.2 / 9 && m.y < H) {

				//输入书架备注
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

				//更新文件
				ofstream os;
				os.open(p.c_str(), ios_base::out | ios_base::binary);
				os.write(reinterpret_cast<char*>(&shs[Flag]), sizeof(shs[Flag]));
				os.close();

				getShe(Flag);

				continue;

			}



			//收藏书籍
			int t = 0;

			for (int i = 0; i < 3; i++) {

				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {

					for (int j = 0; j < 8; j++) {

						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {

							unsigned int temp = i * 8 + j;


							Cobook cb;
							string te;
							//更新收藏vector
							if (temp < bks.size()) {
								cb.setName(bks[temp].getName());
								cb.setPage(bks[temp].getPage());
								cb.setFlag(csh[Flag].getSize());
								cb.setNote("");

								te.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\").append(bks[temp].getName()).append(".co");

							}

							//重新写入co
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

							//更新收藏书架
							csh[Flag].getCoBook();

							t = 1;

						}

					}

				}

			}
			if (t == 1)

				continue;


			//创建新书架
			char s[100] = { 0 };
			InputBox(s, 20, "请输入书架名称：");


			string p;
			p.assign(DATA_DIR).append("\\").append(s);

			if (0 != _access(p.c_str(), 0))
			{
				int iRst = _mkdir(p.c_str());
			}

			//新书架信息
			Shelf sh;
			sh.setName(s);
			sh.setNum(num);
			sh.setSize(0);
			//书架个数增加
			num++;

			//写入新书架信息
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

			//转到新书架
			Flag = num - 1;

			getSh(DATA_DIR, Flag);

		}



		//如果是左键
		if (m.message == WM_LBUTTONDOWN) {


			//切换书架
			if (m.y > 0 && m.y < H * 0.8 / 9) {

				for (int i = 0; i < num; i++) {

					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {

						//切换到点击的书架
						Flag = i;

						getShe(Flag);

					}

				}

				continue;

			}

			//点击“收藏”
			if (m.x > 50 && m.y > 500 && m.x < 100 && m.y < 532) {

				//跳转到收藏
				getCo(Flag);

				continue;

			}

			//点击“帮助”
			if (m.x > 860 && m.y > 500 && m.x < 910 && m.y < 532) {

				help();

				continue;
			}


			//点击书籍
			for (int i = 0; i < 3; i++) {

				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {

					for (int j = 0; j < 8; j++) {

						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {

							unsigned int temp = i * 8 + j;

							if (temp < bks.size()) {
								//看书
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

	//初始化
	init();
	//第一个书架显示
	getSh(DATA_DIR, Flag);
	//首页
	build();

	system("pause");

	closegraph();
}