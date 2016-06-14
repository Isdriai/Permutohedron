#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>

using namespace std;

int n = 4;

struct Noeud
{
	int chemin;
	vector<int> elem;
};

bool sup (int a, int b){
	return a > b;
}

bool inf (int a, int b){
	return a < b;
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

int somme (int e){

	if(!e)
		return 1;

	int tmp = 0 ;

	for(int i = e; i > 0 ; i-- ){
		tmp+= i;
	}

	return tmp;
}

vector<int> premier(int etage){

	vector<int> premier (n,0);
	int reste = etage-1; // l'etage 1 a 0 permutation

	for (int i = 1; i < n; ++i)
	{

		if(reste > i){
			premier[n-i-1]=i;
			reste-=i;
		}
		else {
			premier[n-i-1]=reste;
			break;
		}
	}

	return permut_to_lehmer(premier);
}

bool egale(vector<int> a, vector<int> b){
	for(int i = 0 ; i < n ; i++){
		if(a[i] != b[i]){
			return false;
		}
	}

	return true;
}

void affiche_etage ( vector<Noeud> et){

	for (Noeud n : et )
	{
		for (int i : n.elem)
		{
			cout << i ;
		}
		cout << endl << " chemin " << n.chemin << endl << endl << endl;
	}
}

vector<Noeud> list_etage(int etage){
	vector<Noeud> niveau;

	Noeud first;

	first.elem = premier(etage);

	first.chemin=0;

	Noeud suiv;


	suiv.elem = next(first.elem);
	suiv.chemin = 0;

	niveau.push_back(first);

	while(!egale(first.elem, suiv.elem)){


		niveau.push_back(suiv);


		first.elem= suiv.elem;


		suiv.elem= next(first.elem);

	}

	return niveau;
}

vector<Noeud> etage(vector<Noeud> v, int eta){


	// génération de l'étage precedent

	vector<Noeud> niveau = list_etage(eta-1); 

	// calcul des chemins
	// on pourrait peut etre aller plus vite avec une table de hash ? 
	// ca limiterait le tps passé a retrouver les noeuds correspondant aux peres du noeud actuel

	for (Noeud n : v)
	{
		vector<vector<int>> parents = prec(n.elem);

		for (vector<int> pre : parents)
		{
			for (Noeud precedent : niveau)
			{
				if(precedent.elem == pre){
					precedent.chemin+= n.chemin;
					break;
				}
			}
		}
	}

	return niveau;
}

int chaines_max_bis(){

	vector<Noeud> premier_etage;
	Noeud id;

	int etage_actuel = (n*(n-1)/2)+1 ;

	id.elem = premier(etage_actuel);
	id.chemin = 1;

	premier_etage.push_back(id);

	while(etage_actuel != 0){

		premier_etage = etage(premier_etage, etage_actuel);

		etage_actuel--;
	}

	return premier_etage[0].chemin;
}

int ch (vector<int> elem){

	vector<vector<int>> possibles = succ(elem);

	int nbr_suiv = possibles.size();

	if(nbr_suiv == 0){
		return 0;
	}

	int rajoute=nbr_suiv-1;

	for(vector<int> suiv : possibles){
		rajoute += ch(suiv);
	}

	return rajoute;
}

int chaines_max(){
	std::vector<int> premier_elem;

	for(int i=1 ; i <= n ; i++){
		premier_elem.push_back(i);
	}

	return 1+ch(premier_elem);
}

int main(int argc, char const *argv[]){
	 std::vector<int> test ({3,2,1,4});

	 // vector<vector<int>> infe = succ(test);

	 // vector<vector<int>> supe = prec(test);

	 // cout << chaines_max_bis() << endl ;

	  affiche_etage(list_etage(4));


	  // for (int i : next(test))
	  // {
	  // 	cout << i ;
	  // }

	  // cout << endl ;

	return 0;
}