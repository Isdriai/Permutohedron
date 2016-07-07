#include <iostream> 
#include <gmpxx.h>

using namespace std;


typedef __uint128_t longuint;

std::ostream& operator<<( std::ostream& dest, longuint value );

mpz_class chaines_max_bis();

longuint chaines_max_ter();

int chaines_max();	


void test_init_hash();
void test_init_array();