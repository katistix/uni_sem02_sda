#include "IteratorMDO.h"
#include "MDO.h"
#include <exception>

IteratorMDO::IteratorMDO(const MDO &d) : dict(d) {
  this->poz = 0;

  // punem toate elementele din MDO in sir

  // pt fiecare cheie din TD
  for (int i = 0; i < dict.m; i++) {

    CheieNod *curent = dict.tabela[i];
    // parcurgem LSI-ul
    while (curent != nullptr) {
      for (int j = 0; j < curent->valori.size(); j++) {
        this->elems.push_back(
            {curent->cheie,
             curent->valori[j]}); // adaugam in elems perechea (cheie, valoare)
      }
      curent = curent->urm; // next
    }
  }

  // sortam elementele dupa relatia de ordine din MDO
  sort(this->elems.begin(), this->elems.end(),
       [this](const TElem &a, const TElem &b) {
         return this->dict.rel(a.first, b.first);
       });
}

void IteratorMDO::prim() {
  // sari la prima pozitie
  this->poz = 0;
}

void IteratorMDO::urmator() {
  // vezi sa fie valid
  if (!this->valid()) {
    throw exception();
  }

  this->poz++;
}

bool IteratorMDO::valid() const {
  // vezi sa nu fi la final
  if (poz >= elems.size()) {
    return false;
  }

  return true;
}

TElem IteratorMDO::element() const {
  if (!this->valid()) {
    throw exception();
  }

  return this->elems[poz];
}
