#include <bitset>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "basique.hpp"
#include "const.hpp"

using namespace std;

typedef vector<vector<int>> partition;


void affiche_vect(vector<int> v){
	for (int i : v )
	{
		cout << i << " " ;
	}

	cout << endl ;
}

void affiche_vect_vect(vector<vector<int>> v){
	for (vector<int> elem : v)
	{
		affiche_vect(elem);
	}
}

// genere les mots binaires allant de 0 à 2^(n-1)
vector<bitset<n>> generation(){

	vector<bitset<n>> solutions;

	for (int i = 0; i < (2 << n-2 ) ; ++i)
	{
		solutions.push_back(bitset<n>(i*2+1));
	}

	return solutions;
}


// donne pour un mot binaire donné la distribution des elements dans la partition
vector<int> bijection (bitset<n> const & bij){
	vector<int> tmp;

	int cpt = 0 ;


	for (int i = n-1; i >= 0 ; i--)
	{ 

		if(bij[i]){
			tmp.push_back(cpt+1);
			cpt = 0 ;
		}
		else {
			cpt++;
		}
	}


	return tmp;
}

// genere toutes les repartitions possibles des elements
vector<vector<int>> gen_comp_n (){

	vector<vector<int>> res ;

	for (bitset<n> & bij : generation())
	{
		res.push_back(bijection(bij));
	}

	return res;
}

int maximum(vector<int> v){
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
(vector<partition> &partitions, vector<int> generateur, vector<int> possibles, vector<vector<int>> acc, vector<int> part_act){

	if(generateur.at(0) == 1 ){
		try 
		{		
			generateur.at(1);
			// 1::_
			for (int i = 0 ; i < possibles.size() ; i++ )
			{
				try {
					if (possibles[i] < part_act.at(part_act.size()-1))
					{
						continue;
					}
				}
				catch(exception e){}

				vector<int> poss_tmp = possibles;
				vector<int> gen_tmp = generateur;
				vector<vector<int>> acc_tmp = acc;
				vector<int> part_act_tmp = part_act;

				part_act_tmp.push_back(possibles[i]);
				poss_tmp.erase(poss_tmp.begin() + i);
				acc_tmp.push_back(part_act_tmp);
				gen_tmp.erase(gen_tmp.begin());
				vector<int> new_part;
				gen_partitions(partitions, gen_tmp , poss_tmp, acc_tmp, new_part);
			}
		}
		catch(exception e) { 	

			// 1::[]
			for (int i : possibles)
			{
				if (!part_act.empty() && i < part_act.at(part_act.size()-1))
				{
					continue;
				}
				else{
					vector<vector<int>> acc_tmp = acc;
					vector<int> part_act_tmp = part_act;

					part_act_tmp.push_back(i);
					acc_tmp.push_back(part_act_tmp);
					partitions.push_back(acc_tmp);
				}
			}
		}
	}
	else{ 	
		// a::_
		for (int i = 0; i <= possibles.size()-(generateur[0]); ++i)
		{

			if (!part_act.empty() && possibles[i] < part_act.at(part_act.size()-1))
			{
				continue;
			}
			else{


				vector<int> poss_tmp = possibles;
				vector<int> gen_tmp = generateur;
				vector<vector<int>> acc_tmp = acc;
				vector<int> part_act_tmp = part_act;

				gen_tmp[0]--;
				part_act_tmp.push_back(possibles[i]);
				poss_tmp.erase(poss_tmp.begin() + i);
				gen_partitions(partitions, gen_tmp, poss_tmp, acc, part_act_tmp);
			}
		}
	}
}

vector<int> init_possibles(){
	std::vector<int> tmp;

	for (int i = 1 ; i <= n; ++i)
	{
		tmp.push_back(i);
	}

	return tmp;
}

void affiche_part(vector<partition> part){
	for (partition p : part)
	{
		affiche_vect_vect(p);
		cout << endl ;
	}
	cout << endl ;
}

//donne toutes les partitions possibles pour n 
vector<partition> get_partitions(){


	//cout << bitset<n>(51) << endl; 
	//affiche_vect(bijection(bitset<n>(51)));

	vector<vector<int>> generateurs = gen_comp_n();


	vector<int> possibles= init_possibles();
	vector<partition> partitions;

	//cout << bitset<n>(3) << endl ;
	//std::vector<int> test = bijection(bitset<n>(3));
	
	 for (vector<int> gen : generateurs)
	 {
	 	gen_partitions(partitions, gen, possibles, vector<vector<int>> (), vector<int> ()) ;
	 }
	 	//gen_partitions(partitions, test, possibles, vector<vector<int>> (), vector<int> ());

	 affiche_part(partitions);
	 cout << endl << endl << endl << endl ;

	return partitions;
}