#include "IteratorMDO.h"
#include "MDO.h"
#include <exception>

using namespace std;

IteratorMDO::IteratorMDO(const MDO &d) : dict(d) { prim(); }

void IteratorMDO::prim() {
  // BC = Theta(1)
  // WC = AC = Theta(h)
  stiva.clear(); // goleste "call-stack"-ul

  // mergi tot la stanga, pana la cel mai mic element
  int curr = dict.radacina;
  while (curr != -1) {
    stiva.push_back(curr);
    curr = dict.elemente[curr].stang;
  }
}

void IteratorMDO::urmator() {
  // BC = Theta(1)
  // WC = Theta(h)

  if (!valid()) {
    throw exception();
  }
  int curr = stiva.back();
  stiva.pop_back();

  // mergi in dreapta
  curr = dict.elemente[curr].drept;
  while (curr != -1) {
    stiva.push_back(curr);
    curr = dict.elemente[curr].stang;
  }
}

bool IteratorMDO::valid() const { return !stiva.empty(); }

TElem IteratorMDO::element() const {
  if (!valid()) {
    throw exception();
  }
  return dict.elemente[stiva.back()].element;
}
