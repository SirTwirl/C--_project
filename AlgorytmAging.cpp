#include "AlgorytmAging.hpp"
#include <algorithm>

using namespace std;

AlgorytmAging::AlgorytmAging(int interwalAging)
    : lacznaLiczba(0), interwalAging(interwalAging > 0 ? interwalAging : 15) {}

void AlgorytmAging::dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) {
    if (!zgloszenie) return;
    const string& spec = zgloszenie->getWymaganaSpecjalizacja();
    kolejki[spec].push_back(zgloszenie);
    lacznaLiczba++;
}

void AlgorytmAging::aktualizuj(int aktualnyCzas) {
    for (auto& [spec, lista] : kolejki) {
        for (auto& zgloszenie : lista) {
            if (zgloszenie) {
                int czasOczekiwania = zgloszenie->getCzasOczekiwania(aktualnyCzas);
                int bonus = czasOczekiwania / interwalAging;
                zgloszenie->zresetujPriorytetAging();
                zgloszenie->zwiekszPriorytetAging(bonus);
            }
        }
    }
}

shared_ptr<Zgloszenie> AlgorytmAging::pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) {
    auto it = kolejki.find(pracownik.getSpecjalizacja());
    if (it == kolejki.end() || it->second.empty()) {
        return nullptr;
    }

    auto& lista = it->second;

    for (auto& z : lista) {
        if (z) {
            int oczekiwanie = z->getCzasOczekiwania(aktualnyCzas);
            int bonus = oczekiwanie / interwalAging;
            z->zresetujPriorytetAging();
            z->zwiekszPriorytetAging(bonus);
        }
    }

    auto najlepszyIt = max_element(lista.begin(), lista.end(),
        [](const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) {
            if (a->getPriorytetDynamiczny() != b->getPriorytetDynamiczny()) {
                return a->getPriorytetDynamiczny() < b->getPriorytetDynamiczny();
            }
            if (a->getCzasWejscia() != b->getCzasWejscia()) {
                return a->getCzasWejscia() > b->getCzasWejscia();
            }
            return a->getId() > b->getId();
        });

    if (najlepszyIt == lista.end()) {
        return nullptr;
    }

    auto wybrane = *najlepszyIt;
    lista.erase(najlepszyIt);
    lacznaLiczba--;
    return wybrane;
}

bool AlgorytmAging::czyPusta() const noexcept {
    return lacznaLiczba == 0;
}

size_t AlgorytmAging::rozmiarKolejki() const noexcept {
    return lacznaLiczba;
}

void AlgorytmAging::wyczysc() {
    kolejki.clear();
    lacznaLiczba = 0;
}

string AlgorytmAging::getNazwa() const {
    return "Strategia D: HPF z Aging (Odpornosc na glodzenie / Starvation-free)";
}

TypAlgorytmu AlgorytmAging::getTyp() const noexcept {
    return TypAlgorytmu::PriorytetAging;
}
