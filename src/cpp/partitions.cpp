#include <bitset>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "basique.hpp"
#include "const.hpp"

using namespace std;

struct liste
{
	int elem;
	struct liste *corps;
};

union partie
{
    struct liste elements;
    bool mur; // si partie est un bool, il sera toujorus a true
};

typedef vector<partie> partition;

// genere les mots binaires allant de 0 à 2^(n-1)
vector<bitset<gen>> generation(){

	vector<bitset<gen>> solutions;

	for (int i = 0; i < (2 << gen-1 ) ; ++i)
	{
		solutions.push_back(bitset<gen>(i));
	}

	return solutions;
}

// donne pour un mot binaire donné la distribution des elements dans la partition
vector<int> bijection (bitset<gen> const & bij){
	vector<int> tmp;
	int cpt = 0 ;

	for (int i = 0; i < gen ; i++)
	{
		if(bij[i]){
			tmp.push_back(cpt+1);
			cpt = 0 ;
		}
		else {
			cpt++;
		}
	}
	tmp.push_back(cpt+1);

	return tmp;
}

// genere toutes les repartitions possibles des elements
vector<vector<int>> gen_comp_n (){

	vector<vector<int>> res ;

	for (bitset<gen> & bij : generation())
	{
		res.push_back(bijection(bij));
	}

	return res;
}

vector<partition> partitions;

vector<partition> gen_partitions(vector<int> generateur, vector<int> possibles, vector<int> acc){
	if
}

vector<int> init_possibles(){
	std::vector<int> tmp;

	for (int i = 1 ; i <= n; ++i)
	{
		tmp.push_back(i);
	}

	return tmp;
}

//donne toutes les partitions possibles pour n 
vector<partition> get_partitions(){
	vector<vector<int>> generateurs = gen_comp_n();

	vector<int> possibles= init_possibles();

	for (vector<int> gen : generateurs)
	{
		gen_partitions(gen, possibles, vector<int> ()) ;
	}

	return partitions;
}