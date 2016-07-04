#include <bitset>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "basique.hpp"
#include <math.h>
#include <array>

using namespace std;

struct Partition
{
  array<int,n> suite {{0}};
  array<bool,n-1> barres {{false}};
};

const int taille_gen = (int)pow(2.0,(double)(n-1));

// genere les mots binaires allant de 0 à 2^(n-1)
array<bitset<n>, taille_gen> generation(){

	array<bitset<n>, taille_gen> solutions;
	for (int i = 0; i < (2 << (n-2) ) ; ++i)
	{
		solutions[i]=(bitset<n>(i*2+1));
	}
	return solutions;
}

// donne pour un mot binaire donné la distribution des elements dans la partition
array<int,n> bijection (bitset<n> const & bij){
	
	array<int,n> tmp;
	int cpt = 0 ;
	int poche=0;
	for (int i = n-1; i >= 0 ; i--)
	{ 
		if(bij[i]){
			tmp[poche] = cpt+1;
			poche++;
			cpt = 0 ;
		}
		else {
			cpt++;
		}
	}
	for (int i = poche ; i < n ; ++i)
	{
		tmp[i]=0;
	}
	return tmp;
}

// genere toutes les repartitions possibles des elements
array<array<int,n>, taille_gen> gen_comp_n (){

	array<array<int,n>, taille_gen> res ;
	int cpt = 0 ;
	for (bitset<n> & bij : generation())
	{
		res[cpt]=bijection(bij);
		cpt++;
	}
	return res;
}

template<size_t N>
int maximum(array<int,N> v){
	int tmp=0;
	for (int i : v)
	{
		if(i>tmp){
			tmp=i;
		}
	}
	return tmp;
}

/*

	algo en pseudo code ML avec des elements de c++ ( boucles for à la c++ ) =
	
	res = ref []
	
	
	enum ((_, corps) as gen) possibles ((dernier,_)as acc)=
	
	match gen with
	| 1 :: [] -> for ( int i : possibles )
					if i > derniers
						res := (i::acc)::!res
	
	| 1 :: _ -> for ( int i : possilbes )
					if i > derniers
						enum corps (possibles/i) (Separateur::i::acc)
	
	|a :: _ -> for ( int i = 0 ; i < n-a ; i++  )
					if i > derniers
						enum (a-1::corps) (possibles/i) (i::acc)
	
	
	subtilités = au debut acc est vide, il y aura un pb mais c'est pr donner une idée de l'algo
				il faut aussi bien definir le type de nos partitions, on voit ici "Separateur",
				on pourrait imaginer un type caml comme ceci =
	
	type partition = P of partition * partition | E of int list | Separateur
	
	PS = n est le nbr d'éléments ds la partition
*/

void gen_partitions
(vector<Partition> &partitions, array<int,n> generateur,int gen, vector<int> possibles, Partition acc, int fait){
	if(generateur.at(gen) == 1 ){
		if(gen != (n-1 ) && generateur.at(gen+1) != 0){
			// 1::_
			for (int i = 0 ; i < possibles.size() ; i++ )
			{
				if (fait != 0 && !acc.barres[fait-1] && possibles[i] < acc.suite[fait-1])
				{
					continue;
				}

				vector<int> poss_tmp = possibles;
				Partition acc_tmp = acc;
				acc_tmp.suite[fait] = possibles[i];
				acc_tmp.barres[fait] = true;
				poss_tmp.erase(poss_tmp.begin() + i);
				gen_partitions(partitions, generateur , gen+1 , poss_tmp, acc_tmp, fait+1);
			}
		}
		else { 	
			// 1::[]
			for (int i : possibles)
			{
				if (fait != 0 && !acc.barres[fait-1] && i < acc.suite[fait-1])
				{
					continue;
				}

				Partition acc_tmp = acc;
				acc_tmp.suite[fait] = i;
				partitions.push_back(acc_tmp);
				
			}
		}
	}
	else{ 	
		// a::_
		for (int i = 0; i <= possibles.size()-(generateur[gen]); ++i)
		{
			if (fait != 0 && !acc.barres[fait-1] && possibles[i] < acc.suite[fait-1])
			{
				continue;
			}
			else{

				vector<int> poss_tmp = possibles;
				Partition acc_tmp = acc;
				array<int,n> gen_tmp = generateur;
				gen_tmp[gen]--;
				acc_tmp.suite[fait] = possibles[i];
				poss_tmp.erase(poss_tmp.begin() + i);
				gen_partitions(partitions, gen_tmp, gen, poss_tmp, acc_tmp, fait+1);
			}
		}
	}
}

vector<int> init_possibles(){
	vector<int> tmp(n);
	for (int i = 1 ; i <= n; ++i)
	{
		tmp[i-1]=i;
	}
	return tmp;
}

//donne toutes les partitions possibles pour n 
vector<Partition> get_partitions(){

	array<array<int,n>, taille_gen> generateurs = gen_comp_n();
	vector<int> possibles = init_possibles();
	vector<Partition> partitions;
	 for (array<int,n> gen : generateurs)
	 {
	 	Partition vide;
	 	gen_partitions(partitions, gen, 0, possibles, vide, 0) ;
	 }
	return partitions;
}
