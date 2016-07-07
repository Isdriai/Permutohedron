#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include "basique.hpp"
#include <array>
#include <sys/time.h>
#include <cilk/cilk.h>
#include <gmpxx.h>
using namespace std;

typedef array<int,n> Code;
typedef array<int,n> Permut;

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
	Permut elem;
};

struct hash_vecteur{
	size_t operator()(const array<int,n> &x) const{
 		
 //  	 std::size_t seed = x.size();
 //    	for(auto& i : x) {
 //     		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
 //  	  }
 //    	return seed;
 // }
		std::size_t seed = ranka(x);
     	return seed;
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

Permut premier(int etage){

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

vector<Noeud> list_etage(int etage){
	vector<Noeud> niveau;
	Noeud first;
	first.elem = premier(etage);
	first.chemin=0;
	niveau.push_back(first);
	int maho = mahonian(n,etage-1);

	for(int i = 0 ; i < maho-1 ; i++){

		first.elem= next(first.elem);
		niveau.push_back(first);
	}
	return niveau;
}

/*vector<Noeud> etage(vector<Noeud> const & etage_fils, int eta){

	vector<Noeud> niveau = list_etage(eta-1); 
	for (Noeud const & noeud : etage_fils)
	{
		vector<array<int,n>> parents = prec(noeud.elem);
		for (array<int,n> const & pre : parents)
		{
			niveau[ranka(pre)].chemin+=noeud.chemin; // a tester copier permutation
		}
	}
	return niveau;
}*/



vector<mpz_class> etage(vector<mpz_class> const & etage_fils, int eta){

	int taille_vect = mahonian(n,eta-2);
	vector<mpz_class> niveau(taille_vect);
	int limite = mahonian(n, eta-1);

	/* ecriture concurrente !
	cilk_for (int i = 0; i < limite; ++i)
	{
		vector<Permut> parents = prec(unrank(eta-1, i));
		for (Permut const & pre : parents)
		{
			niveau[ranka(pre)]+=etage_fils[i];
		}
	}
	*/

	// lecture concurrente seulement -> ca marche 
	cilk_for (int i=0; i < taille_vect; i++){
		vector<Permut> fils = succ(unrank(eta-2, i));
		for (Permut const & su : fils)
		{
			niveau[i]+=etage_fils[ranka(su)];
		}
	}

	return niveau;
}

unordered_map<Permut, longuint, hash_vecteur> etage_ter 
 (unordered_map<Permut, longuint, hash_vecteur> const & etage_fils, int eta){

	unordered_map<Permut, longuint, hash_vecteur> nouveau;
	for (auto const & permut : etage_fils)
	{
		vector<Permut> parents = prec(permut.first);
		for (Permut const & pre : parents)
		{
			auto it = nouveau.find (pre);
			if(it != nouveau.end()){
				longuint tmp = nouveau[pre];
				it->second  = tmp + permut.second; 
			}
			else{
				nouveau[pre]=permut.second;
			}
		}
	}
	return nouveau;
}

/*longuint chaines_max_bis(){

	vector<Noeud> premier_etage;
	Noeud id;
	int etage_actuel = taille;
	id.elem = premier(etage_actuel);
	id.chemin = 1;
	premier_etage.push_back(id);

	while(etage_actuel != 1){
		premier_etage = etage(premier_etage, etage_actuel);
		etage_actuel--;
	}
	return premier_etage[0].chemin;
}*/

mpz_class chaines_max_bis(){

	vector<mpz_class> premier_etage(1);
	int etage_actuel = taille;
	premier_etage[0]=1;

	while(etage_actuel != 1){
		premier_etage = etage(premier_etage, etage_actuel);
		etage_actuel--;
	}
	return premier_etage[0];
}

longuint chaines_max_ter(){
	unordered_map<Permut, longuint, hash_vecteur> premier_etage;
	int etage_actuel = (n*(n-1)/2)+1 ;
	Permut elem = premier(etage_actuel);
	premier_etage[elem]=1;

	while(etage_actuel != 1){
		premier_etage = etage_ter(premier_etage, etage_actuel);
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
	array<int,n> premier_elem;
	int cpt=0;
	for(int i=1 ; i <= n ; i++){
		premier_elem[cpt]=i;
		cpt++;
	}
	return 1+ch(premier_elem);
}

vector<Permut> gen_etage(int etage){
	vector<Permut> niveau;
	Permut first;
	first = premier(etage);
	niveau.push_back(first);
	int maho = mahonian(n,etage-1);

	for(int i = 0 ; i < maho-1 ; i++){

		first= next(first);
		niveau.push_back(first);
	}
	return niveau;
}

void affiche(Permut const & p){
	cout << " nouvelle permutation " << endl ;
	for (int i : p)
	{
		cout << i << " " ;
	}
	cout << endl << endl << endl ;
}

unordered_map<Permut, longuint, hash_vecteur> etage_precedent;

void init_hash(){
	//unordered_map<Permut, longuint, hash_vecteur> nouveau;

	//for (Permut const & p : gen_etage(taille/2+1))
	//{
	//	nouveau[p]=0;
	//}

	unordered_map<Permut, longuint, hash_vecteur> nouveau;
	for (auto const & permut : etage_precedent)
	{
		vector<Permut> parents = prec(permut.first);
		for (Permut const & pre : parents)
		{
			auto it = nouveau.find (pre);
			if(it != nouveau.end()){
				longuint tmp = nouveau[pre];
				it->second  = tmp + permut.second; 
			}
			else{
				nouveau[pre]=permut.second;
			}
		}
	}
}


void init_array(){

	vector<longuint> niveau(mahonian(n,taille/2));
	int limite = mahonian(n, taille/2+1);

	for (int i = 0; i < limite; ++i)
	{
		vector<Permut> parents = prec(unrank(taille/2+1, i));
		for (Permut const & pre : parents)
		{
			niveau[ranka(pre)]+=0;
		}
	}
}

void test_init(void (*fonction_init) ()){
	struct timeval tbegin,tend;
	 double texec=0.;
	 gettimeofday(&tbegin,NULL);
	 
	 for (int i = 0; i < 100; ++i)
	 {
	 	cout << " i : " << i << endl ;
	 	fonction_init();
	 }
	 
	 gettimeofday(&tend,NULL);             
	 texec=((double)(1000*(tend.tv_sec-tbegin.tv_sec)+((tend.tv_usec-tbegin.tv_usec)/1000)))/1000.;

	 std::cout << "temps d'execution : " << texec << endl ;
}

void test_init_hash(){

	int limite = mahonian(n, taille/2+1);
	for (int i = 0; i < limite ; ++i)
	{
		etage_precedent[unrank(taille/2+1, i)]=0;
	}

	cout << " limite " << limite << endl ;

	test_init(init_hash);
}


void test_init_array(){
	test_init(init_array);
}

