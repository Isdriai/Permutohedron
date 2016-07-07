#include <iostream>
#include <stdlib.h>
#include "chaine.hpp"
#include "partitions.hpp"

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

int main(int argc, char const *argv[]){

	init_maho();

	 //vector<Partition> partitions = get_partitions();

	 //cout << partitions.size() << endl ;

		//test_init_hash();
	  //test_init_array();
		cout << chaines_max_bis() << endl ;
	

	return 0;
}