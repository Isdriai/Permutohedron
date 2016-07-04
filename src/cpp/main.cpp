#include <iostream>
#include <stdlib.h>
#include "chaine.hpp"
#include "partitions.hpp"

using namespace std;

int main(int argc, char const *argv[]){

	init_maho();

	cout << chaines_max_bis() << endl ;

	return 0;
}