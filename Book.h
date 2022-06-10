#ifndef BOOK_H
#define BOOK_H

class Book
{
private:
	char name[100];
	int page=1;
	int flag=0;
public:
	char* getName() { return name; }
	int getPage() { return page; }
	int getFlag() { return flag; }
	void setName(char* na) { strcpy_s(name, na); }
	void setName(const char* na) { strcpy_s(name, na); }
	void setPage(int n) { page = n; }
	void setFlag(int n) { flag = n; }
};

#endif