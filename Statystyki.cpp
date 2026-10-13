#include "Statystyki.hpp"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

Statystyki::Statystyki(string nazwaAlgorytmu, TypAlgorytmu typ, int horyzontCzasowy)
    : nazwaAlgorytmu(move(nazwaAlgorytmu)),
      typAlgorytmu(typ),
      horyzontCzasowy(horyzontCzasowy) {}

void Statystyki::zarejestrujObsluzone(shared_ptr<Zgloszenie> zgloszenie) {
    if (zgloszenie) {
        zgloszeniaObsluzone.push_back(zgloszenie);
        przeliczone = false;
    }
}

void Statystyki::zarejestrujWToku(shared_ptr<Zgloszenie> zgloszenie) {
    if (zgloszenie) {
        zgloszeniaWToku.push_back(zgloszenie);
        przeliczone = false;
    }
}

void Statystyki::zarejestrujNieobsluzone(shared_ptr<Zgloszenie> zgloszenie) {
    if (zgloszenie) {
        zgloszeniaNieobsluzone.push_back(zgloszenie);
        przeliczone = false;
    }
}

void Statystyki::zarejestrujPracownikow(const vector<Pracownik>& pracownicy) {
    statystykiPracownikow.clear();
    double sumaWykorzystania = 0.0;
    for (const auto& p : pracownicy) {
        double ut = p.getStopienWykorzystania(horyzontCzasowy);
        statystykiPracownikow.push_back({
            p.getId(),
            p.getSpecjalizacja(),
            p.getLiczbaObsluzonych(),
            p.getLacznyCzasPracy(),
            ut
        });
        sumaWykorzystania += ut;
    }
    if (!pracownicy.empty()) {
        srednieWykorzystaniePracownikow = sumaWykorzystania / pracownicy.size();
    } else {
        srednieWykorzystaniePracownikow = 0.0;
    }
    przeliczone = false;
}

static double obliczPercentyl(const vector<int>& posortowane, double procent) {
    if (posortowane.empty()) return 0.0;
    if (posortowane.size() == 1) return posortowane[0];

    double rank = (procent / 100.0) * static_cast<double>(posortowane.size() - 1);
    size_t dolnyIndeks = static_cast<size_t>(floor(rank));
    size_t gornyIndeks = static_cast<size_t>(ceil(rank));
    double waga = rank - dolnyIndeks;

    if (dolnyIndeks == gornyIndeks) {
        return posortowane[dolnyIndeks];
    }
    return posortowane[dolnyIndeks] + waga * (posortowane[gornyIndeks] - posortowane[dolnyIndeks]);
}

