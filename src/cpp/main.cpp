/*

IMPORTANT, voici la ligne de compilation pour le programme

g++ -std=c++11 -fcilkplus src/cpp/* -O3 -Wall -o bin/exe -lgmp -lgmpxx

il faut absolument mettre une option -Ox sinon il y a un segfault,
malheureusement je n'arrive pas à le gérer, ni avec -Wall/-Wextra ni avec gdb

*/
#include <iostream>
#include <stdlib.h>
#include "chaine.hpp"
#include "sous_monoide.hpp"
#include <cilk/cilk_api.h>

using namespace std;

void affiche_part(Partition const & p){
	for (int i = 0; i < n; ++i)
	{
		cout << p.suite[i] << " ";
	}
	cout << endl ;

	for (int i = 0; i < n-1; ++i)
	{
		cout << p.barres[i] << " ";
	}
	cout << endl;
}

template <size_t N>
void affiche_array(array<int,N> const & tab){
	for (int i = 0; i < N; i++)
	{
		cout << tab[i] << " " ;
	}

	cout << endl ;
}

void affichep(Permut const & p){
	cout << " nouvelle permutation " << endl ;
	for (int i : p)
	{
		cout << i << " " ;
	}
	cout << endl << endl << endl ;
}

void show_usage(string name)
{
  cerr << "Usage: " << name << " [-n <proc_number>] " << endl;
}

template <size_t N>
bool egale(array<int,N> const & a, array<int,N> const & b){
	for (unsigned int i = 0; i < N; ++i)
	{
		if (a[i]!=b[i])
		{
			return false;
		}
	}
	return true;
}

template <size_t N>
bool egale(array<bool,N> const & a, array<bool,N> const & b){
	for (unsigned int i = 0; i < N; ++i)
	{
		if (a[i]!=b[i])
		{
			return false;
		}
	}
	return true;
}

bool egale(Partition const & a, Partition const & b){
	return egale(a.suite, b.suite) && egale(a.barres, b.barres);
}

int main(int argc, char const *argv[]){

	init_maho();

	
	string nproc = "0";

 	if (argc != 1 and argc != 3) { show_usage(argv[0]); return 1; }
 	if (argc == 3)
    {
     	 if (string(argv[1]) != "-n")  { show_usage(argv[0]); return 1; }
      nproc = argv[2];
    }

	if (__cilkrts_set_param("nworkers", nproc.c_str() ) != __CILKRTS_SET_PARAM_SUCCESS)
   		 cerr << "Failed to set the number of Cilk workers" << endl;

  	cout << " cpu utilisés " << __cilkrts_get_nworkers() << endl;


	//mpz_class res = chaines_max_bis();
	//	 cout << res << endl ;
	

	/*
  	vector<Partition> partitions = get_partitions();

  	int i = 0;
  	for (Partition p : partitions)
  	{
  		cout << "partition n°" << i << endl;
  		cout << " rank " ;
  		if(i==ranka(p)){

  			cout << "true "<< endl;
  		}
  		else{

  			cout << "false" << endl;
  		}

  		cout << " unrank " ;
  		if(egale(p,unrank(i))){
  			cout << "true "<< endl;
  		}
  		else{
  			cout << "false" << endl ;
  		}

  		i++;
  	}

  	cout << endl << endl << "nombre de partitions : " << partitions.size() << endl ;
	*/

  	int tot = 0 ;

  	for (int i = 0; i < taille; ++i)
  	{
  		_Cilk_for (int j = 0; j < mahonian(n, i); ++j)
  		{
  			cout << " maho " << mahonian(i,j) << endl ;
  			Partition test;
  			test.suite = unrank(i, j);
  			for (int k = 0; k < n-1; ++k)
  			{
  				test.barres[k]=true;
  			}

  			cout << " nombre de fonctions liées à la permutation " ;
  			for (int k = 0; k < n ; ++k)
  			{
  				cout << test.suite[k] << " ";
  			}
  			int taille_ensemble = mots_generateurs(test).size();
  			cout << " : " << taille_ensemble << endl << endl ;
  			tot+=taille_ensemble;
  		}
  	}
  	cout << " total fonctions : " << tot << endl ;
	return 0;
}