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

constexpr int taille_gen = (int)pow(2.0,(double)(n-1));

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

void affiche_tab(array<int,n> v){
 	for (int i : v )
 	{
 		cout << i << " " ;
 	}
 
 	cout << endl ;
 }

void affiche_b(array<bool,n-1> tab){
	for (bool b : tab)
 	{
		cout << b << " ";
 	}

	cout << endl;
}

void affiche_partition(Partition p){
	cout << " suite " << endl ;
	affiche_tab(p.suite);
	cout << " barres " << endl ;
	affiche_b(p.barres);
 }
/*

	algo en pseudo code ML avec des elements de c++ ( boucles for à la c++ ) =
	
	res = ref []
	
	
	enum ((_, corps) as gen) possibles ((dernier,_)as acc)=
	
	match gen with
	| 1 :: [] -> for ( int i : possibles )
					if i > derniers
						res := (i::acc)::!res
	
	| 1 :: _ -> for ( int i : possibles )
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

int ajust(array<bool,n> const & possibles, int index){
	for (int i = 0; i < n ; ++i)
	{
		if(index <= 0 && possibles[i]){
			return i;
		}
		if(possibles[i])
		{
			index--;
		}
	}
}

int nbr_partitions=0;

void gen_partitions
(vector<Partition> &partitions, array<int,n> generateur,int gen, array<bool,n> possibles, Partition acc, int fait){
	if(generateur.at(gen) == 1 ){
		if(gen != (n-1) && generateur.at(gen+1) != 0){
			// 1::_
			for (int i = 0 ; i < n-fait ; i++ )
			{
				int indice = ajust(possibles, i);

				if (fait != 0 && !acc.barres[fait-1] && indice < acc.suite[fait-1])
				{
					continue;
				}

				array<bool,n> poss_tmp = possibles;
				Partition acc_tmp = acc;
				acc_tmp.suite[fait] = indice+1;
				acc_tmp.barres[fait] = true;
				poss_tmp[indice]=false;
				gen_partitions(partitions, generateur , gen+1 , poss_tmp, acc_tmp, fait+1);
			}
		}
		else { 	
			// 1::[]
			for (int i = 0 ; i < n-fait ; i++)
			{
				int indice = ajust(possibles, i);
				if (fait != 0 && !acc.barres[fait-1] && indice < acc.suite[fait-1])
				{
					continue;
				}

				Partition acc_tmp = acc;
				acc_tmp.suite[fait] = indice+1;
				partitions.push_back(acc_tmp);
				
			}
		}
	}
	else{ 	
		// a::_
		for (int i = 0; i <= (n-fait)-(generateur[gen]); ++i)
		{
			int indice = ajust(possibles, i);
			if (fait != 0 && !acc.barres[fait-1] && indice < acc.suite[fait-1])
			{
				continue;
			}
			else{

				array<bool,n> poss_tmp = possibles;
				Partition acc_tmp = acc;
				array<int,n> gen_tmp = generateur;
				gen_tmp[gen]--;
				acc_tmp.suite[fait] = indice+1;
				poss_tmp[indice]=false;
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
//int get_partitions(){
	//array<array<int,n>, taille_gen> generateurs = gen_comp_n();
	array<array<int,n>, taille_gen> generateurs = {{3,1,2,0,}};
	array<bool,n> possibles;
	for (int i = 0; i < n; ++i)
	{
		possibles[i]=true;
	}
	vector<Partition> partitions;
	 for (array<int,n> gen : generateurs)
	 {
	 	Partition vide;
	 	gen_partitions(partitions, gen, 0, possibles, vide, 0) ;
	 }
	return partitions;
	//return nbr_partitions;
}


/* faire un tableau dont l'index est le mot booleen et 
l'element de la case est le multimoniaux ( n , forme issue du mot booleen)

exemple pr n = 4

le mot booleen 1,0,1 = 5 donc dans la 5eme case il y aura

4! / ( 1! 2! 1!)

ca va servir pour faire le unrank
*/

array<int,taille_gen> sav_multimoniaux;

int multimoniaux(int fm, array<int,n> k){
	int div = 1 ;

	for(int el : k){
		div*=fact(el);
	}

	return fm/div;
}

