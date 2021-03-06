#include "Config.h"
#include "Instance.h"
#include "Solve.h"

void run_single_instance(const Environment& env)
{
	Solve solve(env, cfg);
	solve.run();
	solve.record_log();
	solve.check();
	solve.record_sol();
}

int main(int argc, char* argv[])
{
	// exename instance_name, timeout, randseed, 参数1, 参数2, 参数n
	if (argc == 5) 
	{
		string ins_name = argv[1];
		Environment env(ins_name);
		cfg.delta = atoi(argv[4]);
		cfg.random_seed = atoi(argv[3]);
		cfg.timeout = atoi(argv[2]);
		run_single_instance(env);
	}
	else 
	{
		string ins_name = "ca-hollywood-2009";
		//string ins_name = "ca-CSphd";
		//string ins_name = "sc-pkustk13";
		//string ins_name = "in";
		cfg.random_seed = 11076;
		cfg.timeout = 1000;
		cfg.delta = 1;
		Environment env(ins_name);
		if (freopen(env.instance_path().c_str(), "r", stdin) == NULL) {
			cerr << "Error instance path: can not open " << env.instance_path() << endl;
			return -1;
		}
		if (freopen(env.solution_path().c_str(), "w", stdout) == NULL)
		{
			cerr << "Error solution path: can not open " << env.solution_path() << endl;
			return -1;
		}
		run_single_instance(env);
	}
	
	return 0;
}
