#include "Config.h"
#include "Instance.h"
#include "Solve.h"

void run_single_instance(const Environment& env)
{
	
	Solve solve(env, cfg);
	solve.run();

	solve.record_log();

	solve.record_sol();
}

int main(int argc, char* argv[])
{
	//exename instance_name, delta, randseed, timeout
	if (argc == 5) 
	{
		string ins_name = argv[1];
		Environment env(ins_name);
		cfg.delta = atoi(argv[2]);
		cfg.random_seed = atoi(argv[3]);
		cfg.timeout = atoi(argv[4]);
		run_single_instance(env);
	}
	else 
	{
		//string ins_name = "ca-hollywood-2009";
		string ins_name = "tech-RL-caida";
		//string ins_name = "test";
		cfg.random_seed = 100;
		cfg.timeout = 10;
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
