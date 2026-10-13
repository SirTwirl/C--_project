#include "GeneratorZgloszen.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

GeneratorZgloszen::GeneratorZgloszen(unsigned int seed)
    : seed(seed), rng(seed) {}

void GeneratorZgloszen::ustawSeed(unsigned int nowySeed) {
    seed = nowySeed;
    rng.seed(seed);
}

vector<ZgloszenieInfo> GeneratorZgloszen::generuj(size_t liczbaZgloszen,
                                                const KatalogUslug& katalog,
                                                ProfilObciazenia profil,
                                                int maxCzasHoryzontu) {
    vector<ZgloszenieInfo> wynik;
    wynik.reserve(liczbaZgloszen);

    auto listaUslug = katalog.jakoWektor();
    if (listaUslug.empty()) {
        cerr << "[Blad Generatora] Katalog uslug jest pusty!" << endl;
        return wynik;
    }

    vector<double> wagi;
    wagi.reserve(listaUslug.size());
    for (const auto& u : listaUslug) {
        double waga = 100.0 / sqrt(static_cast<double>(u.getDomyslnyCzas()));
        if (u.getPriorytet() == 4) waga *= 0.2;
        wagi.push_back(waga);
    }
    discrete_distribution<size_t> distUslugi(wagi.begin(), wagi.end());

    const vector<string> typyUrzadzen = {
        "Laptop", "PC Stacjonarny", "Smartfon", "Tablet", "Serwer RACK", "Dysk HDD/SSD"
    };
    const vector<string> marki = {
        "Dell", "Lenovo", "HP", "Apple", "Asus", "Samsung", "Xiaomi", "Kingston", "Seagate", "Synology"
    };

    uniform_int_distribution<size_t> distTyp(0, typyUrzadzen.size() - 1);
    uniform_int_distribution<size_t> distMarka(0, marki.size() - 1);

    double lambda = 0.15;
    if (profil == ProfilObciazenia::Niskie) lambda = 0.05;
    else if (profil == ProfilObciazenia::Przeciazenie) lambda = 0.40;
    else if (profil == ProfilObciazenia::DuzaSkala10k) lambda = 0.50;

    int biezacyCzas = 0;

    for (size_t i = 1; i <= liczbaZgloszen; ++i) {
        if (profil == ProfilObciazenia::SkokoweBurst) {
            if (biezacyCzas >= 100 && biezacyCzas <= 160) {
                lambda = 1.20;
            } else if (biezacyCzas > 160) {
                lambda = 0.08;
            } else {
                lambda = 0.05;
            }
        }

        exponential_distribution<double> distInterArrival(lambda);
        double delta = distInterArrival(rng);
        int krok = max(0, static_cast<int>(round(delta)));
        biezacyCzas += krok;

        if (profil != ProfilObciazenia::DuzaSkala10k && biezacyCzas > maxCzasHoryzontu) {
            biezacyCzas = maxCzasHoryzontu;
        }

        size_t idxUslugi = distUslugi(rng);
        const auto& usluga = listaUslug[idxUslugi];

        int marginesMin = 60;
        int marginesMax = 240;
        if (usluga.getPriorytet() == 4) {
            marginesMin = 30;
            marginesMax = 90;
        } else if (usluga.getPriorytet() == 3) {
            marginesMin = 45;
            marginesMax = 150;
        } else if (usluga.getPriorytet() == 1) {
            marginesMin = 180;
            marginesMax = 480;
        }

        uniform_int_distribution<int> distSlack(marginesMin, marginesMax);
        int slack = distSlack(rng);
        int termin = biezacyCzas + usluga.getDomyslnyCzas() + slack;

        ZgloszenieInfo info;
        info.idZgloszenia = static_cast<int>(i);
        info.idUslugi = usluga.getId();
        info.typUrzadzenia = typyUrzadzen[distTyp(rng)];
        info.marka = marki[distMarka(rng)];
        info.czasWejscia = biezacyCzas;
        info.termin = termin;

        wynik.push_back(info);
    }

    return wynik;
}

bool GeneratorZgloszen::zapiszDoPliku(const string& sciezka, const vector<ZgloszenieInfo>& zgloszenia) {
    ofstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Generatora] Nie udalo sie otworzyc pliku do zapisu: " << sciezka << endl;
        return false;
    }

    plik << "# ID_zgloszenia;ID_uslugi;typ_urzadzenia;marka_urzadzenia;czas_wejscia;termin\n";
    for (const auto& z : zgloszenia) {
        plik << z.idZgloszenia << ";"
             << z.idUslugi << ";"
             << z.typUrzadzenia << ";"
             << z.marka << ";"
             << z.czasWejscia << ";"
             << z.termin << "\n";
    }

    plik.close();
    return true;
}
