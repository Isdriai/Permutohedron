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
  array<bool,n-1> barres {{false}}; // a modifier en bitset
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
// il faudra faire quelque chose, il y a 3 fois la meme fonction
// il faut changer l'attribut barres en bitset dans la structure partition
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

array<int,n> repartition_to_binaire_bitset(bitset<n-1> barres){

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

template<size_t N>
void affiche_b(array<bool,N> tab){
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

 Partition separer(Partition const & p){

 }

 Partition fusioner(Partition const & p){
 	
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
	array<array<int,n>, taille_gen> generateurs = gen_comp_n();
	//array<array<int,n>, taille_gen> generateurs = {{5,2,0,0,0,0,0}};
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

array<int,taille_gen> sav_multimoniaux;

int multimoniaux(int m, array<int,n> k){
	int div = 1 ;

	for(int el : k){
		div*=fact(el);
	}

	return fact(m)/div;
}

void init_multimoniaux(){
	array<bitset<n>,taille_gen> mots_binaires = generation();

	for ( int i = 0 ; i < taille_gen ; i++)
	{
		array<int,n> mot = bijection(mots_binaires[i]);

		sav_multimoniaux[i] = multimoniaux(n,mot);
	}
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

array<int,n> les_possibles(int actuel, array<bool,n> const & possibles, Partition const & p, int indice){
	int min=0;

	try {
		if(!p.barres.at(n-indice-1)){
			min = p.suite.at(n-indice-1);
		}
	}
	catch(exception e){}

	array<int,n> res;
	for (int i = 0; i < n; ++i)
	{
		if(possibles[i] && (i+1) > min && (i+1) < actuel){
			res[i]=i+1;
		}
		else{
			res[i]=0;
		}
	}
	return res;
}

int binomial(int n, int k){
	int res = fact(n)/(fact(k)*fact(n-k));
	if (res == 0)
	{
		return 1;
	}
	return res;
}

int plus_grand(array<bool,n> const & possibles, int actuel){
	int res=0;
	for (int i = 0; i < n; ++i)
	{
		if(possibles[i] && i+1 > actuel){
			res++;
		}
	}
	return res;
}

int puissance(int x, int p){
	int res=1;
	for (int i = 0; i < p ; ++i)
	{
		res*=x;
	}
	return res;
}

int rank_tas(array<bool,n-1> const & barres){
	int res=0;
	for (int i = 0; i < n-1; ++i)
	{
		res+=puissance(2, n-2-i)*barres[i];
	}
	return res;
}

array<int,n> tronc (array<int,n> const & forme, int endroit){
	auto res = forme;
	res[endroit]=0;
	return res;
}

/* pr chaque element j, on regarde les éléments encore possibles qui sont plus petits,
 pr chacun d'eux (i), on compte le nombre d'éléments plus grands qu'eux encore possibles
 et on regarde cb de place reste t-il dans la part actuel et on fait 
 le coef binomial * coef multimonial des autres parts
 ex : 1 2 3 6 7 | 4 5
 quand on arrive au 6, les plus petits possibles sont 4 et 5
 pr 4 par exemple, ca ferait binomial(3, 1) * (2!/2!) car il reste une place dans la part et 
 qu'il y a 5,6,7 qui sont plus grands que lui
 pr ce qui est de la deuxieme partie c'est le coef multimonial du reste
*/
int ranka(Partition const & p){
 	int niv = n;
 	int min = 0;
 	int num_tas = rank_tas(p.barres);
 	for (int i = 0; i < num_tas; ++i)
 	{
 		array<int,n> forme_tmp = repartition_to_binaire_bitset(bitset<n-1> (i));
 		min+=multimoniaux(n, forme_tmp);
 	}
 	array<bool,n> possibles;
 	for (int i = 0; i < n; ++i)
 	{
 		possibles[i]=true;
 	}
 	array<int,n> forme = repartition_to_binaire(p.barres);
 	int num_forme=0;

 	for(int j : p.suite ){
 		if (forme[num_forme]==0)
 		{
 			num_forme++;
 		}
 		enleve(forme);
 		for (int i : les_possibles(j, possibles, p, niv)){
 			if ( i != 0)
 			{
	 			int nbr_forme=forme[num_forme];
		 		int binome = binomial(plus_grand(possibles, i), nbr_forme);
		 		array<int,n> post_forme=forme;
		 		post_forme[num_forme]--;
		 		int div = multimoniaux(niv-1-nbr_forme, tronc(post_forme, num_forme));
		 		min+=binome*div;
 			}		
 		}
 		possibles[j-1]=false;
 		niv--;
 	}
 	return min;
}

array<bool,n-1> transfo(bitset<n-1> const & barres){
	array<bool,n-1> res;
	for (int i = 0; i < n-1; ++i)
	{
		res[i]=barres[n-2-i];
	}
	return res;
}

void urk(Partition &res, array<int,n> forme, array<int,n> post_forme, array<bool,n> & possibles, int rang, int num_forme, int min, int niv){
	/*cout << "forme " << endl ;
	affiche_tab(forme);
	cout << " post_forme " << endl;
	affiche_tab(post_forme);
	cout << " possibles " << endl;
	affiche_b(possibles);
	cout << " rang " << rang << endl ;
	cout << " num_forme " << num_forme << endl ;
	cout << " min " << min << endl;
	cout << "niv " << niv << endl ;
	cout << endl << endl << endl;
*/

	for (int i = min; i < n; ++i)
	{
		if (possibles[i])
		{
			int nbr=binomial(plus_grand(possibles, i+1), forme[num_forme])*multimoniaux(niv-1-forme[num_forme], post_forme);
			//cout << " pr i : " << i+1 << "  nbr : " << nbr << endl ;
			if (rang>=nbr && nbr!=0)
			{
				rang-=nbr;
			}
			else{
				res.suite[n-niv]=i+1;
				//cout << " i choisi " << i+1 << endl;
				possibles[i]=false;
				min=i+1;
				if(forme[num_forme]==0){
					min=0;
					num_forme++;
					post_forme=tronc(forme, num_forme);
				}
				forme[num_forme]--;
				urk(res, forme, post_forme, possibles, rang, num_forme, min, niv-1);
				break;
			}
		}
	}
}

// il me faut quelque chose de plus fin que les nbrs de stirling
Partition unrank(int rang){

	// génération de la forme

	Partition res;
	int multi=0;
	int i=0;
	array<int,n> forme;
	bitset<n-1> barres_tmp;
	while(multi <= rang){
		rang-=multi;
		barres_tmp=bitset<n-1> (i);
		forme=repartition_to_binaire(transfo(barres_tmp));
		multi=multimoniaux(n, forme);
		i++;
	}
	res.barres=transfo(barres_tmp);

	// génération de la suite

	array<bool,n> possibles;
	for (int i = 0; i < n; ++i)
	{
		possibles[i]=i+1;
	}
 	//cout << " rang " << rang << endl ;
	int min=0;
	int num_forme=0;
	array<int,n>post_forme=tronc(forme, num_forme);
	forme[num_forme]--;

	/*cout << "forme " << endl ;
	affiche_tab(forme);
	cout << " post_forme " << endl;
	affiche_tab(post_forme);
	cout << " possibles " << endl;
	affiche_b(possibles);*/

	urk(res, forme, post_forme, possibles, rang, 0, 0, n);

	return res;
}