#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include "const.hpp"
#include <thread>

using namespace std;

struct Noeud{
	unsigned long long int chemin;
	vector<int> elem;
};

struct Case{
	int lig;
	int co;
};

struct hash_Case{
  size_t operator()(const Case &x) const{
    return std::hash<int>()(x.lig)*100 + std::hash<int>()(x.co);
  }
};


bool operator==(Case const& a, Case const& b){
	return a.lig == b.lig && a.co == b.co;
}

bool sup (int a, int b){
	return a > b;
}

bool inf (int a, int b){
	return a < b;
}

/*
	Mahonian fonction

	M(0, 0) = 1
	M(0, c) = 0 for c <> 0
	M(r, c) = Σ(k=0 -> r) M(r - 1, c-k )

*/

unordered_map<Case, int, hash_Case> sav_maho;

int min(int a, int b){
	if (a < b)
		return a;

	return b;
}

int mahonian_h(int ligne, int col){
	
	Case tmp;
	tmp.lig=ligne;
	tmp.co=col;

	unordered_map<Case,int,hash_Case>::const_iterator got = sav_maho.find(tmp);

	try {
	 	return sav_maho.at(tmp);
	 }
	catch(exception e){

		if(tmp.lig == 0){
			if(tmp.co == 0){

				sav_maho[tmp]=1;
				return 1;
			}
		sav_maho[tmp]=0;
		return 0;
		}

		else {

			int res = 0;
			for (int i = 0; i < ligne; ++i)
			{
				res+= mahonian_h(ligne-1, col-i);
			}

			sav_maho[tmp]=res;
			return res;
		}
	}
}

vector<vector<int>> sav_mahonian{{0}};

void affiche_maho(){
	for ( vector<int> etage : sav_mahonian){
		for (int i : etage )
		{
			cout << i << " " ;
		}
		cout << endl;
	}
}

void construire(int ligne);

// ne veut pas retourner la bonne valeur au premier appel
 int mahonia(int ligne, int col){
  	//affiche_maho();
  	//cout << " mahonian     ligne  " << ligne << "  col " << col << endl << endl << endl << endl ;
  	if (col >= (ligne*(ligne-1)/2)+1 )
  	{
  		return 0;
  	}

  	try {
  		return sav_mahonian.at(ligne).at(col);
  	}
  	catch (exception e){
  		construire(ligne);
  		mahonia(ligne,col);
  	}
  }

int mahonian(int ligne, int col){
	mahonia(ligne, col);
	return mahonia(ligne, col);
}

void construire(int ligne){
	//cout << "contruire   ligne " << ligne << endl ;
	if(ligne == 1){
		sav_mahonian.push_back(vector<int>{1});
	}
	else{
		int nbr_cases = (ligne*(ligne-1)/2)+1;
		vector<int> etage(nbr_cases);

		for (int i = 0; i < nbr_cases; ++i)
		{
		
			int tmp = 0 ;

			for (int j = 0; j <= min(i, ligne-1); ++j)
			{
				int acces = i-j;

				tmp+= mahonian(ligne-1, acces);
				
			}

			etage[i]=tmp;
		}

		//cout << " etage " << ligne << " ok " << endl ;
		sav_mahonian.push_back(etage);
	}
}

vector<vector<int>> haut_bas (vector<int> elem, bool (*compare)(int,int)){

	std::vector<vector<int>> res;

	for(int i = 0 ; i < elem.size()-1 ; i++){
		vector<int> tampon (elem);

		if(compare(elem[i], elem[i+1])){
			tampon[i]=elem[i+1];
			tampon[i+1]=elem[i];
			res.push_back(tampon);
		}
	}

	return res;
}

vector<vector<int>> succ (vector<int> elem){
	return haut_bas(elem,inf);
}

vector<vector<int>> prec (vector<int> elem){
	return haut_bas(elem,sup);
}

vector<int> lehmer_to_permut(vector<int> elem){
	vector<int> tmp;

	for (int i = 0 ; i < n ; i++)
	{
		int compte=0;
		int actuel = elem[i];

		for(int j = i ; j < n ; j++ ){

			if (elem[j] < actuel){
				compte++;
			}
		}

		tmp.push_back(compte);
	}

	return tmp;
}

vector<int> permut_to_lehmer(vector<int> lehm){

	vector<int> possibles;


	for (int i = 1; i <= n ; i++)
	{
		possibles.push_back(i);
	}



	vector<int> traduction;

	for (int i : lehm)
	{

		traduction.push_back(possibles[i]);

		possibles.erase(possibles.begin() + i );
	}



	return traduction;
}

vector<int> next(vector<int> elem){

	vector<int> lehm = lehmer_to_permut(elem);


	int ramassage=0;
	int pivot;

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

int cpt(vector<int> lehm){
	int tmp = 0 ;
	for (int i : lehm )
	{
		tmp+=i;
	}

	return tmp;
}

int ranka(vector<int> elem){
	vector<int> lehm = lehmer_to_permut(elem);

	int p = cpt(lehm);
	int niv = n;
	int min = 0;

	for(int i : lehm){

		for (int j = 0 ; j < i ; j++ )
		{
			min+= mahonian(niv-1, p-j);
		}

		p-=i;
		niv--;

		if (!p){
			return min;
		}
	}

	return 0 ;
}