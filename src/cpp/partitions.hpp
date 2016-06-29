#include <vector>
#include <array>
#include "const.hpp"



using namespace std;

struct Partition
{
	array<int,n> suite;
	array<bool,n-1> barres;
};

//vector<partition> gen_partitions();

vector<Partition> get_partitions();