#pragma once

#include <vector>

typedef int TCheie;
typedef int TValoare;

#include <utility>
typedef std::pair<TCheie, TValoare> TElem;

using namespace std;

class IteratorMDO;

typedef bool (*Relatie)(TCheie, TCheie);

struct Nod {
  TElem element;
  int stang;
  int drept;
};

class MDO {
  friend class IteratorMDO;

private:
  // pt tabloul dinamic de inlantuiri
  Nod *elemente;
  int cp;        // capacitate
  int size;      // nr curent de elemente
  int radacina;  // radacina ABC
  int primLiber; // primul element liber din tablou
  Relatie rel;

  void resize();
  int aloca();
  void dealoca(int i);

  void cauta_rec(int nod, TCheie c, vector<TValoare> &rez) const;
  bool sterge_rec(int &nod, TCheie c, TValoare v);

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
