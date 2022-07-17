#ifndef _SRC_SOLVE_H_
#define _SRC_SOLVE_H_

#include "Instance.h"
#include "Utils.h"

using namespace Debug;

class Solve {

private:
	const Environment& _env;
	const Config& _cfg;

	Instance _ins;
	default_random_engine _gen;
	Timer timer;

	SetInt best_sol, sol, tmp_sol;
	SetInt NI, tmp_NI;
	int* NE_size, *tmp_NE_size;
	int* NE_only, *tmp_NE_only;
	int* node;
	int* tabu;
	int* NM;
	int NM_size;
	// NM先直接算，不记录试试
	int record;

	double _duration; // 最优解出现时间
	int _iteration;   // 最优解出现迭代次数


public:

	Solve() = delete;

	Solve(const Environment& env, const Config& cfg) :
		_env(env), _cfg(cfg), _ins(env),
		_gen(_cfg.random_seed), timer(cfg.timeout),
		_duration(0), _iteration(0), record(INF), NI(_ins.node_num + 10),
		tmp_NI(_ins.node_num + 10), best_sol(_ins.node_num + 10), 
		sol(_ins.node_num + 10), tmp_sol(_ins.node_num + 10),
		NM_size(0)
		// 把 _obj_area初始化为最大值
	{
		// 初始化变量
		NE_size = new int[_ins.node_num + 10]();
		node = new int[_ins.node_num + 10]();
		tabu = new int[_ins.node_num + 10]();
		NE_only = new int[_ins.node_num + 10]();
		NM = new int[_ins.node_num + 10]();
		tmp_NE_size = new int[_ins.node_num + 10]();
		tmp_NE_only = new int[_ins.node_num + 10]();
		for (int i = 1; i <= _ins.node_num; ++i)
		{
			sol.insert(i);
			NI.insert(i);
			node[i] = i;
		}
	}

	~Solve()
	{
		delete[] NE_size;
		delete[] node;
		delete[] tabu;
		delete[] NE_only;
		delete[] NM;
		delete[] tmp_NE_size;
		delete[] tmp_NE_only;
	}

	void init_solution()
	{
		while (!NI.empty())
		{
			int u = -1, min_deg = INF;
			if (_ins.avg_deg < NI.size())
			{
				for (int i = 1, j; i <= _ins.avg_deg; ++i)
				{
					j = rand(i, NI.size() + 1);
					NI.swap(NI[i], NI[j]);
					if (min_deg > _ins.deg[NI[i]])
					{
						u = NI[i];
						min_deg = _ins.deg[u];
					}
				}
			}
			else
			{
				for (int i = 1; i <= NI.size(); ++i)
				{
					int select_node = NI[i];
					if (_ins.deg[select_node] < min_deg)
					{
						u = select_node;
						min_deg = _ins.deg[select_node];
					}
				}
			}
			sol_erase_update(u);
		}
	}

	inline void sol_erase_update(int u)
	{
		sol.erase(u);
		NI.erase(u);
		NE_size[u] = 0;
		NE_only[u] = 0;
		// 更新 NI,NE，删去了结点 u
		for (int i = _ins.head[u]; i; i = _ins.edge[i].next)
		{
			int v = _ins.edge[i].v; // v in sol, u not in sol
			if (sol.exist(v))
			{
				NE_size[v]++;
				NI.erase(v);
				NE_only[v] = u;
				// 这里可以更新 NM
			}
		}
	}

	inline void sol_insert_update(int u)
	{
		sol.insert(u); // 插入了 u 
		NE_size[u] = 0;
		NE_only[u] = 0;
		for (int i = _ins.head[u]; i; i = _ins.edge[i].next)
		{
			int v = _ins.edge[i].v;
			if (!sol.exist(v))
			{
				NE_size[u]++;
				NE_only[u] = v;
			}
			else
			{
				NE_size[v]--;
				if (NE_size[v] == 0)
				{
					NI.insert(v);
					NE_only[v] = 0;
				}
				else if (NE_only[v] == u)
				{
					// 要确定 NE(v)_only 是啥
					for (int j = _ins.head[v]; j; j = _ins.edge[j].next)
					{
						int nev = _ins.edge[j].v;
						if (!sol.exist(nev))
						{
							NE_only[v] = nev;
							break;
						}
					}
				}
			}
		}
		if (NE_size[u] == 0) NI.insert(u);
	}

