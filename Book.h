#ifndef BOOK_H
#define BOOK_H


//�鼮��
class Book
{

private:

	char name[100]; //�鼮����
	int page = 1;   //�鼮ͣ��ҳ��
	int flag = 0;   //�鼮���


public:

	//���캯��
	Book() {

		for (int i = 0; i < 100; i++) {
			name[i] = 0;
		}

		page = 1;
		flag = 0;
	}


	//��������(const)
	void setName(const char* na) { strcpy_s(name, na); }
	//��������
	void setName(char* na) { strcpy_s(name, na); }
	//��ȡ����
	char* getName() { return name; }


	//��ȡ��ǰҳ��
	int getPage() { return page; }
	//���õ�ǰҳ��
	void setPage(int n) { page = n; }	
	

	//�����鼮���
	void setFlag(int n) { flag = n; }	
	//��ȡ�鼮���
	int getFlag() { return flag; }

};

#endif