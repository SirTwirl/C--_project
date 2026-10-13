#ifndef ZGLOSZENIE_HPP
#define ZGLOSZENIE_HPP

#include "Typy.hpp"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class Zgloszenie {
private:
    int id;
    string idUslugi;
    string typUrzadzenia;
    string marka;
    int czasWejscia;
    int termin;
    int czasObslugi;
    int priorytetBazowy;
    int priorytetDynamiczny;
    string wymaganaSpecjalizacja;

    StanZgloszenia stan;
    int czasRozpoczecia;
    int czasZakonczenia;
    int idPrzydzielonegoPracownika;

public:
    Zgloszenie(int id, string idUslugi, string typUrzadzenia,
               string marka, int czasWejscia, int termin,
               int czasObslugi, int priorytet, string wymaganaSpecjalizacja);

    int getId() const noexcept { return id; }
    const string& getIdUslugi() const noexcept { return idUslugi; }
    const string& getTypUrzadzenia() const noexcept { return typUrzadzenia; }
    const string& getMarka() const noexcept { return marka; }
    int getCzasWejscia() const noexcept { return czasWejscia; }
    int getTermin() const noexcept { return termin; }
    int getCzasObslugi() const noexcept { return czasObslugi; }
    int getPriorytetBazowy() const noexcept { return priorytetBazowy; }
    int getPriorytetDynamiczny() const noexcept { return priorytetDynamiczny; }
    const string& getWymaganaSpecjalizacja() const noexcept { return wymaganaSpecjalizacja; }
    StanZgloszenia getStan() const noexcept { return stan; }
    int getCzasRozpoczecia() const noexcept { return czasRozpoczecia; }
    int getCzasZakonczenia() const noexcept { return czasZakonczenia; }
    int getIdPrzydzielonegoPracownika() const noexcept { return idPrzydzielonegoPracownika; }

    int getCzasOczekiwania() const noexcept {
        if (czasRozpoczecia >= 0) {
            return czasRozpoczecia - czasWejscia;
        }
        return 0;
    }

    int getCzasOczekiwania(int aktualnyCzas) const noexcept {
        if (czasRozpoczecia >= 0) {
            return czasRozpoczecia - czasWejscia;
        }
        return max(0, aktualnyCzas - czasWejscia);
    }

    bool czyDotrzymanoTerminu() const noexcept {
        if (czasZakonczenia < 0) return false;
        return czasZakonczenia <= termin;
    }

    int getSpoznienie() const noexcept {
        if (czasZakonczenia < 0) return 0;
        return max(0, czasZakonczenia - termin);
    }

    void rozpocznijObsluge(int czas, int idPracownika);
    void zakonczObsluge(int czas);
    void oznaczJakoPrzeterminowane();

    void zwiekszPriorytetAging(int delta) noexcept {
        priorytetDynamiczny += delta;
    }
    void zresetujPriorytetAging() noexcept {
        priorytetDynamiczny = priorytetBazowy;
    }

    friend ostream& operator<<(ostream& os, const Zgloszenie& z);
};

#endif
