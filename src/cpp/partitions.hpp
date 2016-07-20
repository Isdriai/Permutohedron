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
Partition unrank(int rang);
Partition separer(Partition const & p, int i);
Partition fusionner(Partition const & p, int i);
void tri_rapide(array<int,n> & suite, int premier, int dernier);
array<vector<int>,n-1> get_operation_Fi();
array<vector<int>,n-1> get_operation_Si();
array<vector<int>,n-1> get_operation_Beg();
void affiche_operation(array<vector<int>,n-1> const & operation);
void init_Fi();
void init_Si();
void init_Beg();
int stirling(int n, int k);