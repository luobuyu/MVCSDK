#ifndef _SRC_STRUCT_H_
#define _SRC_STRUCT_H_
#include <iostream>
/*
* 该文件，编写一些需要用到的数据结构，数据类型等
*/
using namespace std;
struct Edge
{
	int u, v, next;
};

struct SetInt
{
private:
	int* index;		// 元素在element中的下标，不存在时为 0
	int* element;	// 元素 [1,cnt]
	int cnt;		// 元素个数 [1, max_size]
	int max_size;   // 元素容量
public:
	SetInt()
	{
		index = NULL;
		element = NULL;
		cnt = 0;
		max_size = 0;
	}
	SetInt(int maxSize)
	{
		index = new int[maxSize + 10]();
		element = new int[maxSize + 10]();
		cnt = 0;
		max_size = maxSize + 10 - 1;
	}
	~SetInt()
	{
		delete[] index;
		delete[] element;
	}

	inline int& operator[](int i) { return element[i]; }
	// 参数不加引用时，浅拷贝，会把成员变量的地址拷贝过来，
	// 形参调用析构函数时会将同一块地址的delete掉，后面就会出错
	SetInt& operator=(const SetInt& s) 
	{
		if (max_size < s.max_size)
		{
			delete[] index;
			delete[] element;
			index = new int[s.max_size + 1];
			element = new int[s.max_size + 1];
		}
		for (int i = 1; i <= s.size(); ++i)
		{
			element[i] = s.element[i];
		}
		for (int i = 1; i <= s.max_size; ++i)
		{
			index[i] = s.index[i];
		}
		cnt = s.cnt;
		return *this;
	}
	int get_index(int u) { return index[u]; }

	inline void insert(int u)	// 插入一个新顶点
	{
		if (index[u] != 0) return;
		element[++cnt] = u;
		index[u] = cnt;
	}
	inline void erase(int u)	// 删除顶点
	{
		if (index[u] == 0) return;
		index[element[cnt]] = index[u];
		element[index[u]] = element[cnt];
		index[u] = 0;
		cnt--;
	}

	void swap(int u, int v)
	{
		element[index[u]] = v;
		element[index[v]] = u;
		int tmp = index[u];
		index[u] = index[v];
		index[v] = tmp;
	}

	void update()
	{
		for (int i = 1; i <= cnt; ++i)
		{
			index[element[i]] = i;
		}
	}

	void clear()
	{
		fill(begin(), end(), 0);
		cnt = 0;
	}

	inline int size() const { return cnt; }
	int* begin() const { return element + 1; }
	int* end() const { return element + cnt + 1; }
	inline bool exist(int u) { return index[u] != 0; }
	inline bool empty() { return cnt == 0; }
};

#endif // !_SRC_STRUCT_H_

