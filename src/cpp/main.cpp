#include <iostream>
#include <stdlib.h>
#include "chaine.hpp"
#include "partitions.hpp"
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

void affiche_array(array<int,n> const & tab){
	for (int i : tab)
	{
		cout << i << " " ;
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


	mpz_class res = chaines_max_bis();
		 cout << res << endl ;
	

	return 0;
}