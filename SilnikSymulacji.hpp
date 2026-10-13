#ifndef SILNIKSYMULACJI_HPP
#define SILNIKSYMULACJI_HPP

#include "Typy.hpp"
#include "KatalogUslug.hpp"
#include "Pracownik.hpp"
#include "Zgloszenie.hpp"
#include "IAlgorytm.hpp"
#include "Statystyki.hpp"
#include "input.hpp"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class SilnikSymulacji {
private:
    int maxCzas;
    KatalogUslug katalog;
    vector<Pracownik> pracownicySzablon;
    vector<Pracownik> pracownicy;
    vector<shared_ptr<Zgloszenie>> zgloszenia;
    unique_ptr<IAlgorytm> algorytm;
    Statystyki statystyki;

public:
    SilnikSymulacji();
    explicit SilnikSymulacji(int maxCzas);

    void zainicjalizujZKatalogu(const KatalogUslug& kat, const vector<PracownikInfo>& infoPracownicy);
    void dodajZgloszenie(shared_ptr<Zgloszenie> z);
    void ustawZgloszenia(const vector<shared_ptr<Zgloszenie>>& lista);
    void wczytajZgloszeniaZInfo(const vector<ZgloszenieInfo>& infoZgloszenia);

    void ustawAlgorytm(unique_ptr<IAlgorytm> nowyAlgorytm);
    void ustawMaxCzas(int czas) noexcept { maxCzas = czas; }
    int getMaxCzas() const noexcept { return maxCzas; }

    void uruchom(bool dokonczWszystkieCzyDoLimitu = false);
    void zresetujStan();

    const Statystyki& getStatystyki() const noexcept { return statystyki; }
    const vector<Pracownik>& getPracownicy() const noexcept { return pracownicy; }
    const KatalogUslug& getKatalog() const noexcept { return katalog; }
    const vector<shared_ptr<Zgloszenie>>& getZgloszenia() const noexcept { return zgloszenia; }
};

#endif
