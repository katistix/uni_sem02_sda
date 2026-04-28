#include "MDO.h"
#include "IteratorMDO.h"
#include <iostream>
#include <utility>
#include <vector>

#include <exception>
using namespace std;

MDO::MDO(Relatie r) {
  // initial nu avem inceput sau sfarsit
  this->prim = nullptr;
  this->ultim = nullptr;
  this->n = 0;
  this->rel = r;
}

void MDO::adauga(TCheie c, TValoare v) {
  TElem e = make_pair(c, v);
  Nod *nou = new Nod(e, nullptr, nullptr);

  // daca lista de vida, noul nod devine si prim si ultim
  if (this->vid()) {
    this->prim = nou;
    this->ultim = nou;
  } else {
    // cautam pozitia corecta folosind relatia de ordine
    Nod *curent = this->prim;
    while (curent != nullptr && !this->rel(c, curent->e.first)) {
      curent = curent->urm;
    }

    // inseram in functie de unde s-a oprit curent
    // inceput
    if (curent == this->prim) {
      nou->urm = this->prim;  // a->b
      this->prim->prec = nou; // a<-b
      this->prim = nou;
    }
    // sfarsit
    else if (curent == nullptr) {
      nou->prec = this->ultim;
      this->ultim->urm = nou;
      this->ultim = nou;
    }
    // in interior
    else {
      nou->urm = curent;
      nou->prec = curent->prec;
      curent->prec->urm = nou;
      curent->prec = nou;
    }
  }

  this->n++; // creste dimensiunea
}

vector<TValoare> MDO::cauta(TCheie c) const {
  vector<TValoare> valori;

  // parcurgem de la primul
  Nod *curent = this->prim;

  // cat timp nu am ajuns la final, avansam
  while (curent != nullptr) {
    if (curent->e.first == c) {
      valori.push_back(curent->e.second);
    }

    // optimizare, daca am trecut de zona unde cheia cautata este, atunci ne
    // oprim
    if (!this->rel(curent->e.first, c) && curent->e.first != c) {
      break;
    }

    curent = curent->urm; // avansam
  }

  return valori;
}

bool MDO::sterge(TCheie c, TValoare v) {
  Nod *curent = this->prim;

  // parcurgem
  while (curent != nullptr) {
    // daca nodul contine perechea (cheie, valoare) pe care vrem sa o stergem
    if (curent->e.first == c && curent->e.second == v) {
      // mai multe moduri de a sterge in functie de pozitia nodului
      if (curent == this->prim && curent == this->ultim) {
        // singurul element din lista
        this->prim = nullptr;
        this->ultim = nullptr;
      } else if (curent == this->prim) {
        // este primul element
        this->prim = this->prim->urm;
        this->prim->prec = nullptr;
      } else if (curent == this->ultim) {
        // este ultimul element
        this->ultim = this->ultim->prec;
        this->ultim->urm = nullptr;
      } else {
        // este in mijloc
        curent->prec->urm = curent->urm;
        curent->urm->prec = curent->prec;
      }

      // dealocam memorie si scadem numarul de noduri
      delete curent;
      this->n--;

      // stergem un singur element, primul gasit
      return true;
    }
    // avansam
    curent = curent->urm;
  }
  return false;
}

int MDO::dim() const { return this->n; }

bool MDO::vid() const { return this->n == 0; }

IteratorMDO MDO::iterator() const { return IteratorMDO(*this); }

MDO::~MDO() {
  // parcurgem toate nodurile si dealocam memoria
  Nod *curent = this->prim;
  while (curent != nullptr) {
    Nod *deSters = curent;
    curent = curent->urm;
    delete deSters;
  }
}
