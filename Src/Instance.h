#ifndef _SRC_INSTANCE_H_
#define _SRC_INSTANCE_H_

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include "Utils.h"
#include "Config.h"
using namespace std;

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

private:
	const Environment& _env; // 样例的配置路径

public:
	Instance(const Environment& env) : _env(env) { read_instance(); }


private:
	void read_instance() {
		ifstream ifs(_env.instance_path());
		if (!ifs.is_open()) {
			cerr << "Error instance path: can not open " << _env.instance_path() << endl;
			return;
		}
	}

	
};

#endif // !_SRC_INSTANCE_H_

