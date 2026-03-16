#include "Matrice.h"

#include <exception>
#include <stdlib.h>
#include <stdexcept>

#define RESIZE_FACTOR 2
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


TElem Matrice::element(int i, int j) const {
    if (i < 0 || j < 0 || i >= this->nr_linii || j >= this->nr_coloane) {
        throw std::out_of_range("Invalid position!");
    }

    int pozitie = cautaPozitie(i, j);

    // Verificam daca la pozitia gasita se afla intr-adevar elementul cautat
    if (pozitie < size && elemente[pozitie].i == i && elemente[pozitie].j == j) {
        return this->elemente[pozitie].value;
    }

    return 0;
}

TElem Matrice::modifica(int i, int j, TElem e) {
    if (i < 0 || j < 0 || i >= this->nr_linii || j >= this->nr_coloane) {
        throw std::out_of_range("Invalid position!");
    }

    int poz = cautaPozitie(i, j);
    TElem valoareVeche = 0;

    // Cazul 1: Elementul exista deja in matrice (i, j)
    if (poz < size && elemente[poz].i == i && elemente[poz].j == j) {
        valoareVeche = elemente[poz].value;

        if (e == 0) {
            // Seteaza pe 0 -> Stergem tripletul si shiftam la stanga
            for (int k = poz; k < size - 1; k++) {
                elemente[k] = elemente[k + 1];
            }
            size--;
        } else {
            // Doar actualizam valoarea
            elemente[poz].value = e;
        }
    } 
    // Cazul 2: Elementul nu exista (e 0 in matrice)
    else {
        valoareVeche = 0;
        if (e != 0) {
            // Trebuie sa inseram un triplet nou pe pozitia 'poz'
            
            // Redimensionare daca e plin
            if (size == capacity) {
                capacity *= RESIZE_FACTOR;
                Triplet* nou = new Triplet[capacity];
                for (int k = 0; k < size; k++) nou[k] = this->elemente[k];
                delete[] this->elemente;
                this->elemente = nou;
            }

            // Shiftam elementele la dreapta pentru a face loc noului triplet
            for (int k = size; k > poz; k--) {
                elemente[k] = elemente[k - 1];
            }

            // Inseram elementul
            elemente[poz].i = i;
            elemente[poz].j = j;
            elemente[poz].value = e;
            size++;
        }
        // Daca e == 0 si nu exista, nu facem nimic
    }

    return valoareVeche;
}



bool Matrice::relatie(Triplet a, Triplet b) const {
	// compare based on `i`
	if (a.i < b.i) return true;
	else if (a.i > b.i) return false;
	else {
		// compare based on `j`
		if(a.j<=b.j) return true;
		else return false;
	}
}

// Returneaza indexul unde se afla (i,j) SAU indexul unde ar trebui inserat
int Matrice::cautaPozitie(int i, int j) const {
    int stanga = 0;
    int dreapta = size - 1;
    int pozitieRezultat = size; // Default: la finalul listei

    while (stanga <= dreapta) {
        int mijloc = stanga + (dreapta - stanga) / 2;

        if (elemente[mijloc].i == i && elemente[mijloc].j == j) {
            return mijloc; // Gasit exact
        }

        // Daca target (i,j) este "mai mic" decat elementul de la mijloc
        // Folosim o logica similara cu functia relatie
        if (i < elemente[mijloc].i || (i == elemente[mijloc].i && j < elemente[mijloc].j)) {
            pozitieRezultat = mijloc;
            dreapta = mijloc - 1;
        } else {
            stanga = mijloc + 1;
        }
    }
    return (stanga > dreapta) ? stanga : pozitieRezultat;
}


// TODO: implementeaza destructor