#include <iostream> //cout, endl
#include <cstdlib>  //srand
#include <algorithm>//copy, random_shuffle
#include <iterator> //ostream_iterator
#include <ctime>    //std::time (NULL) to seed srand
#include <cstdio>   //sscanf
#include "mergesort.h"

void test0() {
	const int n = 1000;
	int a[n];
	for ( int i=0; i<n; ++i ) { a[i] = i; }
    std::srand( static_cast<unsigned int>(std::time (NULL)) );
    std::random_shuffle( a, a+n );
    mergesort(a,0,n-1); //first and last index
	
    //check result
    for ( int i=0; i<n; ++i ) {
        if ( a[i] != i ) {
            std::cout << "Error at index " << i << std::endl;
            return;
        }
    }
    std::cout << "OK" << std::endl;
    return;
}

void test1() {
	const int n = 100000;
	int a[n];
	for ( int i=0; i<n; ++i ) { a[i] = i; }
    std::srand( static_cast<unsigned int>(std::time (NULL)) );
    std::random_shuffle( a, a+n );
    mergesort(a,0,n-1); //first and last index
	
    //check result
    for ( int i=0; i<n; ++i ) {
        if ( a[i] != i ) {
            std::cout << "Error at index " << i << std::endl;
            return;
        }
    }
    std::cout << "OK" << std::endl;
    return;
}

void (*pTests[])(void) = { test0,test1 };

int main( int argc, char ** argv) {
	if (argc!=2) return 1;
	else {
		int test = 0;
		std::sscanf(argv[1],"%i",&test);
		pTests[ test ]();
	}
	return 0;
}
