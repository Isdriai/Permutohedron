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

typedef list<int> Mot;


struct hash_vect{
  	std::size_t operator()(std::vector<int> const& vec) const {
  		std::size_t seed = vec.size();
  		for(auto& i : vec) {
   		 	seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		}
  		return seed;
	}
};

struct hash_mot
{
	std::size_t operator()(std::list<int> const& mot) const {
  		std::size_t seed = mot.size();
  		for(auto& i : mot) {
   		 	seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		}
  		return seed;
	}
};

void affiche_mot(Mot const &f){
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

bool present(Mot const &xi, unordered_set<vector<int>, hash_vect> &sav_beg_trouve){
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

unordered_set<Mot, hash_mot> get_mots(){
	init_Beg();
	Mot id={0};
	stack<Mot> afaire;
	afaire.push(id);
	unordered_set<Mot, hash_mot> trouve={id};
	unordered_set<vector<int>, hash_vect> sav_beg_trouve;
	while(!afaire.empty()){
		Mot x=afaire.top();
		afaire.pop();
		for (int i = 1; i < n; ++i)
		{
			Mot xi = x;
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

bool beg=false;

Partition inverse(Partition const &p, int i){
	// ATTENTION ! la partition entrée p doit etre une permutation ! On ne peut pas appliquer l'inverse de π_i aussi facilement sinon
	Partition res = p;
	int tmp = res.suite[i];
	res.suite[i]=res.suite[i+1];
	res.suite[i+1]=tmp;
	return res;
}

unordered_set<Mot, hash_mot> mots_generateurs(Partition const &p){
	cout << " partition representant la permut : " ;
	for (int i = 0; i < n; ++i)
	{
		cout << p.suite[i] << " ";
	}
	cout << endl ;
	if (!beg)
	{
		init_Beg();
		beg=true;
		cout << " beg init " << endl;
	}
	bool descente = false;
	array<bool,n-1> pi_pre;
	for (int i = 0; i < n-1; ++i)
	{
		if (p.suite[i] > p.suite[i+1])
		{
			pi_pre[i]=true;
			descente=true;
		}
		else{
			pi_pre[i]=false;
		}
	}

	unordered_set<Mot, hash_mot> trouve;

	if(!descente){
		Mot id;
		id.push_back(0);
		trouve.insert(id);
	}
	else{
		unordered_set<vector<int>, hash_vect> sav_beg_trouve;
		stack<Mot> afaire;
		for (int i = 0; i < n-1; ++i)
		{
			if (pi_pre[i])
			{
				for (auto f : mots_generateurs(inverse(p, i)))
				{
					Mot xi=f;
					xi.push_back(i);
					if (!present(xi, sav_beg_trouve))
					{
						cout << " ajout a trouve " << endl;
						trouve.insert(xi);
						afaire.push(xi);
					}
				}
			}
		}
		while(!afaire.empty()){
			Mot x=afaire.top();
			afaire.pop();
			for (int i = 1; i < n; ++i)
			{
				Mot xi = x;
				xi.push_back(i);
				if (!present(xi,sav_beg_trouve)) 
				{
					cout << " ajout a trouve " << endl;
					trouve.insert(xi);
					afaire.push(xi);
				}
			}
		}
	}
	return trouve;
}