#include <vector>
#include <array>
#include "const.hpp"


using namespace std;


struct Case{
	int lig;
	int co;
};

struct hash_Case{
  size_t operator()(const Case &x) const{
    return std::hash<int>()(x.lig)*100 + std::hash<int>()(x.co);
  }
};

void affiche_maho();
int mahonian(int ligne, int col);
void init_maho();
int maho_suiv(int ligne, int col);
vector<array<int,n>> succ(array<int,n> const & elem);
vector<array<int,n>> prec(array<int,n> const & elem);
array<int,n> lehmer_to_permut(array<int,n> const & elem);
array<int,n> permut_to_lehmer(array<int,n> const & lehm);
array<int,n> next(array<int,n> const & elem);
int ranka(array<int,n> const & elem);