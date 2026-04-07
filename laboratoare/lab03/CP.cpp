
#include "CP.h"
#include <exception>
#include <stdio.h>

using namespace std;


CP::CP(Relatie r) {
	// Theta(1)
	this->relatie = r;
	this->startNode = nullptr; // initial lista e goala
}


/// @brief Cauta pozitia unde ar trebui inserat un element cu prioritatea p, pornind de la startNode
/// @param startNode 
/// @param p 
/// @return Nodul anterior celui cu prioritatea p sau nullptr daca p ar trebui sa fie primul element
Nod* CP::cautaPozitie(TPrioritate p) const {
	// BC -> Theta(1) = daca nodul trebuie sa fie primul element, atunci nu intram in while
	// WC -> O(n) = daca nodul trebuie sa fie ultimul element, atunci parcurgem toata lista

	Nod* current = this->startNode;
    Nod* prev = nullptr;

	// cat timp nodul curent nu e null si prioritatea lui e mai mare decat p, continuam sa cautam
	while (current != nullptr && relatie(current->value.second, p)) {
        prev = current;
        current = current->next;
    }
    return prev;
}


/// @brief Adauga un element e cu prioritatea p in coada de prioritati
/// @param e Elementul de adaugat
/// @param p Prioritatea elementului
void CP::adauga(TElem e, TPrioritate p) {
	// Best case: coada e goala: Theta(1)
	if(this->vida()) {
		Nod *newNode = new Nod();

		newNode->value.first = e;
		newNode->value.second = p;
		newNode->next = nullptr;

		// il punem ca startNode
		this->startNode = newNode;

		return; 
	}

	// Cautare: O(n) worst case, Theta(n) best case

	Nod *prevNode = cautaPozitie(p);



	if (prevNode == nullptr){
		// inseamna ca elementul cu prioritatea p ar trebui sa fie primul element din coada
		Nod *newNode = new Nod();

		newNode->value.first = e;
		newNode->value.second = p;
		newNode->next = this->startNode;

		this->startNode = newNode; // actualizam startNode

		return;
	}

	// inseamna ca elementul cu prioritatea p ar trebui sa fie dupa prevNode
	Nod *newNode = new Nod();
	newNode->value.first = e;
	newNode->value.second = p;
	newNode->next = prevNode->next; // legam newNode de urmatorul nod dupa prevNode

	prevNode->next = newNode; // legam prevNode de newNode
}

/// @brief Returneaza elementul cu prioritatea cea mai mare (primul element din coada)
/// @return Elementul cu prioritatea cea mai mare
/// @throws exception daca coada e vida
Element CP::element() const {
	// Theta(1)
	if (this->vida()) {
		throw exception();
	}


	return this->startNode->value;
}

/// @brief Sterge elementul cu prioritatea cea mai mare (primul element din coada) si il returneaza
/// @return Elementul sters
/// @throws exception daca coada e vida
Element CP::sterge() {
	// Theta(1) pentru stergerea primului element, care e cel cu prioritatea cea mai mare

	if (this->vida()) {
		throw exception();
	}


	Element elem = this->startNode->value; // salvam elementul care trebuie sters
	Nod *nodeToDelete = this->startNode; // salvam nodul care trebuie sters
	this->startNode = this->startNode->next; // actualizam startNode sa fie urmatorul nod

	delete nodeToDelete; // stergem nodul sters
	return elem; // returnam elementul sters
}

/// @brief Verifica daca coada de prioritati e vida
/// @return true daca coada e vida, false altfel
bool CP::vida() const {
	// Theta(1) = verificam doar daca startNode e nullptr sau nu
	if (this->startNode == nullptr) {
		return true;
	}
	return false;
}

/// @brief Destructorul cozii de prioritati, care elibereaza memoria alocata pentru noduri
CP::~CP() {
	// WC -> Theta(n) = daca coada are n elemente, atunci parcurgem toata lista pentru a sterge fiecare nod
	// BC -> Theta(1) = daca coada e goala, atunci nu intram in while

	// parcurgem toata lista si stergem fiecare nod
	Nod *currentNode = this->startNode;
	// pana cand nu mai exista nodul next, adica pana cand currentNode devine nullptr
	while (currentNode != nullptr) {
		Nod *nodeToDelete = currentNode; // salvam nodul care trebuie sters
		currentNode = currentNode->next; // actualizam currentNode sa fie urmatorul nod
		delete nodeToDelete; // stergem nodul sters
	}
};



