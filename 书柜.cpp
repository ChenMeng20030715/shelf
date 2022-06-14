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


//����ͼ
IMAGE img1;
//���vector
vector<Shelf> shs;
//�ղؼ�vector
vector<Coshelf> csh;
//����·��
string DATA_DIR = "C:\\Users\\lenovo\\Desktop\\��";
//�ļ�vector
vector <_finddata_t> ff;
//��ǰ�����
int num = 0;
//��ǰ������
int Flag = 0;


//����ʱ��˳��
bool cmp1(_finddata_t a, _finddata_t b) {

	return a.time_write < b.time_write;
}


//����������
bool cmp2(Shelf a, Shelf b) {

	return a.getNum() < b.getNum();
}


//����
void bkg() {

	//���ñ���ͼ
	putimage(0, 0, &img1);

	//ǳ��ɫ
	setfillcolor(RGB(173, 186, 192));

	//��������
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));

	//��������
	solidrectangle(0, (int)(H * 8.2 / 9), W, H);

}


//��ʼ��
void init() {

	//���ر���ͼ
	loadimage(&img1, _T("����.jpg"));

	//�򿪻���
	initgraph(W, H);

	//��ȡ����
	gettextstyle(&f);

	//��ʼ������
	bkg();

}


//ɾ�����
void RemoveAllFiles(string path)
{

	//�ж��Ƿ���·��
	if (path.empty())
	{
		return;
	}



	//�����ļ���

	//���
	HANDLE hFind;
	//�ļ���
	WIN32_FIND_DATA findData;
	//·��
	string wstrTempDir = path + ("\\*");

	//������һ���ļ�
	hFind = FindFirstFile(wstrTempDir.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		// ����"."��".."�������
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
		{
			continue;
		}


		//��һ���ļ������ļ�������
		string wstrFileName;
		wstrFileName.assign(path);
		wstrFileName.append("\\");
		wstrFileName.append(findData.cFileName);


		//�ж��Ƿ����ļ���
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//�ݹ�ɾ���ļ������ļ�
			RemoveAllFiles(wstrFileName.c_str());
		}

		else
		{
			//ɾ���ļ�
			DeleteFile(wstrFileName.c_str());
		}
	}
	//������һ���ļ�
	while (FindNextFile(hFind, &findData));

	FindClose(hFind);

	//ɾ�����ļ���
	RemoveDirectory(path.c_str());

}


//��ʼ���
void getSh(string path, int flag) {

	//����ļ�vector
	ff.clear();
	//������vector
	shs.clear();
	//��������
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));


	//�����ļ�
	long hFile = 0;
	_finddata_t fileinfo;

	//�ļ�·��
	string p;
	p.assign(path).append("\\*.*");


	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {

		do {
			//�����ļ���
			if ((fileinfo.attrib & _A_SUBDIR)) {

				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					//�����ļ���
				}
			}

			else {
				//�����ļ����ļ�����Ϣ��
				ff.push_back(fileinfo);
			}
		} 
		while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	else {
		printf("Failed(%d)\n", GetLastError());
	}


	//����ʱ��˳��
	sort(ff.begin(), ff.end(), cmp1);


	//��ȡ�����Ϣ
	for (unsigned int i = 0; i < ff.size(); i++) {

		//����һ�������
		Shelf sh;
		//�����Ϣ·��
		string t;
		t.assign(DATA_DIR).append("\\").append(ff[i].name);

		/*errno_t err;
		err = fopen_s(&fp, t.c_str(), "rb");
		if (err) {
			puts("���ļ�ʧ��");
			exit(0);
		}
		fread(&sh, sizeof(sh), 1, fp);

		shs.push_back(sh);
		fclose(fp);*/


		//ʹ���ļ�������
		ifstream is;
		is.open(t.c_str(), ios_base::in | ios_base::binary);

		if (is) {
			is.read(reinterpret_cast<char*>(&sh), sizeof(sh));
			shs.push_back(sh);
		}

		is.close();

	}


	//��������������
	sort(shs.begin(), shs.end(), cmp2);


	//��ȡ��ǰ�������
	num = shs.size();


	//�����ղؼ�
	for (int i = 0; i < num; i++) {

		//����һ���ղؼ�
		Coshelf cs;
		//���ƶ�Ӧ��ܵ���Ϣ
		cs.setName(shs[i].getName());
		cs.setNum(shs[i].getNum());
		cs.setNote(shs[i].getNote());
		//�����ղؼ�vector
		csh.push_back(cs);


		//��ʾ����
		shs[i].show();

		//������ǰ������
		if (shs[i].getNum() == flag) {

			//��ʾ�鼮������
			shs[i].showBook();

			//����ɫ
			setfillcolor(RGB(137, 149, 154));
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));

			//��ɫ����
			settextcolor(BLACK);
			settextstyle(&f);

			//ǿ����ǰ���
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}

	}

}


