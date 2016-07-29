#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include "const.hpp"
#include <array>


using namespace std;

typedef array<int,n> Code;
typedef array<int,n> Permut;

struct Case{
	int lig;
	int co;
};

struct Maho{
	int res;
	int somme_suiv;
};

struct hash_Case{
  size_t operator()(const Case &x) const{
    return std::hash<int>()(x.lig)*100 + std::hash<int>()(x.co);
  }
};

int fact (int x){
	if(x <= 0)
		return 1;
	
	int tmp = 1 ;
	for (int i = x ; i > 1; --i)
	{
		tmp*= i;
	}

	return tmp;
}

bool operator==(Case const& a, Case const& b){
	return a.lig == b.lig && a.co == b.co;
}

bool sup (int a, int b){
	return a > b;
}

bool inf (int a, int b){
	return a < b;
}


array<array<Maho, taille>, n> sav_maho;
/*
	Mahonian fonction

	M(0, 0) = 1
	M(0, c) = 0 for c <> 0
	M(r, c) = Σ(k=0 -> r) M(r - 1, c-k )

*/

int min(int a, int b){
	if (a < b)
		return a;

	return b;
}

void affiche_maho(){
	for (array<Maho, taille> el : sav_maho)
	{
		for (Maho e : el)
		{
			cout << e.res << ","<< e.somme_suiv << " " ;
		}
		cout << endl ;
	}
}

// Attention, le triangle commence avec la ligne indexée par 1, il y a donc un decalage entre 
// la ligne que demande l'utilisateur et le tableau reellement complété

int mahonian(int ligne, int col){
	if (col >= n*(n+1)/2 + 1)
	{
		return 0;
	}
  	return sav_maho[ligne-1][col].res;
}

void init_maho(){
	for (int i = 0 ; i < n ; ++i)
	{
		int s_suiv = fact(i+1);
		
		for (int j = 0; j < taille ; ++j)
		{
			Maho emp;

			if ( j >= i*(i+1)/2 + 1)
			{
				emp.res=0;				
			}
			else if (i == 0)
			{
				emp.res=1;	
			}
			else
			{	
				int tmp = 0;
				for (int k = 0; k <= min(i, j) ; ++k)
				{
					tmp+= mahonian(i,j-k);
				}
				emp.res = tmp;				
			}

			if (s_suiv){
				s_suiv-=emp.res;
			}
			emp.somme_suiv=s_suiv;
			sav_maho[i][j]=emp;
		}
	}
}

int maho_suiv(int ligne, int col){

	return sav_maho[ligne-1][col].somme_suiv;
}

vector<Permut> haut_bas (Permut const & elem, bool (*compare)(int,int)){

	vector<Permut> res;

	for(unsigned int i = 0 ; i < elem.size()-1 ; i++){
		Permut tampon = elem;

		if(compare(elem[i], elem[i+1])){
			tampon[i]=elem[i+1];
			tampon[i+1]=elem[i];
			res.push_back(tampon);
		}
	}
	return res;
}

vector<Permut> succ (Permut const & elem){
	return haut_bas(elem,inf);
}

vector<Permut> prec (Permut const & elem){
	return haut_bas(elem,sup);
}

Code lehmer_to_permut(Permut const & elem){
	array<int,n> tmp;
	int cpt=0;
	for (int i = 0 ; i < n ; i++)
	{
		int compte=0;
		int actuel = elem[i];

		for(int j = i ; j < n ; j++ ){

			if (elem[j] < actuel){
				compte++;
			}
		}
		tmp[cpt]=compte;
		cpt++;
	}
	return tmp;
}

int ajustement(array<bool,n> & possibles, int index){
	for (int i = 0; i < n ; ++i)
	{
		if(index <= 0 && possibles[i]){
			possibles[i]=false;
			return i+1;
		}
		if(possibles[i])
		{
			index--;
		}
	}
	return 0; // ne doit pas arriver
}

Permut permut_to_lehmer(Code const & lehm){
	array<bool,n> possibles;

	for (int i = 0; i < n ; ++i)
	{
		possibles[i]= true;
	}
	array<int,n> traduction;

	int cpt=0;
	for (int i : lehm)
	{
		int ajust= ajustement(possibles, i);
		traduction[cpt]=ajust;
		cpt++;
	}
	return traduction;
}

// comportement incertain si l'élément n'a pas de suivant !
Permut next(Permut const & elem){

	array<int,n> lehm = lehmer_to_permut(elem);
	int ramassage=0;
	int pivot=0;
	for (int i = n-2; i > 0; i--)
	{
		if(lehm[i] > 0 && lehm[i-1] < n-i){
			ramassage+= lehm[i]-1;
			lehm[i]=0;
			lehm[i-1]++;
			pivot = i;
			break;
		}
		else{
			ramassage+= lehm[i];
			lehm[i]=0;
		}
	}
	if (ramassage){
		for (int i = n-2; i >= pivot; i--)
		{
			if(ramassage >= n-1-i){
				lehm[i]=n-1-i;
				ramassage-=n-1-i;
			}
			else if (ramassage > 0){
				lehm[i]=ramassage;
				break;
			}
		}
	}
	return permut_to_lehmer(lehm);
}

int cpt(array<int,n> const & lehm){
	int tmp = 0 ;
	for (int i : lehm )
	{
		tmp+=i;
	}
	return tmp;
}

int ranka(Permut const & elem){
	Code lehm = lehmer_to_permut(elem);
	int p = cpt(lehm);
	int niv = n;
	int min = 0;
	for(int i : lehm){
		min+=maho_suiv(niv-1, p-i)-maho_suiv(niv-1, p);
		p-=i;
		niv--;
		if (!p){
			return min;
		}
	}
	return 0 ;
}

Permut unrank(int etage, int indice){
	Code lehm;

	for (int i = 0; i < n; i++)
	{
		int decalage = 0;
		while(indice >= mahonian(n-i, etage-decalage)){
			indice-= mahonian(n-i, etage-decalage);
			decalage++;
		}
		lehm[i-1]=decalage;
		etage-=decalage;
	}
	lehm[n-1] = 0;

	return permut_to_lehmer(lehm);
}