#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <unordered_map>

using namespace std;

int n ;

struct Noeud{
	int chemin;
	vector<int> elem;
};

struct Case{
	int lig;
	int co;
};

struct hash_Case{
  size_t operator()(const Case &x) const{
    return std::hash<int>()(x.lig) ^ std::hash<int>()(x.co);
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

// regarder comment memoiser ce truc !!!!
int mahonian (int ligne, int col){
	
	// cout << "maho " << ligne << "  " << col << endl ;

	 Case tmp;
	 tmp.lig=ligne;
	 tmp.co=col;

	unordered_map<Case,int,hash_Case>::const_iterator got = sav_maho.find (tmp);

	 if(got != sav_maho.end()) {
	 	return sav_maho[tmp];
	 }
	 else{

		if(ligne == 0){
			if(col == 0){

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
				res+= mahonian(ligne-1, col-i);
			}

			 sav_maho[tmp]=res;
			return res;
		}
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

	int maho = mahonian(n,etage-1);

	for(int i = 0 ; i < maho - 1 ; i++){


		niveau.push_back(suiv);


		first.elem= suiv.elem;


		suiv.elem= next(first.elem);

	}

	return niveau;
}

vector<Noeud> etage(vector<Noeud> etage_fils, int eta){


	// génération de l'étage precedent

	vector<Noeud> niveau = list_etage(eta-1); 

	// calcul des chemins
	// on pourrait peut etre aller plus vite avec une table de hash ? 
	// ca limiterait le tps passé a retrouver les noeuds correspondant aux peres du noeud actuel

	for (Noeud const & noeud : etage_fils)
	{
		vector<vector<int>> parents = prec(noeud.elem);

		for (vector<int> const & pre : parents)
		{


			niveau[ranka(pre)].chemin+=noeud.chemin;
			// for (Noeud & precedent : niveau)
			// {
			// 	if(egale(precedent.elem,pre)){
			// 		precedent.chemin+= noeud.chemin;
			// 		break;
			// 	}
			// }
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

	while(etage_actuel != 1){

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


	for (int i = 1; i < 8 ; ++i)
	{
		n=i;
		cout << chaines_max_bis() << endl ;

	}

	return 0;
}