//��ȡ���
void getShe(int flag) {

	//��������
	setfillcolor(RGB(173, 186, 192));
	solidrectangle(0, 0, W, (int)(H * 0.8 / 9));

	//��ȡ�������
	num = shs.size();
	settextstyle(&f);
	

	//������ǰ���
	for (int i = 0; i < num; i++) {

		if (shs[i].getNum() == flag) {

			//�̱���
			bkg();

			//��ʾ�в�������
			shs[i].showBook();

		}
	}


	//��ʾ����
	for (int i = 0; i < num; i++) {
		shs[i].show();
	}


	//������ǰ���
	for (int i = 0; i < num; i++) {

		if (shs[i].getNum() == flag) {

			//�������
			setfillcolor(RGB(137, 149, 154));
			//ǿ����ǰ���
			solidrectangle(shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9));

			//��ɫ����
			settextcolor(BLACK);
			//ǿ����ǰ�������
			RECT r = { shs[i].getNum() * 100 + 150, 0, shs[i].getNum() * 100 + 250, (int)(H * 0.8 / 9) };
			drawtext(shs[i].getName(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}

	}

}


//����
void readBook(int temp) {

	//��մ���
	cleardevice();
	//�̱���
	bkg();


	//����ɫ
	setfillcolor(RGB(137, 149, 154));
	//ģʽ͸��
	setbkmode(TRANSPARENT);
	//��ɫ����
	settextcolor(BLACK);


	//�����ĸ����ܿ�
	solidrectangle(50, 8, 100, 40);
	solidrectangle(50, 500, 100, 532);
	solidrectangle(860, 8, 910, 40);
	solidrectangle(860, 500, 910, 532);

	//�����ء���
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//����һҳ����
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//����һҳ����
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//"��һҳ����
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//��������Ϣ
	ExMessage e;


	//������ȡtxt
	FILE* fp;
	//�����ж�ȡ
	char ch[1000];
	//��ȡ��ҳ��
	int num = 0;


	//���·��
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\");

	//�鼮·��
	tt = t;
	t.append(bks[temp].getName()).append(".txt");

	//���鼮
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("���ļ�ʧ��");
		exit(0);
	}



	while (1) {

		//ҳ������
		num++;

		//С���鼮ͣ��ҳ��������ʾ
		if (num < bks[temp].getPage()) {

			//��ʼ���ı���
			memset(ch, sizeof(ch), 0);

			//��ȡʮ����
			for (int k = 0; k < 13; k++) {

				if ((fgets(ch, 101, fp)) != NULL)
					memset(ch, sizeof(ch), 0);

			}

			continue;

		}



		//�̱���
		bkg();
		//����ɫ
		setfillcolor(RGB(137, 149, 154));
		//ģʽ͸��
		setbkmode(TRANSPARENT);
		//��ɫ����
		settextcolor(BLACK);


		//�����ĸ����ܿ�
		solidrectangle(50, 8, 100, 40);
		solidrectangle(50, 500, 100, 532);
		solidrectangle(860, 8, 910, 40);
		solidrectangle(860, 500, 910, 532);

		//�����ء���
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//����һҳ����
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//����һҳ����
		r = { 50,(int)(H * 8.2 / 9), 100, H };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//"��һҳ����
		r = { 860,(int)(H * 8.2 / 9), 910, H };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		//��ʼ���ı���
		memset(ch, sizeof(ch), 0);
		//��ʾʮ����
		for (int k = 0; k < 13; k++) {

			if ((fgets(ch, 101, fp)) != NULL) {

				RECT r = { 100, k * 30 + 80, 950, k * 30 + 110 };
				drawtext(ch, &r, DT_LEFT);
				memset(ch, sizeof(ch), 0);

			}

		}


		//��ȡ�����Ϣ
		while (1) {

			e = getmessage(EM_MOUSE | EM_KEY);

			//��������
			if (e.message == WM_LBUTTONDOWN) {


				//��������ء�
				if (e.x > 50 && e.x < 100 && e.y>0 && e.y < H * 0.8 / 9) {

					//�ر��鼮
					fclose(fp);

					//������ǰ�����鼮����
					for (int i = 0; i < temp; i++) {

						bks[i].setFlag(i + 1);

					}

					//������������ǰ��
					bks[temp].setFlag(0);
					//����ͣ��ҳ��
					bks[temp].setPage(num);
					//�����ղ���ͣ��ҳ��
					csh[Flag].getCoBook();

					//��������ѱ��ղ�
					for (unsigned int i = 0; i < cbs.size(); i++) {

						if (strcmp(cbs[i].getName(), bks[temp].getName()) == 0) {

							//ͬ��ͣ��ҳ��
							cbs[i].setPage(num);

						}

					}


					//���������鼮��Ϣ
					for (unsigned int i = 0; i < bks.size(); i++) {

						string ttt;
						ttt = tt;
						//sddΪ�鼮��Ϣ
						ttt.append(bks[i].getName()).append(".sdd");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("���ļ�ʧ��");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/


						//ʹ���ļ���д��
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));

						os.close();

					}


					//���������ղ�����Ϣ
					for (unsigned int i = 0; i < cbs.size(); i++) {

						string ttt;
						ttt = tt;

						//coΪ�ղ�����Ϣ
						ttt.append(cbs[i].getName()).append(".co");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("���ļ�ʧ��");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/


						//ʹ���ļ���д��
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();

					}


					//�̱���
					bkg();
					//�ص���ҳ
					getShe(Flag);

					return;

				}


				//�����һҳ
				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {

					//�ر��鼮
					fclose(fp);

					//��ҳ���ĵ��ʼ
					num = 0;

					//���´��鼮
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("���ļ�ʧ��");
						exit(0);
					}

					//��ͣ��ҳ������Ϊһ
					if (bks[temp].getPage() > 1)
						bks[temp].setPage(1);

					break;

				}


				//�������һҳ��
				if (e.x > 50 && e.x < 100 && e.y>H * 8.2 / 9 && e.y < H) {

					fclose(fp);

					//��ҳ�������ڿ�ͷ
					num = 0;

					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("���ļ�ʧ��");
						exit(0);
					}

					//��ͣ��ҳ����������һҳ
					if (bks[temp].getPage() > 1)
						bks[temp].setPage(bks[temp].getPage() - 1);

					break;

				}


				//�������һҳ��
				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {

					//��ҳ����������һҳ
					bks[temp].setPage(bks[temp].getPage() + 1);

					break;

				}

			}

		}

	}

}


