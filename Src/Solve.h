#ifndef _SRC_SOLVE_H_
#define _SRC_SOLVE_H_

#include "Instance.h"
#include "Utils.h"

using namespace utils;
using namespace fast_io;
class Solve {

private:
	const Environment& _env;
	const Config& _cfg;

	Instance _ins;
	default_random_engine _gen;
	Timer timer;

	bool* is_select;  // 是否选中为顶点覆盖
	int* solution;	  // 顶点覆盖的解

	double _duration; // 最优解出现时间
	int _iteration;   // 最优解出现迭代次数


public:

	Solve() = delete;

	Solve(const Environment& env, const Config& cfg) :
		_env(env), _cfg(cfg), _ins(env), _gen(_cfg.random_seed), timer(cfg.timeout),
		_duration(0), _iteration(0) 
		// 把 _obj_area初始化为最大值
	{
		is_select = new bool[_ins.node_num + 10]();
		solution = new int[_ins.node_num + 10]();

	}

	void run() {
		while (!timer.isTimeout()) {
			
		}
	}

	// 将结果写入文件
	void record_sol(const string& sol_path) const {

	}
	// 可视化检查
	void draw_sol(const string& html_path) const {

	}
	// 写入日志文件
	void record_log() {
		// 写入表头
		writeLog(_env.log_path(), LineType::HEAD, "instance", "randomSeed", "iterTimes", "time");
		// 写入行数据
		writeLog(_env.log_path(), LineType::LINE, _env.instance_name(), _cfg.random_seed, _iteration, timer.getDuration());
	}

private:

	/// 检查结果是否正确
	void check() {
		for (int i = 1; i <= _ins.edge_num; i = i + 2)
		{
			auto& edge = _ins.edge[i];
			if (!is_select[edge.u] && !is_select[edge.v])
			{
				cerr << "求解失败，存在不能覆盖的边" << endl;
				return;
			}
		}
	}


};
#endif // !_SRC_SOLVE_H_