	void run()
	{
		int iter = 0;
		init_solution();
		best_sol = sol;
		_duration = timer.getDuration();
		_iteration = iter;
		record = best_sol.size();
		while (!timer.isTimeout() && best_sol.size() != 864052)
		{
			iter++;
			random_shuffle(node + 1, node + 1 + _ins.node_num);
			for (int i = 1, u, v; i <= _ins.node_num; ++i)
			{
				v = node[i];
				if (sol.exist(v))
				{
					if (NE_size[v] == 0)
					{
						sol_erase_update(v);
						if (sol.size() < best_sol.size())
						{
							best_sol = sol;
							_iteration = iter;
							_duration = timer.getDuration();
						}
					}
					else if (NE_size[v] == 1 && tabu[v] < iter)
					{
						u = NE_only[v];
						sol_erase_update(v);
						sol_insert_update(u);
						tabu[u] = iter;
					}
				}
				else
				{
					// 求解一下 NM
					NM_size = 0;
					for (int j = _ins.head[v]; j; j = _ins.edge[j].next)
					{
						u = _ins.edge[j].v;
						if (sol.exist(u) && NE_size[u] == 1)
						{
							NM[++NM_size] = u;
						}
					}
					if (NM_size > 0 && tabu[v] < iter)
					{
						u = NM[rand(NM_size)];
						sol_erase_update(u);
						sol_insert_update(v);
						tabu[v] = iter;
					}
					else if (sol.size() + 1 <= record + cfg.delta)
					{
						sol_insert_update(v);
					}
				}
			}
			record = best_sol.size();
			tmp_sol = sol;
			tmp_NI = NI;
			for (int j = 1; j <= _ins.node_num; ++j)
			{
				tmp_NE_size[j] = NE_size[j];
				tmp_NE_only[j] = NE_only[j];
			}
			int u = 0;
			while (!NI.empty())
			{
				u = NI[rand(NI.size())];
				sol_erase_update(u);
			}
			if (sol.size() < best_sol.size())
			{
				best_sol = sol;
				_iteration = iter;
				_duration = timer.getDuration();
			}
			if (rand(0, 2) == 1) record = best_sol.size();
			else
			{
				sol = tmp_sol;
				NI = tmp_NI;
				for (int j = 1; j <= _ins.node_num; ++j)
				{
					NE_size[j] = tmp_NE_size[j];
					NE_only[j] = tmp_NE_only[j];
				}
			}
		}
	}

	// 将结果写入文件
	void record_sol(const string& sol_path) const
	{
		if (freopen(sol_path.c_str(), "w", stdout) == NULL) 
		{
			cerr << "Error solution path: can not open " << sol_path << endl;
			return;
		}
		record_sol();
	}

	void record_sol() const
	{
		cout << best_sol.size() << endl;
		for (auto& u : best_sol)
		{
			cout << u << " ";
		}
		cout << endl;
	}

	// 可视化检查
	void show_sol(SetInt& sol) const
	{
		for (auto& x : sol)
		{
			cout << x << " ";
		}
		cout << endl;
	}

	// 写入日志文件，不传参数为写入标准错误流
	void record_log(const string& log_path)
	{
		if (freopen(log_path.c_str(), "a", stderr) == NULL)
		{
			cerr << "Error log path: can not open " << log_path << endl;
			return;
		}
		ofstream log_file(log_path, ios::app);
		log_file.seekp(0, ios::end);
		if (log_file.tellp() <= 0) { record_log_header(); }
		record_log_data();
	}

	// 写入标准错误流
	void record_log()
	{
		//record_log_header();
		record_log_data();
	}


	/// 检查结果是否正确
	bool check()
	{
		for (int i = 1; i <= _ins.edge_num; i = i + 2)
		{
			auto& edge = _ins.edge[i];
			if (!best_sol.exist(edge.u) && !best_sol.exist(edge.v))
			{
				cerr << "求解失败，存在不能覆盖的边" << endl;
				return false;
			}
		}
		return true;
	}
private:
	void record_log_header()
	{
		writeStream("date", "instance", "delta", "obj_node_num", "randomSeed",
			"obj_iterTimes", "obj_time", "all_time");
	}

	void record_log_data()
	{
		writeStream(Date::to_format_str(), _env.instance_name(), cfg.delta, best_sol.size(), _cfg.random_seed,
			_iteration, _duration, timer.getDuration());
	}

	int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(_gen); } // [lb, ub - 1]

	int rand(int ub) { return uniform_int_distribution<int>(1, ub)(_gen); } // [1, ub]

	//double rand() { return uniform_real_distribution<double>(0, 1)(_gen); }

};
#endif // !_SRC_SOLVE_H_

