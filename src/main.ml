let n = 4


let rec affiche l = 
	match l with
	| a::b -> (* Printf.printf "%d" a ; *) affiche b
	| [] -> Printf.printf "\n"

let fact n = 
	let rec factorielle x acc =
		match x with
		| 0 -> acc
		| a -> factorielle (x-1) (acc*a)
	in
	factorielle n 1

let count = fact n

(*
	Mahonian fonction

	M(0, 0) = 1
	M(0, c) = 0 for c <> 0
	M(r, c) = Σ(k=0 -> r) M(r - 1, c-k )

*)

let rec mahonian r c =
	match r, c with
	| 0, 0 -> 1
	| 0, col when c <> 0 -> 0
	| lig, col -> let res = ref 0 in 

	(for i = 0 to lig do 
		res := !res + mahonian (lig - 1 ) ( col - i )
	done);

	!res
	

let matrice = Hashtbl.create 1

let rec construction r niv = 

		if r = 0 then  begin Hashtbl.add matrice (r,niv) (1,1::[]) end
		else if r >= niv && niv > 0 then begin Hashtbl.add matrice (r,niv) (0,[]) end 
		else
		let rec col acc c tot=
			let ligne = r-c in 

			if ligne >= 0 then begin (* on remonte la colonne precedente .....*)
				try
					let (valeur,_) = Hashtbl.find matrice (ligne, niv-1) in 

					if tot = mahonian niv r then (* ... mais on s'arrete quand on a atteind le nombre de mahonian c'est à dire min(n ,k+1)*)
						Hashtbl.add matrice (r,niv) (tot,List.rev_append acc []) 
					else 
						col (valeur::acc) (c+1) (tot+valeur)

				with Not_found -> construction ligne (niv-1) ; col acc c tot
			end
			else
				Hashtbl.add matrice (r,niv) (tot,List.rev_append acc [])

		in 

		col [] 0 0


let parcourt dec i =


	let rec parc d reste c = 
		match d with
		| a::b -> if a > reste then (reste, c) else parc b (reste-a) (c+1)
		| [] -> (0,0) (*n'arrive jamais*)
	in

	parc dec i 0



let lehmer rang index = 


	let rec rec_lehmer r i nbr acc=

		if nbr = 1 && r = 0 then (0::acc) (*on aurait pu ecrire (Hashtbl.find matrice (r,nbr) )::acc*)
		else
			try 
				let (_,decoupage) = Hashtbl.find matrice (r, nbr) in 
				let (new_index, res) = parcourt decoupage i in 
				rec_lehmer (r-res) new_index (nbr-1) (res::acc)

			with Not_found -> construction r nbr ; rec_lehmer r i nbr acc

	in 

	rec_lehmer rang index n []

let permut_to_lehmer leh =

	let possibles = (*liste les nombres allant de 1 à n*)
		let rec construct i acc = 
			if i < n then 
				construct (i+1) (i::acc)
			else
				(i::acc)
		in
		construct 1 []
	in 

	let rec rang_nbr nbr liste= (*dit le rang d'un nombre dans une liste, ex = le rang de 5 dans 1235 est 4 *)
		
		let rec rg_nbr l acc=
			match l with
			| a::b -> if a > nbr then rg_nbr b acc else rg_nbr b (acc+1)
			| [] -> acc
		in 

		rg_nbr liste 0
	in

	let rec search rang liste =
		match liste with
		| p::a::b -> if rang = rang_nbr a liste then (a, (p::b)) else search rang b
		| a::b -> if rang = rang_nbr a liste then (a, b) else search rang b
		| [] -> (0,[]) (*ce cas ne doit jamais arriver !*)
	in

	let rec trans lehm reste acc = 
		match lehm with
		| a::b -> let (rang, res ) = search a reste in trans b res (rang::acc)
		| [] -> acc
	in

	trans leh possibles []


let rec affiche l = 
	match l with
	| a::b ->  Printf.printf "%d " a ; affiche b
	| [] -> Printf.printf "[]\n"

let debug () = 
	Hashtbl.iter (fun (x,y) (tot, liste) -> Printf.printf "x %d y %d tot %d liste " x y tot ; 
											affiche liste ;
											Printf.printf "\n" ) matrice 

let unrank rang index = 

(* 	let (i, num ) = parcourt (0::1::2::2::1::[]) 2 in
	Printf.printf "i %d num %d \n" i num  *)


	let leh = lehmer rang index in 
	debug ();
	affiche leh
	(*
	let permut = permut_to_lehmer leh in (*la permutation est sous forme de liste *)
	affiche permut *)

let () =
	Hashtbl.add matrice (0,1) (1,(1::[]));
	unrank 4 0