//���ղ���
void readCoBook(int temp) {

	//��մ���
	cleardevice();
	//�̱���
	bkg();


	//����ɫ
	setfillcolor(RGB(137, 149, 154));
	//ģʽ͸��
	setbkmode(TRANSPARENT);
	//��ɫ����
	settextcolor(BLACK);


	//�����ĸ����ܿ�
	solidrectangle(50, 8, 100, 40);
	solidrectangle(50, 500, 100, 532);
	solidrectangle(860, 8, 910, 40);
	solidrectangle(860, 500, 910, 532);

	//�����ء���
	RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
	drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//����һҳ����
	r = { 860, 0, 910, (int)(H * 0.8 / 9) };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//����һҳ����
	r = { 50,(int)(H * 8.2 / 9), 100, H };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//"��һҳ����
	r = { 860,(int)(H * 8.2 / 9), 910, H };
	drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//��������Ϣ
	ExMessage e;


	//������ȡtxt
	FILE* fp;
	//�����ж�ȡ
	char ch[1000];
	//��ȡ��ҳ��
	int num = 0;


	//���·��
	string t, tt;
	t.assign(DATA_DIR).append("\\").append(csh[Flag].getName()).append("\\");

	//�鼮·��
	tt = t;
	t.append(cbs[temp].getName()).append(".txt");

	//���鼮
	errno_t err;
	err = fopen_s(&fp, t.c_str(), "r");
	if (err) {
		puts("���ļ�ʧ��");
		exit(0);
	}



	while (1) {

		//ҳ������
		num++;

		//С���鼮ͣ��ҳ��������ʾ
		if (num < cbs[temp].getPage()) {

			//��ʼ���ı���
			memset(ch, sizeof(ch), 0);

			//��ȡʮ����
			for (int k = 0; k < 13; k++) {

				if ((fgets(ch, 101, fp)) != NULL)

					memset(ch, sizeof(ch), 0);

			}

			continue;
		}


		//�̱���
		bkg();
		//����ɫ
		setfillcolor(RGB(137, 149, 154));
		//ģʽ͸��
		setbkmode(TRANSPARENT);
		//��ɫ����
		settextcolor(BLACK);


		//�����ĸ����ܿ�
		solidrectangle(50, 8, 100, 40);
		solidrectangle(50, 500, 100, 532);
		solidrectangle(860, 8, 910, 40);
		solidrectangle(860, 500, 910, 532);

		//�����ء���
		RECT r = { 50, 0, 100, (int)(H * 0.8 / 9) };
		drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//����һҳ����
		r = { 860, 0, 910, (int)(H * 0.8 / 9) };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//����һҳ����
		r = { 50,(int)(H * 8.2 / 9), 100, H };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//"��һҳ����
		r = { 860,(int)(H * 8.2 / 9), 910, H };
		drawtext("��һҳ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		//��ʼ���ı���
		memset(ch, sizeof(ch), 0);

		//��ʾʮ����
		for (int k = 0; k < 13; k++) {

			if ((fgets(ch, 101, fp)) != NULL) {

				RECT r = { 100, k * 30 + 80, 950, k * 30 + 110 };
				drawtext(ch, &r, DT_LEFT);
				memset(ch, sizeof(ch), 0);

			}

		}


		//��ȡ�����Ϣ
		while (1) {

			e = getmessage(EM_MOUSE | EM_KEY);

			//��������
			if (e.message == WM_LBUTTONDOWN) {

				//��������ء�
				if (e.x > 50 && e.x < 100 && e.y>0 && e.y < H * 0.8 / 9) {

					//�ر��鼮
					fclose(fp);

					//������ǰ�����鼮����
					for (int i = 0; i < temp; i++) {

						cbs[i].setFlag(i + 1);

					}


					//������������ǰ��
					cbs[temp].setFlag(0);
					//����ͣ��ҳ��
					cbs[temp].setPage(num);
					//�����ղ���ͣ��ҳ��
					for (unsigned int i = 0; i < bks.size(); i++) {

						if (strcmp(bks[i].getName(), cbs[temp].getName()) == 0) {

							bks[i].setPage(num);

						}

					}


					//���������鼮��Ϣ
					for (unsigned int i = 0; i < bks.size(); i++) {

						string ttt;
						ttt = tt;

						//sddΪ�鼮��Ϣ
						ttt.append(bks[i].getName()).append(".sdd");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("���ļ�ʧ��");
							exit(0);
						}
						fwrite(&bks[i], sizeof(bks[i]), 1, fpp);
						fclose(fpp);*/

						//ʹ���ļ���д��
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&bks[i]), sizeof(bks[i]));
						os.close();

					}


					//���������ղ�����Ϣ
					for (unsigned int i = 0; i < cbs.size(); i++) {

						string ttt;
						ttt = tt;
						//coΪ�ղ�����Ϣ
						ttt.append(cbs[i].getName()).append(".co");

						/*errno_t err;
						err = fopen_s(&fpp, ttt.c_str(), "wb+");
						if (err) {
							puts("���ļ�ʧ��");
							exit(0);
						}
						fwrite(&cbs[i], sizeof(cbs[i]), 1, fpp);
						fclose(fpp);*/

						//ʹ���ļ���д��
						ofstream os;
						os.open(ttt.c_str(), ios_base::out | ios_base::binary);
						os.write(reinterpret_cast<char*>(&cbs[i]), sizeof(cbs[i]));
						os.close();

					}

					//�̱���
					bkg();
					//�ص���ҳ
					csh[Flag].showCoBook();

					return;

				}


				//�����һҳ
				if (e.x > 860 && e.x < 910 && e.y>0 && e.y < H * 8.2 / 9) {

					//�ر��鼮
					fclose(fp);

					//��ҳ���ĵ��ʼ
					num = 0;

					//���´��鼮
					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("���ļ�ʧ��");
						exit(0);
					}

					//��ͣ��ҳ������Ϊһ
					if (cbs[temp].getPage() > 1)
						cbs[temp].setPage(1);

					break;

				}


				//�������һҳ��
				if (e.x > 50 && e.x < 100 && e.y>H * 8.2 / 9 && e.y < H) {

					fclose(fp);

					//��ҳ�������ڿ�ͷ
					num = 0;

					errno_t err;
					err = fopen_s(&fp, t.c_str(), "r");
					if (err) {
						puts("���ļ�ʧ��");
						exit(0);
					}

					if (cbs[temp].getPage() > 1)

						//��ͣ��ҳ����������һҳ
						cbs[temp].setPage(cbs[temp].getPage() - 1);

					break;

				}


				//�������һҳ��
				if (e.x > 860 && e.x < 910 && e.y>H * 8.2 / 9 && e.y < H) {

					//��ҳ����������һҳ
					cbs[temp].setPage(cbs[temp].getPage() + 1);

					break;

				}

			}

		}

	}

}


