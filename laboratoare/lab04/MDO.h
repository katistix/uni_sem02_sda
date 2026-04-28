#pragma once

#include <vector>

typedef int TCheie;
typedef int TValoare;

#include <utility>
typedef std::pair<TCheie, TValoare> TElem;

using namespace std;

class IteratorMDO;

typedef bool (*Relatie)(TCheie, TCheie);

// definim un nod pentru LDI
struct Nod {
  TElem e;   // (cheie,valoare)
  Nod *urm;  // next
  Nod *prec; // prev
             // constructor pt un nod
  Nod(TElem e, Nod *urm, Nod *prec) : e(e), urm(urm), prec(prec) {}
};

class MDO {
  friend class IteratorMDO;

private:
  /* aici e reprezentarea */

  // avem referinte la primul si ultimul element
  Nod *prim;
  Nod *ultim;
  int n;       // numarul de chei
  Relatie rel; // relatia de ordine dintre chei

public:
  // constructorul implicit al MultiDictionarului Ordonat
  MDO(Relatie r);

  // adauga o pereche (cheie, valoare) in MDO
  void adauga(TCheie c, TValoare v);

  // cauta o cheie si returneaza vectorul de valori asociate
  vector<TValoare> cauta(TCheie c) const;

  // sterge o cheie si o valoare
  // returneaza adevarat daca s-a gasit cheia si valoarea de sters
  bool sterge(TCheie c, TValoare v);

  // returneaza numarul de perechi (cheie, valoare) din MDO
  int dim() const;

  // verifica daca MultiDictionarul Ordonat e vid
  bool vid() const;

  // se returneaza iterator pe MDO
  // iteratorul va returna perechile in ordine in raport cu relatia de ordine
  IteratorMDO iterator() const;

  // destructorul
  ~MDO();
};
