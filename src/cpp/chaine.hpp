#include <iostream> 

using namespace std;


typedef __uint128_t longuint;

std::ostream& operator<<( std::ostream& dest, longuint value );

longuint chaines_max_bis();

longuint chaines_max_ter();

int chaines_max();	


void test_init_hash();
void test_init_array();