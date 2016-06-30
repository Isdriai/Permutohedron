#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include "basique.hpp"
#include <array>

using namespace std;

typedef __uint128_t longuint;

// There is no overload for << for __uint128_t
std::ostream& operator<<( std::ostream& dest, longuint value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        longuint tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

struct Noeud{
	longuint chemin;
	array<int,n> elem;
};

struct hash_vecteur{
	size_t operator()(const array<int,n> &x) const{
 		
  		 size_t tmp = 0;

 		 for (int i = 0 ; i < x.size() ; i++)
 		 {
 		 	tmp += (x[i])*(i+1);
 		 }
 		 return tmp;

 		// std::size_t seed = x.size();
  	// 	for(auto& i : x) {
   //  		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
 		//  }
  	// 	return seed;
	}
};

int somme (int e){

	if(!e)
		return 1;

	int tmp = 0 ;

	for(int i = e; i > 0 ; i-- ){
		tmp+= i;
	}

	return tmp;
}

array<int,n> premier(int etage){

	array<int,n> premier;

	for (int i = 0; i < n; ++i)
	{
		premier[i] = 0;
	}

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

bool egale(array<int,n> const & a, array<int,n> const & b){
	for(int i = 0 ; i < n ; i++){
		if(a[i] != b[i]){
			return false;
		}
	}

	return true;
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

	int maho = mahonian(n+1,etage-1);

	for(int i = 0 ; i < maho - 1 ; i++){


		niveau.push_back(suiv);


		first.elem= suiv.elem;


		suiv.elem= next(first.elem);

	}

	return niveau;
}

vector<Noeud> etage(vector<Noeud> const & etage_fils, int eta){

	vector<Noeud> niveau = list_etage(eta-1); 


	for (Noeud const & noeud : etage_fils)
	{
		vector<array<int,n>> parents = prec(noeud.elem);

		for (array<int,n> const & pre : parents)
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

unordered_map<array<int,n>, longuint, hash_vecteur> etage_ter 
 (unordered_map<array<int,n>, longuint, hash_vecteur> const & etage_fils, int eta){

	unordered_map<array<int,n>, longuint, hash_vecteur> nouveau;

	for (auto const & permut : etage_fils)
	{

		vector<array<int,n>> parents = prec(permut.first);

		for (array<int,n> const & pre : parents)
		{
			auto it = nouveau.find (pre);

			if(it != nouveau.end()){
				longuint tmp = nouveau[pre];
				it->second  = tmp + permut.second; // euh c'est pas la qu'il y a un pb avec les longuint ? 
			}
			else{
				nouveau[pre]=permut.second;
			}
		}
	}
	return nouveau;
}

longuint chaines_max_bis(){

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

longuint chaines_max_ter(){

	unordered_map<array<int,n>, longuint, hash_vecteur> premier_etage;

	int etage_actuel = (n*(n-1)/2)+1 ;

	array<int,n> elem = premier(etage_actuel);

	premier_etage[elem]=1;

	while(etage_actuel != 1){

		premier_etage = etage_ter(premier_etage, etage_actuel);

		// cout << " affiche etage " << endl << endl ;

		// cout << "elem " << endl ;

		// for(auto kv : premier_etage) {
  //   		for (int i : kv.first)
  //   		{
  //   			cout << i ;
  //   		}

  //   		cout << endl ;

  //   		cout << "chemin " <<  kv.second << endl ; 
  //   	}

		etage_actuel--;
	}

	// ultra moche mais c'est bizarre, en faisant premier_etage[id()] il ne retrouve pas la valeur
	for (auto kv : premier_etage)
	{
		return kv.second;
	}
	return 0;
}

int ch (array<int,n> const & elem){

	vector<array<int,n>> possibles = succ(elem);

	int nbr_suiv = possibles.size();

	if(nbr_suiv == 0){
		return 0;
	}

	int rajoute=nbr_suiv-1;

	for(array<int,n> suiv : possibles){
		rajoute += ch(suiv);
	}

	return rajoute;
}

int chaines_max(){
	std::array<int,n> premier_elem;

	int cpt=0;

	for(int i=1 ; i <= n ; i++){
		premier_elem[cpt]=i;
		cpt++;
	}

	return 1+ch(premier_elem);
}