#include <array>
#include <vector>
#include "partitions.hpp"
#include <unordered_set>
#include <list>

typedef list<int> Fonction;

struct hash_fonction
{
	std::size_t operator()(std::list<int> const& fonction) const {
  		std::size_t seed = fonction.size();
  		for(auto& i : fonction) {
   		 	seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		}
  		return seed;
	}
};

void affiche_fonction(Fonction const &f);
array<vector<int>,n-1> get_operation_Fi();
array<vector<int>,n-1> get_operation_Si();
array<vector<int>,n> get_operation_Beg();
void affiche_operation(array<vector<int>,n-1> const & operation);
int stirling(int n, int k);
unordered_set<Fonction, hash_fonction> get_fonctions();
