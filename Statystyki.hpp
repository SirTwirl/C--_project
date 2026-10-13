#ifndef STATYSTYKI_HPP
#define STATYSTYKI_HPP

#include "Zgloszenie.hpp"
#include "Pracownik.hpp"
#include "Typy.hpp"
#include <vector>
#include <memory>
#include <string>

using namespace std;

struct StatystykaPracownika {
    int id;
    string specjalizacja;
    int liczbaObsluzonych;
    int lacznyCzasPracy;
    double stopienWykorzystania;
};

class Statystyki {
private:
    string nazwaAlgorytmu;
    TypAlgorytmu typAlgorytmu;
    int horyzontCzasowy;

    vector<shared_ptr<Zgloszenie>> zgloszeniaObsluzone;
    vector<shared_ptr<Zgloszenie>> zgloszeniaWToku;
    vector<shared_ptr<Zgloszenie>> zgloszeniaNieobsluzone;
    vector<StatystykaPracownika> statystykiPracownikow;

    mutable bool przeliczone{false};
    mutable double sredniCzasOczekiwania{0.0};
    mutable double odchylenieStdOczekiwania{0.0};
    mutable int minCzasOczekiwania{0};
    mutable int maxCzasOczekiwania{0};
    mutable double p50Oczekiwania{0.0};
    mutable double p90Oczekiwania{0.0};
    mutable double p95Oczekiwania{0.0};
    mutable double p99Oczekiwania{0.0};

    mutable double sredniCzasRealizacji{0.0};
    mutable int liczbaWTerminie{0};
    mutable int liczbaSpoznionych{0};
    mutable double procentSLA{0.0};
    mutable double srednieSpoznienie{0.0};
    mutable int maxSpoznienie{0};
    mutable double srednieWykorzystaniePracownikow{0.0};

    void przeliczMetryki() const;

public:
    Statystyki() = default;
    Statystyki(string nazwaAlgorytmu, TypAlgorytmu typ, int horyzontCzasowy);

    void zarejestrujObsluzone(shared_ptr<Zgloszenie> zgloszenie);
    void zarejestrujWToku(shared_ptr<Zgloszenie> zgloszenie);
    void zarejestrujNieobsluzone(shared_ptr<Zgloszenie> zgloszenie);
    void zarejestrujPracownikow(const vector<Pracownik>& pracownicy);

    const string& getNazwaAlgorytmu() const noexcept { return nazwaAlgorytmu; }
    TypAlgorytmu getTypAlgorytmu() const noexcept { return typAlgorytmu; }
    int getHoryzontCzasowy() const noexcept { return horyzontCzasowy; }

    size_t getLiczbaObsluzonych() const noexcept { return zgloszeniaObsluzone.size(); }
    size_t getLiczbaWToku() const noexcept { return zgloszeniaWToku.size(); }
    size_t getLiczbaNieobsluzonych() const noexcept { return zgloszeniaNieobsluzone.size(); }
    size_t getLiczbaCalkowita() const noexcept {
        return zgloszeniaObsluzone.size() + zgloszeniaWToku.size() + zgloszeniaNieobsluzone.size();
    }

    double getSredniCzasOczekiwania() const;
    double getOdchylenieStdOczekiwania() const;
    int getMinCzasOczekiwania() const;
    int getMaxCzasOczekiwania() const;
    double getP50Oczekiwania() const;
    double getP90Oczekiwania() const;
    double getP95Oczekiwania() const;
    double getP99Oczekiwania() const;

    double getSredniCzasRealizacji() const;
    int getLiczbaWTerminie() const;
    int getLiczbaSpoznionych() const;
    double getProcentSLA() const;
    double getSrednieSpoznienie() const;
    int getMaxSpoznienie() const;
    double getSrednieWykorzystaniePracownikow() const;

    const vector<StatystykaPracownika>& getStatystykiPracownikow() const noexcept {
        return statystykiPracownikow;
    }
    const vector<shared_ptr<Zgloszenie>>& getZgloszeniaObsluzone() const noexcept {
        return zgloszeniaObsluzone;
    }

    void zresetuj();
};

#endif