void init_multimoniaux(){
	array<bitset<n>,taille_gen> mots_binaires = generation();

	for ( int i = 0 ; i < taille_gen ; i++)
	{
		array<int,n> mot = bijection(mots_binaires[i]);

		sav_multimoniaux[i] = multimoniaux(fact(n),mot);
	}
}

array<int,n> repartition_to_binaire(array<bool,n-1> barres){

	array<int,n> res;
	for (int i = 0; i < n; ++i)
	{
		res[i]=0;
	}
	int nbr_poche=0;
	int poche=0;

	for (int i = 0; i < n-1; ++i)
	{
		if (barres[i])
		{
			res[poche]=nbr_poche+1;
			poche++;
			nbr_poche=0;
		}
		else{
			nbr_poche++;
		}
	}

	res[poche]=nbr_poche+1;

	return res;
}


// un nombre est considéré comme possible quand il est plus grand que les éléments qui sont deja dans la part 
// et qui est plus petit que l'élément actuel
int nbr_possibles(int actuel, array<bool,n> const & possibles, Partition const & p, int indice){
	int min=0;

	try {
		if(!p.barres.at(n-indice-1)){
			min = p.suite.at(n-indice-1);
		}
	}
	catch(exception e){}

	int res=0;
	for (int i = 0; i < n; ++i)
	{
		if(possibles[i] && (i+1) > min && (i+1) < actuel){
			cout << i+1 << "  est possible " << endl ;
			res++;
		}
	}
	return res;
}

bool fin_part(array<int,n> const & forme, int reste){
	for (int i = n-1 ; i >= 0 ; i--)
	{
		reste-=forme[i];
		if (reste == 0 && forme[i] == 1){
			if (forme[i-1] && i-1 >= 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;// theoriquement on ne fait jamais ce return
}

void enleve(array<int,n> & forme){
	for (int i = 0; i < n; ++i)
 		{
 			if(forme[i]){
 				forme[i]--;
 				break;
 			}
 		}
}

int possibles_superieurs(array<bool,n> const & possibles, int val, Partition const & p, int endroit){
	int fin_part=false;
	int res=0;
	int espace_fin=0;

	int i=endroit;
	while (!fin_part)
	{
		if (p.barres[i])
		{
			fin_part=true;
		}
		else{
			espace_fin++;
		}
		i++;
		if (i==n)
		{
			break;
		}
	}

	for (int i = endroit; i <= n-espace_fin; ++i)
	{
		if (possibles[i] && i+1 > val)
		{
			res++;
		}
	}
	return res;
}

array<int,n> les_possibles(int actuel, array<bool,n> const & possibles, Partition const & p, int indice){
	array<int,n> res;
	for (int i = 0; i < n; ++i)
	{
		if (possibles[i] && i+1 < actuel)
		{
			res[i]=i+1;
		}
		else{
			res[i]=0;
		}
	}
	return res;
}

int ranka(Partition const & p){
 	int niv = n;
 	int min = 0;
 	array<bool,n> possibles;
 	for (int i = 0; i < n; ++i)
 	{
 		possibles[i]=true;
 	}
 	array<int,n> forme = repartition_to_binaire(p.barres);

 	for(int j : p.suite ){
 		
 		if(fin_part(forme, niv)){
 			enleve(forme);
 			int multi = multimoniaux(fact(niv-1),forme);
 			min+=nbr_possibles(j, possibles, p, niv)*multi;
 		}
 		else{
	 		enleve(forme);

		 	for (int i : les_possibles(j, possibles, p, niv))
		 	{
		 		if (i != 0)
		 		{
			 		int sup=possibles_superieurs(possibles, i, p, n-niv);
			 		int num = fact(niv-2)*sup;
			 		int multi = multimoniaux(num, forme);
			 		min+=multi;
			 		cout << "pour i : " << i << "  num " << num << "niv : " << niv << " sup : " << sup << endl ;
		 		}
		 	}
 		}
 		cout << "forme" << endl;
 		for (int i : forme)
 		{
 			cout << i << " "  ;
 		}
 		cout << endl ;
 		cout << " j " << j << " min " << min << endl ;
 		possibles[j-1]=false;
 		niv--;

 	}

 	return min;
}