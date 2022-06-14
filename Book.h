#ifndef BOOK_H
#define BOOK_H


//书籍类
class Book
{

private:

	char name[100]; //书籍名称
	int page = 1;   //书籍停留页数
	int flag = 0;   //书籍序号


public:

	//构造函数
	Book() {

		for (int i = 0; i < 100; i++) {
			name[i] = 0;
		}

		page = 1;
		flag = 0;
	}


	//设置书名(const)
	void setName(const char* na) { strcpy_s(name, na); }
	//设置书名
	void setName(char* na) { strcpy_s(name, na); }
	//获取书名
	char* getName() { return name; }


	//获取当前页数
	int getPage() { return page; }
	//设置当前页数
	void setPage(int n) { page = n; }	
	

	//设置书籍序号
	void setFlag(int n) { flag = n; }	
	//获取书籍序号
	int getFlag() { return flag; }

};

#endif