#include "Config.h"
#include "Instance.h"
#include "Solve.h"

void run_single_instance(const string& ins_str)
{
	Environment env(ins_str);
	Solve solve(env, cfg);
	solve.run();

	solve.record_log(env.log_path());

	solve.check();
	solve.record_sol();
}

int main(int argc, char* argv[])
{
	//exename instance_name, randseed, timeout
	string ins_name;
	if (argc == 4) 
	{
		ins_name = argv[1];
		cfg.random_seed = atoi(argv[2]);
		cfg.timeout = atoi(argv[3]);
	}
	else 
	{
		//string ins_name = "ca-hollywood-2009";
		string ins_name = "ca-MathSciNet";
		//string ins_name = "test";
		cfg.random_seed = 100;
		cfg.timeout = 5;
	}
	run_single_instance(ins_name);
	return 0;
}
