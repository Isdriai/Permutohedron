exe: basique.o chaine.o partitions.o main.o
	gcc -o /home/rodolphe/Documents/lri/Permutohedron/bin/exe -c basique.o chaine.o partitions.o main.o

basique.o: basique.cpp const.hpp
	gcc -o /home/rodolphe/Documents/lri/Permutohedron/bin/basique.o -c basique.cpp

chaine.o: chaine.cpp const.hpp basique.hpp
	gcc -o /home/rodolphe/Documents/lri/Permutohedron/bin/chaine.o -c chaine.cpp

partitions.o: partitions.cpp basique.hpp const.hpp
	gcc -o /home/rodolphe/Documents/lri/Permutohedron/bin/partitions.cpp

main.o: main.cpp basique.hpp const.hpp chaine.hpp partitions.hpp
	gcc -o /home/rodolphe/Documents/lri/Permutohedron/bin/main.o -c main.cpp