void Statystyki::przeliczMetryki() const {
    if (przeliczone) return;

    if (zgloszeniaObsluzone.empty()) {
        sredniCzasOczekiwania = 0.0;
        odchylenieStdOczekiwania = 0.0;
        minCzasOczekiwania = 0;
        maxCzasOczekiwania = 0;
        p50Oczekiwania = 0.0;
        p90Oczekiwania = 0.0;
        p95Oczekiwania = 0.0;
        p99Oczekiwania = 0.0;
        sredniCzasRealizacji = 0.0;
        liczbaWTerminie = 0;
        liczbaSpoznionych = 0;
        procentSLA = 0.0;
        srednieSpoznienie = 0.0;
        maxSpoznienie = 0;
        przeliczone = true;
        return;
    }

    vector<int> czasyOczekiwania;
    czasyOczekiwania.reserve(zgloszeniaObsluzone.size());
    long long sumaOczekiwania = 0;
    long long sumaRealizacji = 0;
    long long sumaSpoznien = 0;

    minCzasOczekiwania = zgloszeniaObsluzone.front()->getCzasOczekiwania();
    maxCzasOczekiwania = minCzasOczekiwania;
    maxSpoznienie = 0;
    liczbaWTerminie = 0;
    liczbaSpoznionych = 0;

    for (const auto& z : zgloszeniaObsluzone) {
        int wait = z->getCzasOczekiwania();
        czasyOczekiwania.push_back(wait);
        sumaOczekiwania += wait;
        sumaRealizacji += z->getCzasObslugi();

        if (wait < minCzasOczekiwania) minCzasOczekiwania = wait;
        if (wait > maxCzasOczekiwania) maxCzasOczekiwania = wait;

        if (z->czyDotrzymanoTerminu()) {
            liczbaWTerminie++;
        } else {
            liczbaSpoznionych++;
            int spoznienie = z->getSpoznienie();
            sumaSpoznien += spoznienie;
            if (spoznienie > maxSpoznienie) {
                maxSpoznienie = spoznienie;
            }
        }
    }

    const double n = static_cast<double>(zgloszeniaObsluzone.size());
    sredniCzasOczekiwania = static_cast<double>(sumaOczekiwania) / n;
    sredniCzasRealizacji = static_cast<double>(sumaRealizacji) / n;
    procentSLA = (static_cast<double>(liczbaWTerminie) / n) * 100.0;
    srednieSpoznienie = (liczbaSpoznionych > 0) ? (static_cast<double>(sumaSpoznien) / liczbaSpoznionych) : 0.0;

    double sumaKwadratowRoznic = 0.0;
    for (int wait : czasyOczekiwania) {
        double roznica = static_cast<double>(wait) - sredniCzasOczekiwania;
        sumaKwadratowRoznic += roznica * roznica;
    }
    odchylenieStdOczekiwania = sqrt(sumaKwadratowRoznic / n);

    sort(czasyOczekiwania.begin(), czasyOczekiwania.end());
    p50Oczekiwania = obliczPercentyl(czasyOczekiwania, 50.0);
    p90Oczekiwania = obliczPercentyl(czasyOczekiwania, 90.0);
    p95Oczekiwania = obliczPercentyl(czasyOczekiwania, 95.0);
    p99Oczekiwania = obliczPercentyl(czasyOczekiwania, 99.0);

    przeliczone = true;
}

double Statystyki::getSredniCzasOczekiwania() const {
    przeliczMetryki();
    return sredniCzasOczekiwania;
}

double Statystyki::getOdchylenieStdOczekiwania() const {
    przeliczMetryki();
    return odchylenieStdOczekiwania;
}

int Statystyki::getMinCzasOczekiwania() const {
    przeliczMetryki();
    return minCzasOczekiwania;
}

int Statystyki::getMaxCzasOczekiwania() const {
    przeliczMetryki();
    return maxCzasOczekiwania;
}

double Statystyki::getP50Oczekiwania() const {
    przeliczMetryki();
    return p50Oczekiwania;
}

double Statystyki::getP90Oczekiwania() const {
    przeliczMetryki();
    return p90Oczekiwania;
}

double Statystyki::getP95Oczekiwania() const {
    przeliczMetryki();
    return p95Oczekiwania;
}

double Statystyki::getP99Oczekiwania() const {
    przeliczMetryki();
    return p99Oczekiwania;
}

double Statystyki::getSredniCzasRealizacji() const {
    przeliczMetryki();
    return sredniCzasRealizacji;
}

int Statystyki::getLiczbaWTerminie() const {
    przeliczMetryki();
    return liczbaWTerminie;
}

int Statystyki::getLiczbaSpoznionych() const {
    przeliczMetryki();
    return liczbaSpoznionych;
}

double Statystyki::getProcentSLA() const {
    przeliczMetryki();
    return procentSLA;
}

double Statystyki::getSrednieSpoznienie() const {
    przeliczMetryki();
    return srednieSpoznienie;
}

int Statystyki::getMaxSpoznienie() const {
    przeliczMetryki();
    return maxSpoznienie;
}

double Statystyki::getSrednieWykorzystaniePracownikow() const {
    przeliczMetryki();
    return srednieWykorzystaniePracownikow;
}

void Statystyki::zresetuj() {
    zgloszeniaObsluzone.clear();
    zgloszeniaWToku.clear();
    zgloszeniaNieobsluzone.clear();
    statystykiPracownikow.clear();
    przeliczone = false;
}
