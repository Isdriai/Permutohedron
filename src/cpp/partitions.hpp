#include <vector>

using namespace std;

struct liste
{
	int elem;
	struct liste *corps;
};

union partie
{
    struct liste elements;
    bool mur; // si partie est un bool, il sera toujorus a true
};

typedef vector<partie> partition;

vector<partition> gen_partitions();

vector<partition> partitions();