#include "AlgorytmA.hpp"

using namespace std;

AlgorytmA::AlgorytmA() : lacznaLiczba(0) {}

void AlgorytmA::dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) {
    if (!zgloszenie) return;
    const string& spec = zgloszenie->getWymaganaSpecjalizacja();
    kolejki[spec].push(zgloszenie);
    lacznaLiczba++;
}

shared_ptr<Zgloszenie> AlgorytmA::pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) {
    (void)aktualnyCzas;
    auto it = kolejki.find(pracownik.getSpecjalizacja());
    if (it == kolejki.end() || it->second.empty()) {
        return nullptr;
    }

    auto zgloszenie = it->second.front();
    it->second.pop();
    lacznaLiczba--;
    return zgloszenie;
}

bool AlgorytmA::czyPusta() const noexcept {
    return lacznaLiczba == 0;
}

size_t AlgorytmA::rozmiarKolejki() const noexcept {
    return lacznaLiczba;
}

void AlgorytmA::wyczysc() {
    kolejki.clear();
    lacznaLiczba = 0;
}

string AlgorytmA::getNazwa() const {
    return "Strategia A: FIFO / FCFS (queue)";
}

TypAlgorytmu AlgorytmA::getTyp() const noexcept {
    return TypAlgorytmu::FIFO;
}
