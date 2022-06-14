#ifndef BOOK_H
#define BOOK_H

class Book
{
private:
	char name[100];
	int page=1;
	int flag=0;
public:
	Book() {
		for (int i = 0; i < 100; i++) name[i] = 0;
		page = 1;
		flag = 0;
	}
	char* getName() { return name; }
	int getPage() { return page; }
	int getFlag() { return flag; }
	void setName(char* na) { strcpy_s(name, na); }
	void setName(const char* na) { strcpy_s(name, na); }
	void setPage(int n) { page = n; }
	void setFlag(int n) { flag = n; }
};

#endif