//��ȡ�ղ�
void getCo(int flag) {

	//��մ���
	cleardevice();
	//�̱���
	bkg();

	//��ʾ��ǰ�ղؼ��в�������
	for (int i = 0; i < num; i++) {
		if (csh[i].getNum() == flag) {
			csh[i].showCoBook();
		}
	}


	//��ȡ�����Ϣ
	ExMessage e;


	
	while (1) {

		e = getmessage(EM_MOUSE | EM_KEY);


		//��������
		if (e.message == WM_LBUTTONDOWN) {

			//��������ء�
			if (e.x > 50 && e.x < 100 && e.y>500 && e.y < 532) {

				//��մ���
				cleardevice();

				//�ص���ҳ
				getShe(Flag);

				return;

			}


			//����鼮
			for (int i = 0; i < 3; i++) {

				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {

					for (int j = 0; j < 84; j++) {

						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {

							unsigned int temp = i * 4 + j;

							if (temp < cbs.size()) {

								//���ղ���
								readCoBook(temp);

							}

						}

					}

				}

			}

		}


		//������Ҽ�
		if (e.message == WM_RBUTTONDOWN) {

			//����鼮
			for (int i = 0; i < 3; i++) {

				if (e.y > 80 + i * 133 && e.y < 170 + i * 133) {

					for (int j = 0; j < 84; j++) {

						if (e.x > 150 + j * 190 && e.x < 217 + j * 190) {

							int temp = i * 4 + j;

							//�����CTRL
							if (e.ctrl) {


								string te = DATA_DIR;
								te.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[temp].getName()).append(".co");
								//ɾ����Ӧco�ļ�
								remove(te.c_str());


								//ʹ�õ�����ɾ����Ӧ�ղ���
								vector<Cobook> ::iterator ite = cbs.begin();
								advance(ite, temp);
								cbs.erase(ite);


								int tt = cbs.size();

								//����д��仯��co
								for (int j = i; j < tt; j++) {

									//�ղ������ǰ��
									cbs[j].setFlag(cbs[j].getFlag() - 1);

									string temp = DATA_DIR;
									temp.append("\\").append(csh[Flag].getName()).append("\\").append(cbs[j].getName()).append(".co");

									/*FILE* fp;
									fopen_s(&fp, temp.c_str(), "wb+");
									if (fp != 0) {
										fwrite(&cbs[j], sizeof(cbs[j]), 1, fp);
										fclose(fp);
									}*/

									//ʹ���ļ���д��
									ofstream os;
									os.open(temp.c_str(), ios_base::out | ios_base::binary);
									os.write(reinterpret_cast<char*>(&cbs[j]), sizeof(cbs[j]));
									os.close();

								}

								//�̱���
								bkg();
								//��ʼ��ҳ��
								csh[Flag].showCoBook();
							}


							//û�а�CTRL
							else {

								//�����鼮�ʼ�
								char s[1000] = { 0 };
								InputBox(s, 200, "�������鼮�ʼǣ�");
								cbs[temp].setNote(s);

								string te;
								te.assign(DATA_DIR).append("\\").append(csh[Flag].getName()).append("\\").append(cbs[temp].getName()).append(".co");

								/*FILE* fp;
								fopen_s(&fp, te.c_str(), "wb+");
								if (fp != 0) {
									fwrite(&cbs[temp], sizeof(cbs[temp]), 1, fp);
									fclose(fp);
								}*/

								//����co�ļ�
								ofstream os;
								os.open(te.c_str(), ios_base::out | ios_base::binary);
								os.write(reinterpret_cast<char*>(&cbs[temp]), sizeof(cbs[temp]));
								os.close();

								//�̱���
								bkg();
								//��ʼ���ղ�
								csh[Flag].showCoBook();

							}

						}

					}

				}

			}

		}

	}

}


