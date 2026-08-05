#include "MDO.h"
#include "IteratorMDO.h"
#include <vector>

using namespace std;

MDO::MDO(Relatie r) {
  // Theta(numarul de elemente initiale in vectorul dinamic)

  rel = r;
  cp = 10; // alocam 10 elemente initial
  elemente = new Nod[cp];
  size = 0; // nu avem elemente la inceput
  radacina = -1;
  primLiber = 0;

  // inlantuim toate elementele din spatiul liber
  for (int i = 0; i < cp - 1; i++) {
    // !!! folosim stand pe post de "urmatorul liber"
    elemente[i].stang = i + 1; // urmatorul spatiu liber
    elemente[i].drept = -1;
  }
  elemente[cp - 1].stang = -1; // ultimul spatiu nu mai are nimic dupa el
  elemente[cp - 1].drept = -1;
}

void MDO::resize() {
  // BC=WC=AC= Theta(n) -> copiem toate elementele
  // O(n) amortizat

  int nouaCp = cp * 2;
  Nod *noiElemente = new Nod[nouaCp]; // alocam noua memorie

  // copiem elementele
  for (int i = 0; i < cp; i++) {
    noiElemente[i] = elemente[i];
  }

  // inlantuim noul spatiu liber
  for (int i = cp; i < nouaCp - 1; i++) {
    noiElemente[i].stang = i + 1;
    noiElemente[i].drept = -1;
  }
  noiElemente[nouaCp - 1].stang = primLiber;
  noiElemente[nouaCp - 1].drept = -1;

  // primul liber e la vechea capacitate (ca vectorul vechi era plin)
  primLiber = cp;
  // free memory
  delete[] elemente;
  elemente = noiElemente; // update vector
  cp = nouaCp;            // update capacitate
}

int MDO::aloca() {
  // avanseaza "pointerul" de primLiber
  // face alocare dinamica automat daca nu mai sunt locuri libere
  // returneaza pozitia pe care trebuie sa adaugam un element
  // BC = Theta(1) exista spatiu liber disponibil
  // WC = Theta(n) se face alocarea dinamica
  // AC = O(n) amortizat
  if (primLiber == -1) {
    resize();
  }
  int poz = primLiber;
  primLiber = elemente[primLiber].stang;
  return poz;
}

void MDO::dealoca(int i) {
  // pt stergere
  // zicem ca pointerul stang da catre primul liber => punem acest element in
  // capul listei inlantuite de spatii libere
  // Theta(1)

  elemente[i].stang = primLiber;
  elemente[i].drept = -1;
  primLiber = i; // am adaugat acest element ca primul liber
}

void MDO::adauga(TCheie c, TValoare v) {
  // BC = Theta(1) arborele e gol
  // AC = Theta(N) arborele este echilibrat
  // WC = Theta(N) complet degenerat (avem o lista inlantuita lunga)

  int nou = aloca(); // luam spatiul in care punem elementul

  // TElem = (cheie,valoare)
  elemente[nou].element = make_pair(c, v);
  // deoarece tocmai l-am adaugat, este frunza in ABC
  elemente[nou].stang = -1;
  elemente[nou].drept = -1;

  // daca e primul element, inseamna ca e si radacina
  if (radacina == -1) {
    radacina = nou;
    size++;
    return; // final
  }
  // ajunge aici daca arborele nu era gol

  // parcurgem arborele in functie de chei pana ajungem la
  // locul unde trebuie pusa noua frunza
  int curr = radacina;
  while (true) {
    // daca e mai mic (in relatie), merge in stanga
    if (rel(c, elemente[curr].element.first)) {
      // daca am ajuns la ultimul nivel (la o frunza)
      if (elemente[curr].stang == -1) {
        // pointam catre noua frunza (adaugam un child)
        elemente[curr].stang = nou;
        break;
      }
      // next
      curr = elemente[curr].stang;
    } else {
      // daca e mai mare (nu e in relatie)
      if (elemente[curr].drept == -1) {
        elemente[curr].drept = nou;
        break;
      }
      curr = elemente[curr].drept;
    }
  }
  size++; // creste numarul de elemente din ABC
}

void MDO::cauta_rec(int nod, TCheie c, vector<TValoare> &rez) const {

  // BC = Theta(1) arborele e gol
  // WC = Theta(n) parcurgem o lista simpla lunga
  // AC = Theta(h+k) (h aprox logN, k numarul de elemente care au cheia c)
  // (arborele e echilibrat)

  // am ajuns la o frunza
  if (nod == -1) {
    return;
  }
  // daca elementul curent are cheia buna, il punem in rezultat
  if (elemente[nod].element.first == c) {
    rez.push_back(elemente[nod].element.second);
  }

  // !!! daca elementul curent are cheia c, vom cauta si in stanga si in dreapta

  // mergi in stanga
  if (rel(c, elemente[nod].element.first)) {
    cauta_rec(elemente[nod].stang, c, rez);
  }
  // mergi in dreapta
  if (rel(elemente[nod].element.first, c)) {
    cauta_rec(elemente[nod].drept, c, rez);
  }
}

vector<TValoare> MDO::cauta(TCheie c) const {
  // Returneaza un vetor cu toate perechile (c,valoare) cu cheia c
  vector<TValoare> rez;
  cauta_rec(radacina, c, rez);
  return rez;
}

bool MDO::sterge_rec(int &nod, TCheie c, TValoare v) {
  // BC = Theta(1) arbore gol
  // WC = Theta(N) lista lunga
  // AC = Theta(log N)

  if (nod == -1) {
    return false;
  }

  // daca elementul curent e (cheie,valoare), am gasit nodul ce trebuie sters
  if (elemente[nod].element.first == c && elemente[nod].element.second == v) {
    if (elemente[nod].stang == -1 && elemente[nod].drept == -1) {
      // daca e frunza, doar dealocam
      int deSters = nod;
      nod = -1;
      dealoca(deSters);
    } else if (elemente[nod].stang == -1) {
      // are doar copil drept, legam parintele
      int deSters = nod;
      nod = elemente[nod].drept;
      dealoca(deSters);
    } else if (elemente[nod].drept == -1) {
      // are doar copil stand, legam parintele
      int deSters = nod;
      nod = elemente[nod].stang;
      dealoca(deSters);
    } else {
      // nodul are 2 copii: cautam succesorul (minimul din subarborele drept)
      int temp = elemente[nod].drept;
      while (elemente[temp].stang != -1) {
        temp = elemente[temp].stang;
      }
      // copiem informatia succesorului in nodul curent
      TElem succElem = elemente[temp].element;
      elemente[nod].element = succElem;
      // stergem succesorul recursiv din subarborele drept (ca sa nu le dublam)
      sterge_rec(elemente[nod].drept, succElem.first, succElem.second);
    }
    return true;
  }

  // altfel, cautam recursiv in jos
  if (rel(c, elemente[nod].element.first)) {
    if (sterge_rec(elemente[nod].stang, c, v)) {
      return true;
    }
  }
  if (rel(elemente[nod].element.first, c)) {
    if (sterge_rec(elemente[nod].drept, c, v)) {
      return true;
    }
  }

  return false;
}

bool MDO::sterge(TCheie c, TValoare v) {
  bool sters = sterge_rec(radacina, c, v);
  if (sters) {
    size--;
  }
  return sters;
}

int MDO::dim() const { return size; }

bool MDO::vid() const { return size == 0; }

IteratorMDO MDO::iterator() const { return IteratorMDO(*this); }

MDO::~MDO() { delete[] elemente; }
