#ifndef PRACOWNIK_HPP
#define PRACOWNIK_HPP

#include "Typy.hpp"
#include "Zgloszenie.hpp"
#include <string>
#include <memory>
#include <iostream>

using namespace std;

class Pracownik {
private:
    int id;
    string specjalizacja;
    StanPracownika stan;
    shared_ptr<Zgloszenie> aktualneZgloszenie;
    int czasZakonczeniaPracy;
    int lacznyCzasPracy;
    int liczbaObsluzonych;

public:
    Pracownik(int id, string specjalizacja);

    int getId() const noexcept { return id; }
    const string& getSpecjalizacja() const noexcept { return specjalizacja; }
    StanPracownika getStan() const noexcept { return stan; }
    int getCzasZakonczeniaPracy() const noexcept { return czasZakonczeniaPracy; }
    int getLacznyCzasPracy() const noexcept { return lacznyCzasPracy; }
    int getLiczbaObsluzonych() const noexcept { return liczbaObsluzonych; }
    shared_ptr<Zgloszenie> getAktualneZgloszenie() const noexcept { return aktualneZgloszenie; }

    bool czyWolny() const noexcept { return stan == StanPracownika::Wolny; }
    bool czyMozeObsluzyc(const string& wymagana) const noexcept;

    void przydzielZgloszenie(shared_ptr<Zgloszenie> zgloszenie, int aktualnyCzas);
    shared_ptr<Zgloszenie> zwolnijPracownika(int aktualnyCzas);

    double getStopienWykorzystania(int calkowityCzas) const noexcept;

    void zresetuj();

    friend ostream& operator<<(ostream& os, const Pracownik& p);
};

#endif
