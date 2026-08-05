#pragma once

#include <vector>

typedef int TCheie;
typedef int TValoare;

#define TD_SIZE 1300

#include <utility>
typedef std::pair<TCheie, TValoare> TElem;

using namespace std;

struct CheieNod {
  TCheie cheie;
  vector<TValoare> valori;
  CheieNod *urm;
};

class IteratorMDO;

typedef bool (*Relatie)(TCheie, TCheie);

class MDO {
  friend class IteratorMDO;

private:
  /* aici e reprezentarea */

  CheieNod **tabela; // tabela de dispersie lista de pointeri la liste simplu
                     // inlantuite
  int m;             // dimensiunea tabelei
  int nrPerechi;     // totalul de perechi (cheie, valoare) (elemente efective)

  int hash(TCheie c, int capacitate) const; // hash function-ul pentru TD
  void resize();

  Relatie rel;

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
