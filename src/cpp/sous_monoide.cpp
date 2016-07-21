#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <array>
#include <unordered_set>
#include "partitions.hpp"
#include <list>
#include <stack>
#include <unordered_map>


int stirling(int n, int k){
	if(k>n){
		return 0;
	}
	else if (k==n || k==1) {
		return 1;
	}
	else{
		return stirling(n-1, k-1)+k*stirling(n-1,k);
	}
}

int somme_stirling(int n){
	int res=0;
	for (int i = 1; i <= n; ++i)
	{
		res+=fact(i)*stirling(n, i);	
	}
	return res;
}

const int nbr_partitions = somme_stirling(n);

array<vector<int>,n-1> operation_Fi;
array<vector<int>,n-1> operation_Si;
array<vector<int>,n> operation_Beg;


array<vector<int>,n-1> get_operation_Fi(){
	return operation_Fi;
}
array<vector<int>,n-1> get_operation_Si(){
	return operation_Si;
}
array<vector<int>,n> get_operation_Beg(){
	return operation_Beg;
}

void affiche_operation(array<vector<int>,n-1> const & operation){
	for (int i = 0; i < n-1; ++i)
	{
		for (int j = 0; j < nbr_partitions; j++)
		{
			affiche_partition(unrank(j)) ;
			cout << "  ->   ";
			affiche_partition(unrank(operation[i][j]));
			cout << endl << endl;
		}
		cout << endl << endl << endl;
	}
}

void init_Fi(){
	for (int i = 0; i < n-1; ++i)
	{
		operation_Fi[i].resize(nbr_partitions);
		for (int j = 0; j < nbr_partitions; ++j)
		{
			operation_Fi[i][j]=ranka(fusionner(unrank(j), i));
		}
	}
}

void init_Si(){
	for (int i = 0; i < n-1; ++i)
	{
		operation_Si[i].resize(nbr_partitions);
		for (int j = 0; j < nbr_partitions; ++j)
		{
			operation_Si[i][j]=ranka(separer(unrank(j),i));
		}
	}
}

void init_Beg(){
	init_Si();
	init_Fi();
	operation_Beg[0].resize(nbr_partitions);
	// fonction identité
	for (int i = 0; i < nbr_partitions ; ++i)
	{
		operation_Beg[0][i]=i;
	}
	// reste des fonctions
	for (int i = 1; i < n; ++i)
	{	
		operation_Beg[i].resize(nbr_partitions);
		for (int j = 0; j < nbr_partitions; j++)
		{
			// du coup beg est decalé de 1 par rapport a fusion et separe car sa premiere ligne est la fonction ID
			int saut = operation_Fi[i-1][j];
			operation_Beg[i][j]=operation_Si[i-1][saut];
		}
	}
}

typedef list<int> Fonction;


struct hash_vect{
  	std::size_t operator()(std::vector<int> const& vec) const {
  		std::size_t seed = vec.size();
  		for(auto& i : vec) {
   		 	seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		}
  		return seed;
	}
};

struct hash_fonction
{
	std::size_t operator()(std::list<int> const& fonction) const {
  		std::size_t seed = fonction.size();
  		for(auto& i : fonction) {
   		 	seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		}
  		return seed;
	}
};

void affiche_fonction(Fonction const &f){
	for (int i : f)
	{
		cout << i << " " ;
	}
}

void affiche_vect(vector<int> const &v){
	for (int i : v)
	{
		cout << i << " ";
	}
	cout << endl;
}

bool present(Fonction const &xi, unordered_set<vector<int>, hash_vect> &sav_beg_trouve){
	vector<int> beg_xi;
	beg_xi.resize(nbr_partitions);
	for (int i = 0; i < nbr_partitions; ++i)
	{
		int partition=i;
		for (int pi : xi)
		{
			partition=operation_Beg[pi][partition];
		}
		beg_xi[i]=partition;
	}

	if (sav_beg_trouve.find(beg_xi) != sav_beg_trouve.end())
	{
		return true;
	}
	else{
		sav_beg_trouve.insert(beg_xi);
		return false;
	}
}

unordered_set<Fonction, hash_fonction> get_fonctions(){
	init_Beg();
	Fonction id={0};
	stack<Fonction> afaire;
	afaire.push(id);
	unordered_set<Fonction, hash_fonction> trouve={id};
	unordered_set<vector<int>, hash_vect> sav_beg_trouve;
	while(!afaire.empty()){
		Fonction x=afaire.top();
		afaire.pop();
		for (int i = 1; i < n; ++i)
		{
			Fonction xi = x;
			xi.push_back(i);
			if (!present(xi,sav_beg_trouve)) // regarde si le beg avec xi existe, si non, le rajoute
			{
				trouve.insert(xi);
				afaire.push(xi);
			}
		}
	}
	return trouve;
}

