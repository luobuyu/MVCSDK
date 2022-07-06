#ifndef _SRC_INSTANCE_H_
#define _SRC_INSTANCE_H_

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include "Utils.h"
#include "Config.h"
#include "Struct.h"
using namespace std;
using namespace utils;
using namespace fast_io;
using namespace Debug;
class Environment {

private:
	string _ins_path;
	string _ins_dir;
	string _ins_name;
	string _ins_id;

public:
	Environment(const string& ins_str) : _ins_path(ins_str) { utils::split_filename(_ins_path, _ins_dir, _ins_name, _ins_id); }

public:
	const string& instance_name() const { return _ins_name; }
	string instance_path() const { return instance_dir() + _ins_name + ".txt"; }
	string solution_path() const { return solution_dir() + _ins_name + ".txt"; }
	string solution_path_with_time() const { return solution_dir() + _ins_name + "." + utils::Date::to_long_str() + ".txt"; }
	string ins_html_path() const { return instance_dir() + _ins_name + ".html"; }
	string sol_html_path() const { return solution_dir() + _ins_name + ".html"; }
	string sol_html_path_with_time() const { return solution_dir() + _ins_name + "." + utils::Date::to_long_str() + ".html"; }
	string log_path() const { return solution_dir() + "log.csv"; }
private:
	static string instance_dir() { return "../Deploy/Instance/"; }
	static string solution_dir() { return "../Deploy/Solution/"; }
};

// 读取样例，并将样例处理后存起来
class Instance {

public:
	// 下标从 1 开始
	const Environment& _env; // 样例的配置路径
	int* head;				 // [1, node_num]
	Edge* edge;				 // [1, edge_num]
	int node_num, edge_num;
	ll edge_cnt;
public:
	Instance(const Environment& env) : _env(env) { read_instance(); }
	~Instance()
	{
		delete[] head;
		delete[] edge;
	}

private:
	void read_instance() {
		// 重定向到stdin
		// 1069126 1069126 56306653
		if (freopen(_env.instance_path().c_str(), "r", stdin) == NULL) {
			cerr << "Error instance path: can not open " << _env.instance_path() << endl;
			return;
		}
		read(node_num, node_num, edge_num);
		new_memory(node_num, edge_num);
		for (int i = 1, u, v; i <= edge_num; ++i)
		{
			read(u, v);
			add_edge(u, v);
			add_edge(v, u);
		}
	}

	void new_memory(int n, int m)
	{
		head = new int[n + 10]();
		edge = new Edge[ll(m * 2 + 10)]();
		edge_cnt = 0;
	}

	inline void add_edge(int u, int v)
	{
		edge[++edge_cnt] = { u, v, head[u] };
		head[u] = edge_cnt;
	}
};

#endif // !_SRC_INSTANCE_H_

