#include "IteratorMDO.h"
#include "MDO.h"
#include <exception>

using namespace std;

IteratorMDO::IteratorMDO(const MDO &d) : dict(d) {
  // setam curent ca primul element din dictionar
  this->curent = d.prim;
}

void IteratorMDO::prim() { this->curent = this->dict.prim; }

void IteratorMDO::urmator() {
  if (!this->valid()) {
    throw exception();
  }
  this->curent = this->dict.urm[this->curent];
}

bool IteratorMDO::valid() const { return this->curent != -1; }

TElem IteratorMDO::element() const {
  if (!this->valid()) {
    throw exception();
  }
  return this->dict.e[this->curent];
}
