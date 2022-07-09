#ifndef _SRC_CONFIG_H_
#define _SRC_CONFIG_H_
#include <random>
static constexpr int INF = 0x3f3f3f3f;
static constexpr long long INF_LL = 0x3f3f3f3f3f3f3f3f;
static constexpr long long mod = 1e9 + 7;
static constexpr double eps = 1e-5;


using coord_t = int;


static const char* ins_list[]{
	"",
	"",
	""
};

struct Config {
	unsigned int random_seed;
	int timeout = 3600;  // ³¬Ê±Ê±¼ä
	int delta = 1;
	Config():timeout(3600)
	{
		random_seed = std::random_device{}();
	}
	Config(int timeout): timeout(timeout)
	{
		random_seed = std::random_device{}();
	}
	Config(int timeout, unsigned int seed) : timeout(timeout), random_seed(seed) {};
} cfg;
#endif // !_SRC_CONFIG_H_