//����
void help() {

	//����մ���
	cleardevice();
	//�̱���
	bkg();
	//ģʽ͸��
	setbkmode(TRANSPARENT);
	//��ɫ����
	settextcolor(BLACK);

	//����ɫ
	setfillcolor(RGB(137, 149, 154));

	//���ơ����ء���
	solidrectangle(860, 500, 910, 532);
	RECT r = { 860,(int)(H * 8.2 / 9),910,H };
	drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//���ư�����
	solidrectangle(150, 0, 250, (int)(H * 0.8 / 9));
	r = { 150, 0, 250, (int)(H * 0.8 / 9) };
	drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	string t;


	//�����ĵ�����
	r = { 150,80,860,100 };
	drawtext("һ����ҳ��", &r, DT_LEFT);
	r = { 150,100,860,120 };
	drawtext("1��������Ϊ������ƣ����������ƿ��л���ܣ���סCTRL���һ�ɾ����ܡ�", &r, DT_LEFT);
	r = { 150,120,860,140 };
	drawtext("2����������Ϊ���ղء���������ղء������Ӧ�ղ�ҳ�档�м�Ϊ��ܱ�ע���һ��������޸ı�ע��", &r, DT_LEFT);
	r = { 150,140,860,160 };
	drawtext("3���в�������Ϊ�鼮�����������뿴����棬�һ������ղ��鼮��", &r, DT_LEFT);
	r = { 150,160,860,180 };
	drawtext("4������������ҳ�հ״��һ������Դ�������ܡ�", &r, DT_LEFT);
	r = { 150,200,860,220 };
	drawtext("�����ղء�", &r, DT_LEFT);
	r = { 150,220,860,240 };
	drawtext("1����������Ӧ��ҳ��������ơ�", &r, DT_LEFT);
	r = { 150,240,860,260 };
	drawtext("2�����������Ϊ�����ء������������ҳ��", &r, DT_LEFT);
	r = { 150,260,860,280 };
	drawtext("3���в���������濴�飬�һ������޸��鼮��ע����סCTRL���һ�����ȡ���ղء�", &r, DT_LEFT);
	r = { 150,300,860,320 };
	drawtext("�������顣", &r, DT_LEFT);
	r = { 150,320,860,340 };
	drawtext("1����������ء��Ƴ�������档", &r, DT_LEFT);
	r = { 150,340,860,360 };
	drawtext("2���������һҳ��������һҳ���͡���һҳ����", &r, DT_LEFT);
	r = { 150,380,860,400 };
	drawtext("�ģ���ӻ�ɾ���鼮��", &r, DT_LEFT);
	r = { 150,400,860,420 };
	drawtext("1������·��Ϊ��C:\\Users\\lenovo\\Desktop\\�顱", &r, DT_LEFT);
	r = { 150,420,860,440 };
	drawtext("2��ͨ���޸��ļ������ļ�����ӻ���ɾ���鼮", &r, DT_LEFT);
	r = { 150,440,860,460 };
	drawtext("3�����������8����ܣ�ÿ�����24���飬ÿ���ղ�12����", &r, DT_LEFT);


	//��ȡ���ͼ�����Ϣ
	ExMessage e;

	while (1) {

		e = getmessage(EM_MOUSE | EM_KEY);

		//��������ء�
		if (e.message == WM_LBUTTONDOWN) {

			if (e.x > 860 && e.x<910 && e.y>H * 8.2 / 9 && e.y < H) {

				//��մ���
				cleardevice();

				//������ҳ
				getShe(Flag);

				return;

			}

		}

	}

}


