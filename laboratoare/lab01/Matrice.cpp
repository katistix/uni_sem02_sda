#include "Matrice.h"

#include <exception>
#include <stdlib.h>
#include <stdexcept>

#define INITIAL_CAPACITY 10

using namespace std;

Matrice::Matrice(int n, int m) {
	// Theta(1)
	this->nr_linii = n;
	this->nr_coloane = m;
	
	this->size = 0;
	this->capacity = INITIAL_CAPACITY;
	
	// Alocare memorie in Triplet* elemente.
	this->elemente = new Triplet[this->capacity];
}



int Matrice::nrLinii() const{
	// Theta(1)
	return this->nr_linii;
}


int Matrice::nrColoane() const{
	// Theta(1)
	return this->nr_coloane;
}


TElem Matrice::element(int i, int j) const{
	// daca e o pozitie invalida, aruncam o exceptie
	if(i<0 || j<0 || i>=this->nr_linii || j>=this->nr_coloane){
		throw std::out_of_range("Invalid position!");
	}

	int pozitie = binary_search(i,j);

	if(pozitie==-1) {
		return 0; // nu se alfla in sir, deci este 0 in matrice
	}

	return this->elemente[pozitie].value;
}



TElem Matrice::modifica(int i, int j, TElem e) {
	int pozitie = binary_search(i, j);
	
	// daca un element nu exista, adaugam un triplet in pozitia corecta
	if (pozitie == -1) {
		Triplet triplet_nou;
		triplet_nou.i = i;
		triplet_nou.j = j;
		triplet_nou.value = e;

		// TODO: allocate more space if no more space is available
		// TODO: put the triplet_nou into the new position
	}

	// TODO: daca setam un element la 0, il eliminam din lista de triplete

	return -1;
}



bool Matrice::relatie(Triplet a, Triplet b) const {
	// compare based on `i`
	if (a.i < b.i) return true;
	else if (a.i > b.i) return false;
	else {
		// compare based on `j`
		if(a.j<=b.j) return true;
		else if (a.j>b.j) return false;
	}	
}

int Matrice::binary_search(int i, int j) const {
	// BC = Theta(1)
	// WC = Theta(log(n))
	// AC = Theta(log(n))
	// General = O(log(n))
	int start = 0, end = this->size;

	Triplet target;
	target.i=i;
	target.j=j;
	target.value=0;


	while (start < end) {
		int mid = (start + end) / 2;
		if (this->elemente[mid].i == target.i && this->elemente[mid].j == target.j) {
			return mid;
		}		
		if (this->relatie(
			target,
			this->elemente[mid]
		)) {
			end = mid;
		}
		else {
			start = mid + 1;
		}
	}

	return -1;
}


// TODO: implementeaza destructor