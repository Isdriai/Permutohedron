let n = 3 

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
	

let lehmer index rang = 

	let limite = index*rang in (*Pour eviter de travailler avec des flottants*)

	let rec lehm col reste acc =

		let maho = mahonian rang (col-1) in
		let mult = reste/maho in
		let r = reste - (mult*maho) in 

		if r > 0 then 
			lehm (col-1) r (mult::acc)
		else
			acc

	in

	lehm n limite []

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
	| a::b -> Printf.printf "%d" a ; affiche b
	| [] -> Printf.printf "\n"


let unrank rang index = 
	let leh = lehmer index rang in 
	let permut = permut_to_lehmer leh in (*la permutation est sous forme de liste *)
	affiche permut

let () =
	unrank 2 2