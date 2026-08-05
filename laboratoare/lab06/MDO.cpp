#include "MDO.h"
#include "IteratorMDO.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

#include <exception>
using namespace std;

MDO::MDO(Relatie r) {
  this->rel = r;
  this->m = TD_SIZE;
  this->nrPerechi = 0;
  this->tabela = new CheieNod *[m]; // alocam m pozitii in TD

  // tabela initial e goala
  for (int i = 0; i < m; i++) {
    tabela[i] = nullptr;
  }
}

void MDO::resize() {
  // Theta(n)

  // dublam capacitatea
  int capacitateNoua = this->m * 2;

  // alocam memorie
  CheieNod **tabelaNoua = new CheieNod *[capacitateNoua];
  for (int i = 0; i < capacitateNoua; i++) {
    tabelaNoua[i] = nullptr;
  }

  // mutam elementele in noua tabela
  for (int i = 0; i < this->m; i++) {
    CheieNod *curent = this->tabela[i];
    // pt fiecare cheie, o
    while (curent != nullptr) {
      CheieNod *urmator = curent->urm;

      // rehashing nod
      int pozNoua = hash(curent->cheie, capacitateNoua);

      curent->urm = tabelaNoua[pozNoua];
      tabelaNoua[pozNoua] = curent;

      curent = urmator;
    }
  }

  // dealocam memorie
  delete[] this->tabela;

  // legam noua tabela
  this->tabela = tabelaNoua;
  this->m = capacitateNoua;
}

void MDO::adauga(TCheie c, TValoare v) {
  // BC = Theta(1)
  // AC = WC = Theta(1) + O(n)

  int resizeLimit = pow(2, m);
  if (nrPerechi > resizeLimit) {
    this->resize();
  }

  int poz = this->hash(c, m); // calculam pozitia in TD

  // parcurgem LSI-ul pana cand gasim cheia
  CheieNod *curent = this->tabela[poz];
  while (curent != nullptr) {
    // daca am gasit cheia, adaugam valoarea in vectorul ei
    if (curent->cheie == c) {
      curent->valori.push_back(v);
      this->nrPerechi++; // creste numarul de elemente stocate in MDO
      return;            // am terminat de adaugat
    }

    // daca nu suntem la cheie inca, mergem la urmatoarea
    curent = curent->urm;
  }

  // daca nu am gasit cheia, o adaugam la inceputul LSI-ului
  CheieNod *nodNou = new CheieNod;
  nodNou->cheie = c;
  nodNou->valori.push_back(v);
  nodNou->urm = tabela[poz];
  this->tabela[poz] = nodNou;
  this->nrPerechi++;
}

vector<TValoare> MDO::cauta(TCheie c) const {
  // BC = Theta(1)
  // AC=WC = O(n)

  int poz = this->hash(c, m);

  // parcurgem LSI-ul pana cand gasim cheia
  CheieNod *curent = this->tabela[poz];
  while (curent != nullptr) {
    // daca am gasit cheia, returnam valorile din aceasta cheie
    if (curent->cheie == c) {
      return curent->valori;
    }

    // daca nu suntem la cheie inca, mergem la urmatoarea
    curent = curent->urm;
  }

  return vector<TValoare>(); // returnam un vector gol daca nu exista cheia
}

bool MDO::sterge(TCheie c, TValoare v) {
  int poz = this->hash(c, m);

  // parcurgem LSI-ul pana cand gasim cheia
  CheieNod *anterior = nullptr;
  CheieNod *curent = this->tabela[poz];
  while (curent != nullptr) {

    // daca am gasit cheia, cautam valoarea
    if (curent->cheie == c) {
      bool valoareGasita = false;

      for (int j = 0; j < curent->valori.size(); j++) {
        if (curent->valori[j] == v) {
          curent->valori.erase(curent->valori.begin() + j);
          nrPerechi--;
          valoareGasita = true;
          break;
        }
      }
      if (!valoareGasita) {
        return false;
      }

      // daca lista e goala, stergem si nodul
      if (curent->valori.empty()) {
        if (anterior == nullptr) {
          tabela[poz] = curent->urm;
        } else {
          anterior->urm = curent->urm;
        }
        // dealocam memorie
        delete curent;
      }

      return true;
    }

    // daca nu suntem la cheie inca, mergem la urmatoarea
    anterior = curent;
    curent = curent->urm;
  }

  return false;
}

int MDO::dim() const {
  // Theta(1)
  return this->nrPerechi;
}

bool MDO::vid() const {
  // Theta(1)
  return this->nrPerechi == 0;
}

IteratorMDO MDO::iterator() const { return IteratorMDO(*this); }

// functia de hash
int MDO::hash(TCheie c, int capacitate) const {
  return (c % capacitate + capacitate) % m;
}

MDO::~MDO() {
  // Theta(n)

  // pt fiecare bucket
  for (int i = 0; i < this->m; i++) {
    // sterge fiecare nod
    CheieNod *curent = this->tabela[i];
    while (curent != nullptr) {
      CheieNod *urmator = curent->urm;
      delete curent;
      curent = urmator;
    }
  }

  // dealocam vectorul de bucketuri
  delete[] this->tabela;
}
