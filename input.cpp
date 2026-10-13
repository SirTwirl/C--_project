#include "input.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

static string usunSpacje(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

static vector<string> podziel(const string& linia, char separator) {
    vector<string> fragmenty;
    stringstream ss(linia);
    string fragment;

    while (getline(ss, fragment, separator)) {
        fragmenty.push_back(usunSpacje(fragment));
    }
    return fragmenty;
}

bool Input::wczytajKonfiguracje(const string& sciezka, KonfiguracjaData& outKonfig) {
    ifstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Odczytu] Nie udalo sie otworzyc pliku konfiguracji: " << sciezka << endl;
        return false;
    }

    string linia;
    string aktualnaSekcja = "";
    int nrLinii = 0;

    while (getline(plik, linia)) {
        nrLinii++;
        linia = usunSpacje(linia);

        if (linia.empty() || linia[0] == '#') {
            continue;
        }

        if (linia == "[PRACOWNICY]") {
            aktualnaSekcja = "PRACOWNICY";
            continue;
        } else if (linia == "[USLUGI]") {
            aktualnaSekcja = "USLUGI";
            continue;
        }

        try {
            if (aktualnaSekcja.empty()) {
                size_t znakRownosci = linia.find('=');
                if (znakRownosci != string::npos) {
                    string klucz = usunSpacje(linia.substr(0, znakRownosci));
                    string wartosc = usunSpacje(linia.substr(znakRownosci + 1));

                    if (klucz == "ALGORYTM") {
                        outKonfig.algorytm = stoi(wartosc);
                    } else if (klucz == "MAX_CZAS_SYMULACJI") {
                        outKonfig.maxCzas = stoi(wartosc);
                    }
                }
            } else if (aktualnaSekcja == "PRACOWNICY") {
                auto kolumny = podziel(linia, ';');
                if (kolumny.size() >= 2) {
                    PracownikInfo p;
                    p.id = stoi(kolumny[0]);
                    p.specjalizacja = kolumny[1];
                    outKonfig.pracownicy.push_back(p);
                } else {
                    cerr << "[Ostrzezenie] Linia " << nrLinii << " w [PRACOWNICY] ma niepoprawna liczbe kolumn: " << linia << endl;
                }
            } else if (aktualnaSekcja == "USLUGI") {
                auto kolumny = podziel(linia, ';');
                if (kolumny.size() >= 6) {
                    UslugaInfo u;
                    u.id = kolumny[0];
                    u.nazwa = kolumny[1];
                    u.kategoria = kolumny[2];
                    u.specjalizacja = kolumny[3];
                    u.czas = stoi(kolumny[4]);
                    u.priorytet = stoi(kolumny[5]);
                    outKonfig.uslugi.push_back(u);
                } else {
                    cerr << "[Ostrzezenie] Linia " << nrLinii << " w [USLUGI] ma niepoprawna liczbe kolumn: " << linia << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "[Blad Formatowania] Linia " << nrLinii << ": '" << linia
                 << "' zawiera niepoprawne dane liczbowe (" << e.what() << "). Pomijanie linii." << endl;
        }
    }

    plik.close();
    return true;
}

bool Input::wczytajZgloszenia(const string& sciezka, vector<ZgloszenieInfo>& outZgloszenia) {
    ifstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Odczytu] Nie udalo sie otworzyc pliku zgloszen: " << sciezka << endl;
        return false;
    }

    string linia;
    int nrLinii = 0;
    while (getline(plik, linia)) {
        nrLinii++;
        linia = usunSpacje(linia);

        if (linia.empty() || linia[0] == '#') {
            continue;
        }

        auto kolumny = podziel(linia, ';');
        if (kolumny.size() >= 6) {
            try {
                ZgloszenieInfo z;
                z.idZgloszenia = stoi(kolumny[0]);
                z.idUslugi = kolumny[1];
                z.typUrzadzenia = kolumny[2];
                z.marka = kolumny[3];
                z.czasWejscia = stoi(kolumny[4]);
                z.termin = stoi(kolumny[5]);

                outZgloszenia.push_back(z);
            } catch (const exception& e) {
                cerr << "[Blad Formatowania] Linia zgloszenia " << nrLinii << ": '" << linia
                     << "' zawiera niepoprawne dane liczbowe (" << e.what() << "). Pomijanie." << endl;
            }
        } else {
            cerr << "[Ostrzezenie] Linia " << nrLinii << " zawiera za malo kolumn (" << kolumny.size()
                 << " z 6 wymaganych). Linia: " << linia << endl;
        }
    }

    plik.close();
    return true;
}