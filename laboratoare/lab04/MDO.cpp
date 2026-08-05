#include "MDO.h"
#include "IteratorMDO.h"
#include <utility>
#include <vector>
using namespace std;

MDO::MDO(Relatie r) {
  this->rel = r;
  this->cp = 10;
  this->e = new TElem[cp];
  this->urm = new int[cp];
  this->prec = new int[cp];
  this->prim = -1;
  this->ultim = -1;
  this->primLiber = 0;
  for (int i = 0; i < cp - 1; i++) {
    this->urm[i] = i + 1;
  }
  this->urm[cp - 1] = -1;
  this->n = 0;
}

int MDO::aloca() {
  // Theta(1)
  int i = this->primLiber;
  this->primLiber = this->urm[this->primLiber];
  return i;
}

void MDO::dealoca(int i) {
  // Theta(1)
  this->urm[i] = this->primLiber;
  this->primLiber = i;
}

void MDO::redimensioneaza() {
  // O(n)
  TElem *eNou = new TElem[2 * cp];
  int *urmNou = new int[2 * cp];
  int *precNou = new int[2 * cp];

  for (int i = 0; i < cp; i++) {
    eNou[i] = this->e[i];
    urmNou[i] = this->urm[i];
    precNou[i] = this->prec[i];
  }

  for (int i = cp; i < 2 * cp - 1; i++) {
    urmNou[i] = i + 1;
  }
  urmNou[2 * cp - 1] = -1;
  this->primLiber = cp;

  // dealoca vectorii vechi
  delete[] this->e;
  delete[] this->urm;
  delete[] this->prec;

  this->e = eNou;
  this->urm = urmNou;
  this->prec = precNou;
  this->cp = 2 * cp;
}

void MDO::adauga(TCheie c, TValoare v) {
  // BC =  Theta(1) (e cea mai mica cheie)
  // O(n) amortizat (se face redimensionarea tot mai rar)
  if (this->primLiber == -1) {
    redimensioneaza();
  }
  int nou = aloca();
  this->e[nou] = make_pair(c, v);
  this->urm[nou] = -1;
  this->prec[nou] = -1;

  if (vid()) {
    this->prim = nou;
    this->ultim = nou;
  } else {
    int curent = this->prim;
    while (curent != -1 && !this->rel(c, this->e[curent].first)) {
      curent = this->urm[curent];
    }

    if (curent == this->prim) {
      this->urm[nou] = this->prim;
      this->prec[this->prim] = nou;
      this->prim = nou;
    } else if (curent == -1) {
      this->prec[nou] = this->ultim;
      this->urm[this->ultim] = nou;
      this->ultim = nou;
    } else {
      this->urm[nou] = curent;
      this->prec[nou] = this->prec[curent];
      this->urm[this->prec[curent]] = nou;
      this->prec[curent] = nou;
    }
  }
  this->n++;
}

vector<TValoare> MDO::cauta(TCheie c) const {
  // Cautarea
  // BC (cea mai mica cheie) = Theta(1)
  // WC (cea mai mare cheie) = AC = O(n)
  vector<TValoare> valori;
  int curent = this->prim;

  while (curent != -1) {
    if (this->e[curent].first == c) {
      valori.push_back(this->e[curent].second);
    }

    if (!this->rel(this->e[curent].first, c) && this->e[curent].first != c) {
      break;
    }

    curent = this->urm[curent];
  }

  return valori;
}

bool MDO::sterge(TCheie c, TValoare v) {
  // Cauta cheia
  // WC = AC = O(n)
  // BC = Theta(1)
  int curent = this->prim;

  while (curent != -1) {
    if (this->e[curent].first == c && this->e[curent].second == v) {
      if (curent == this->prim && curent == this->ultim) {
        this->prim = -1;
        this->ultim = -1;
      } else if (curent == this->prim) {
        this->prim = this->urm[this->prim];
        this->prec[this->prim] = -1;
      } else if (curent == this->ultim) {
        this->ultim = this->prec[this->ultim];
        this->urm[this->ultim] = -1;
      } else {
        this->urm[this->prec[curent]] = this->urm[curent];
        this->prec[this->urm[curent]] = this->prec[curent];
      }

      dealoca(curent);
      this->n--;

      return true;
    }
    curent = this->urm[curent];
  }
  return false;
}

int MDO::dim() const {
  // Theta(1)
  return this->n;
}

bool MDO::vid() const {
  // Theta(1)
  return this->n == 0;
}

IteratorMDO MDO::iterator() const { return IteratorMDO(*this); }

MDO::~MDO() {
  // Theta(1)
  delete[] this->e;
  delete[] this->urm;
  delete[] this->prec;
}
