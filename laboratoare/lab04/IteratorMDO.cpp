#include "IteratorMDO.h"
#include "MDO.h"
#include <exception>

IteratorMDO::IteratorMDO(const MDO &d) : dict(d) {
  // setam curent ca primul element din dictionar
  this->curent = d.prim;
}

void IteratorMDO::prim() { this->curent = this->dict.prim; }

void IteratorMDO::urmator() {
  if (!this->valid()) {
    throw exception();
  }
  this->curent = this->curent->urm;
}

bool IteratorMDO::valid() const { return this->curent != nullptr; }

TElem IteratorMDO::element() const {
  if (!this->valid()) {
    throw exception();
  }
  return this->curent->e;
}
