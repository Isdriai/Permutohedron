#include <vector>
#include <array>
#include "basique.hpp"

using namespace std;

struct Partition
{
	array<int,n> suite;
	array<bool,n-1> barres;
};

void affiche_partition(Partition p);
vector<Partition> get_partitions();
void gen_partitions
(vector<Partition> &partitions, array<int,n> generateur,int gen, array<bool,n> possibles, Partition acc, int fait);
//int get_partitions();
void init_multimoniaux();
int ranka(Partition const & p);