//��ҳ����
void build() {

	//��ȡ�����Ϣ
	ExMessage m;


	while (1) {

		m = getmessage(EM_MOUSE | EM_KEY);


		//������Ҽ�
		if (m.message == WM_RBUTTONDOWN) {

			//�����ܲ���סctrlɾ�����
			if (m.y > 0 && m.y < H * 0.8 / 9 && m.ctrl) {

				for (int i = 0; i < num; i++) {

					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {

						//�Ƴ����
						string temp = DATA_DIR;
						temp.append("\\").append(shs[i].getName());
						RemoveAllFiles(temp);

						//�Ƴ������Ϣ
						temp.append(".txt");
						remove(temp.c_str());

						//ʹ�õ�������vector���Ƴ����
						vector<Shelf> ::iterator it = shs.begin();
						advance(it, i);
						shs.erase(it);

						vector<Coshelf> ::iterator ite = csh.begin();
						advance(ite, i);
						csh.erase(ite);


						num = shs.size();


						for (int j = i; j < num; j++) {

							//����������
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

							//����д��
							ofstream os;
							os.open(temp.c_str(), ios_base::out | ios_base::binary);
							os.write(reinterpret_cast<char*>(&shs[j]), sizeof(shs[j]));
							os.close();
						}

						//��ǰҳ��Ϊ��һ�����
						Flag = 0;

						getShe(Flag);

					}

				}

				continue;

			}


			//������
			if (m.y > H * 8.2 / 9 && m.y < H) {

				//������ܱ�ע
				char s[100] = { 0 };
				InputBox(s, 20, "��������ܱ�ע��");
				shs[Flag].setNote(s);

				string p;
				p.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append(".txt");

				/*FILE* fp;
				fopen_s(&fp, p.c_str(), "wb+");
				if (fp != 0) {
					fwrite(&shs[Flag], sizeof(shs[Flag]), 1, fp);
					fclose(fp);
				}*/

				//�����ļ�
				ofstream os;
				os.open(p.c_str(), ios_base::out | ios_base::binary);
				os.write(reinterpret_cast<char*>(&shs[Flag]), sizeof(shs[Flag]));
				os.close();

				getShe(Flag);

				continue;

			}



			//�ղ��鼮
			int t = 0;

			for (int i = 0; i < 3; i++) {

				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {

					for (int j = 0; j < 8; j++) {

						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {

							unsigned int temp = i * 8 + j;


							Cobook cb;
							string te;
							//�����ղ�vector
							if (temp < bks.size()) {
								cb.setName(bks[temp].getName());
								cb.setPage(bks[temp].getPage());
								cb.setFlag(csh[Flag].getSize());
								cb.setNote("");

								te.assign(DATA_DIR).append("\\").append(shs[Flag].getName()).append("\\").append(bks[temp].getName()).append(".co");

							}

							//����д��co
							if (temp < bks.size()) {

								/*errno_t err;
								err = fopen_s(&fps, te.c_str(), "wb+");
								if (err) {
									puts("���ļ�ʧ��");
									exit(0);
								}
								fwrite(&cb, sizeof(cb), 1, fps);
								fclose(fps);*/

								ofstream os;
								os.open(te.c_str(), ios_base::out | ios_base::binary);
								os.write(reinterpret_cast<char*>(&cb), sizeof(cb));
								os.close();

							}

							//�����ղ����
							csh[Flag].getCoBook();

							t = 1;

						}

					}

				}

			}
			if (t == 1)

				continue;


			//���������
			char s[100] = { 0 };
			InputBox(s, 20, "������������ƣ�");


			string p;
			p.assign(DATA_DIR).append("\\").append(s);

			if (0 != _access(p.c_str(), 0))
			{
				int iRst = _mkdir(p.c_str());
			}

			//�������Ϣ
			Shelf sh;
			sh.setName(s);
			sh.setNum(num);
			sh.setSize(0);
			//��ܸ�������
			num++;

			//д���������Ϣ
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

			//ת�������
			Flag = num - 1;

			getSh(DATA_DIR, Flag);

		}



		//��������
		if (m.message == WM_LBUTTONDOWN) {


			//�л����
			if (m.y > 0 && m.y < H * 0.8 / 9) {

				for (int i = 0; i < num; i++) {

					if (m.x > i * 100 + 150 && m.x < i * 100 + 250) {

						//�л�����������
						Flag = i;

						getShe(Flag);

					}

				}

				continue;

			}

			//������ղء�
			if (m.x > 50 && m.y > 500 && m.x < 100 && m.y < 532) {

				//��ת���ղ�
				getCo(Flag);

				continue;

			}

			//�����������
			if (m.x > 860 && m.y > 500 && m.x < 910 && m.y < 532) {

				help();

				continue;
			}


			//����鼮
			for (int i = 0; i < 3; i++) {

				if (m.y > 80 + i * 133 && m.y < 170 + i * 133) {

					for (int j = 0; j < 8; j++) {

						if (m.x > 150 + j * 95 && m.x < 217 + j * 95) {

							unsigned int temp = i * 8 + j;

							if (temp < bks.size()) {
								//����
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

	//��ʼ��
	init();
	//��һ�������ʾ
	getSh(DATA_DIR, Flag);
	//��ҳ
	build();

	system("pause");

	closegraph();
}