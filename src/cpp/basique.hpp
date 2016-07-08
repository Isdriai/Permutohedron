#include <vector>
#include <array>
#include "const.hpp"

using namespace std;

typedef array<int,n> Code;
typedef array<int,n> Permut;

struct Case{
	int lig;
	int co;
};

struct hash_Case{
  size_t operator()(const Case &x) const{
    return std::hash<int>()(x.lig)*100 + std::hash<int>()(x.co);
  }
};

int fact(int x);
void affiche_maho();
int mahonian(int ligne, int col);
void init_maho();
int maho_suiv(int ligne, int col);
vector<Permut> succ(Permut const & elem);
vector<Permut> prec(Permut const & elem);
Code lehmer_to_permut(Permut const & elem);
Permut permut_to_lehmer(Code const & lehm);
Permut next(Permut const & elem);
int ranka(Permut const & elem);
Permut unrank(int etage, int indice);