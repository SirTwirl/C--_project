#include "Usluga.hpp"
#include <utility>

using namespace std;

Usluga::Usluga(string id, string nazwa, string kategoria,
               string wymaganaSpecjalizacja, int domyslnyCzas, int priorytet)
    : id(move(id)),
      nazwa(move(nazwa)),
      kategoria(move(kategoria)),
      wymaganaSpecjalizacja(move(wymaganaSpecjalizacja)),
      domyslnyCzas(domyslnyCzas),
      priorytet(priorytet) {}

ostream& operator<<(ostream& os, const Usluga& u) {
    os << "[" << u.id << "] " << u.nazwa << " (" << u.kategoria
       << ") Specjalizacja: " << u.wymaganaSpecjalizacja
       << ", Czas: " << u.domyslnyCzas << " min, Priorytet: " << u.priorytet;
    return os;
}
