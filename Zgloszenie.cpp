#include "Zgloszenie.hpp"
#include <utility>

using namespace std;

Zgloszenie::Zgloszenie(int id, string idUslugi, string typUrzadzenia,
                       string marka, int czasWejscia, int termin,
                       int czasObslugi, int priorytet, string wymaganaSpecjalizacja)
    : id(id),
      idUslugi(move(idUslugi)),
      typUrzadzenia(move(typUrzadzenia)),
      marka(move(marka)),
      czasWejscia(czasWejscia),
      termin(termin),
      czasObslugi(czasObslugi),
      priorytetBazowy(priorytet),
      priorytetDynamiczny(priorytet),
      wymaganaSpecjalizacja(move(wymaganaSpecjalizacja)),
      stan(StanZgloszenia::Oczekuje),
      czasRozpoczecia(-1),
      czasZakonczenia(-1),
      idPrzydzielonegoPracownika(-1) {}

void Zgloszenie::rozpocznijObsluge(int czas, int idPracownika) {
    stan = StanZgloszenia::WRealizacji;
    czasRozpoczecia = czas;
    idPrzydzielonegoPracownika = idPracownika;
}

void Zgloszenie::zakonczObsluge(int czas) {
    stan = StanZgloszenia::Zakonczone;
    czasZakonczenia = czas;
}

void Zgloszenie::oznaczJakoPrzeterminowane() {
    stan = StanZgloszenia::Przeterminowane;
}

ostream& operator<<(ostream& os, const Zgloszenie& z) {
    os << "Zgloszenie #" << z.id << " [" << z.idUslugi << " - " << z.wymaganaSpecjalizacja
       << "] Urzadzenie: " << z.typUrzadzenia << " " << z.marka
       << ", Wejscie: " << z.czasWejscia << ", Termin: " << z.termin
       << ", CzasTrwania: " << z.czasObslugi << " min, PrioBazowe: " << z.priorytetBazowy
       << ", PrioDyn: " << z.priorytetDynamiczny << ", Stan: " << stanZgloszeniaNaString(z.stan);
    return os;
}
