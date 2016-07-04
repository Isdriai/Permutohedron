#include <iostream>
#include <stdlib.h>
#include "chaine.hpp"
#include "partitions.hpp"

using namespace std;

int main(int argc, char const *argv[]){

	//array<int, n> test {3,2,1};

	init_maho();

	//cout << maho_suiv(4,8) << endl ;

	//affiche_maho();

	cout << chaines_max_bis() << endl ;

	return 0;
}