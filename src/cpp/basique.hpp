#include <vector>

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
void affiche_maho_h(int max);
int mahonian (int ligne, int col);
int mahonian_h (int ligne, int col);
vector<vector<int>> succ (vector<int> elem);
vector<vector<int>> prec (vector<int> elem);
vector<int> lehmer_to_permut(vector<int> elem);
vector<int> permut_to_lehmer(vector<int> lehm);
vector<int> next(vector<int> elem);
int ranka(vector<int> elem);