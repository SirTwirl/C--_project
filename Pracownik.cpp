#include "Pracownik.hpp"
#include <utility>

using namespace std;

Pracownik::Pracownik(int id, string specjalizacja)
    : id(id),
      specjalizacja(move(specjalizacja)),
      stan(StanPracownika::Wolny),
      aktualneZgloszenie(nullptr),
      czasZakonczeniaPracy(-1),
      lacznyCzasPracy(0),
      liczbaObsluzonych(0) {}

bool Pracownik::czyMozeObsluzyc(const string& wymagana) const noexcept {
    return specjalizacja == wymagana;
}

void Pracownik::przydzielZgloszenie(shared_ptr<Zgloszenie> zgloszenie, int aktualnyCzas) {
    stan = StanPracownika::Zajety;
    aktualneZgloszenie = zgloszenie;
    czasZakonczeniaPracy = aktualnyCzas + zgloszenie->getCzasObslugi();
    zgloszenie->rozpocznijObsluge(aktualnyCzas, id);
}

shared_ptr<Zgloszenie> Pracownik::zwolnijPracownika(int aktualnyCzas) {
    if (!aktualneZgloszenie) {
        return nullptr;
    }
    auto zakonczone = aktualneZgloszenie;
    lacznyCzasPracy += zakonczone->getCzasObslugi();
    liczbaObsluzonych++;
    zakonczone->zakonczObsluge(aktualnyCzas);

    aktualneZgloszenie = nullptr;
    stan = StanPracownika::Wolny;
    czasZakonczeniaPracy = -1;
    return zakonczone;
}

double Pracownik::getStopienWykorzystania(int calkowityCzas) const noexcept {
    if (calkowityCzas <= 0) return 0.0;
    return (static_cast<double>(lacznyCzasPracy) / calkowityCzas) * 100.0;
}

void Pracownik::zresetuj() {
    stan = StanPracownika::Wolny;
    aktualneZgloszenie = nullptr;
    czasZakonczeniaPracy = -1;
    lacznyCzasPracy = 0;
    liczbaObsluzonych = 0;
}

ostream& operator<<(ostream& os, const Pracownik& p) {
    os << "Pracownik #" << p.id << " [" << p.specjalizacja << "] Stan: "
       << stanPracownikaNaString(p.stan) << ", Obsluzonych: " << p.liczbaObsluzonych
       << ", CzasPracy: " << p.lacznyCzasPracy << " min";
    return os;
}
