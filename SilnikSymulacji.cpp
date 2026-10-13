#include "SilnikSymulacji.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

SilnikSymulacji::SilnikSymulacji() : maxCzas(480) {}

SilnikSymulacji::SilnikSymulacji(int maxCzas) : maxCzas(maxCzas) {}

void SilnikSymulacji::zainicjalizujZKatalogu(const KatalogUslug& kat, const vector<PracownikInfo>& infoPracownicy) {
    katalog = kat;
    pracownicySzablon.clear();
    for (const auto& p : infoPracownicy) {
        pracownicySzablon.emplace_back(p.id, p.specjalizacja);
    }
    pracownicy = pracownicySzablon;
}

void SilnikSymulacji::dodajZgloszenie(shared_ptr<Zgloszenie> z) {
    if (z) {
        zgloszenia.push_back(z);
    }
}

void SilnikSymulacji::ustawZgloszenia(const vector<shared_ptr<Zgloszenie>>& lista) {
    zgloszenia = lista;
}

void SilnikSymulacji::wczytajZgloszeniaZInfo(const vector<ZgloszenieInfo>& infoZgloszenia) {
    zgloszenia.clear();
    for (const auto& info : infoZgloszenia) {
        auto optUsluga = katalog.pobierzUsluge(info.idUslugi);
        int czasObslugi = optUsluga ? optUsluga->getDomyslnyCzas() : 60;
        int priorytet = optUsluga ? optUsluga->getPriorytet() : 1;
        string wymagana = optUsluga ? optUsluga->getWymaganaSpecjalizacja() : "JuniorSoftware";

        auto z = make_shared<Zgloszenie>(
            info.idZgloszenia,
            info.idUslugi,
            info.typUrzadzenia,
            info.marka,
            info.czasWejscia,
            info.termin,
            czasObslugi,
            priorytet,
            wymagana
        );
        zgloszenia.push_back(z);
    }
}

void SilnikSymulacji::ustawAlgorytm(unique_ptr<IAlgorytm> nowyAlgorytm) {
    algorytm = move(nowyAlgorytm);
}

void SilnikSymulacji::zresetujStan() {
    pracownicy = pracownicySzablon;
    for (auto& p : pracownicy) {
        p.zresetuj();
    }
    if (algorytm) {
        algorytm->wyczysc();
    }
    statystyki.zresetuj();
}

void SilnikSymulacji::uruchom(bool dokonczWszystkieCzyDoLimitu) {
    if (!algorytm) {
        cerr << "[Blad Silnika] Brak ustawionego algorytmu kolejkowania!" << endl;
        return;
    }

    pracownicy = pracownicySzablon;
    for (auto& p : pracownicy) {
        p.zresetuj();
    }
    algorytm->wyczysc();

    vector<shared_ptr<Zgloszenie>> aktywneZgloszenia;
    aktywneZgloszenia.reserve(zgloszenia.size());
    for (const auto& z : zgloszenia) {
        auto kopia = make_shared<Zgloszenie>(
            z->getId(),
            z->getIdUslugi(),
            z->getTypUrzadzenia(),
            z->getMarka(),
            z->getCzasWejscia(),
            z->getTermin(),
            z->getCzasObslugi(),
            z->getPriorytetBazowy(),
            z->getWymaganaSpecjalizacja()
        );
        aktywneZgloszenia.push_back(kopia);
    }

    sort(aktywneZgloszenia.begin(), aktywneZgloszenia.end(),
         [](const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) {
             if (a->getCzasWejscia() != b->getCzasWejscia()) {
                 return a->getCzasWejscia() < b->getCzasWejscia();
             }
             return a->getId() < b->getId();
         });

    statystyki = Statystyki(algorytm->getNazwa(), algorytm->getTyp(), maxCzas);

    size_t indeksNastepnego = 0;
    const size_t lacznieZgloszen = aktywneZgloszenia.size();

    int t = 0;
    while (true) {
        if (!dokonczWszystkieCzyDoLimitu) {
            if (t > maxCzas) break;
        } else {
            bool czyKtosPracuje = false;
            for (const auto& p : pracownicy) {
                if (!p.czyWolny()) {
                    czyKtosPracuje = true;
                    break;
                }
            }
            if (t >= maxCzas && indeksNastepnego >= lacznieZgloszen && algorytm->czyPusta() && !czyKtosPracuje) {
                break;
            }
        }

        for (auto& p : pracownicy) {
            if (!p.czyWolny() && p.getCzasZakonczeniaPracy() == t) {
                auto ukonczone = p.zwolnijPracownika(t);
                if (ukonczone) {
                    statystyki.zarejestrujObsluzone(ukonczone);
                }
            }
        }

        while (indeksNastepnego < lacznieZgloszen && aktywneZgloszenia[indeksNastepnego]->getCzasWejscia() == t) {
            algorytm->dodajZgloszenie(aktywneZgloszenia[indeksNastepnego]);
            indeksNastepnego++;
        }

        algorytm->aktualizuj(t);

        for (auto& p : pracownicy) {
            if (p.czyWolny() && !algorytm->czyPusta()) {
                auto zgloszenie = algorytm->pobierzDlaPracownika(p, t);
                if (zgloszenie) {
                    p.przydzielZgloszenie(zgloszenie, t);
                }
            }
        }

        t++;
    }

    statystyki.zarejestrujPracownikow(pracownicy);

    for (const auto& p : pracownicy) {
        if (!p.czyWolny()) {
            statystyki.zarejestrujWToku(p.getAktualneZgloszenie());
        }
    }

    while (indeksNastepnego < lacznieZgloszen) {
        statystyki.zarejestrujNieobsluzone(aktywneZgloszenia[indeksNastepnego]);
        indeksNastepnego++;
    }
}
