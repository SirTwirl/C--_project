#include "AlgorytmB.hpp"

using namespace std;

AlgorytmB::AlgorytmB() : lacznaLiczba(0) {}

void AlgorytmB::dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) {
    if (!zgloszenie) return;
    const string& spec = zgloszenie->getWymaganaSpecjalizacja();
    kolejki[spec].push(zgloszenie);
    lacznaLiczba++;
}

shared_ptr<Zgloszenie> AlgorytmB::pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) {
    (void)aktualnyCzas;
    auto it = kolejki.find(pracownik.getSpecjalizacja());
    if (it == kolejki.end() || it->second.empty()) {
        return nullptr;
    }

    auto zgloszenie = it->second.top();
    it->second.pop();
    lacznaLiczba--;
    return zgloszenie;
}

bool AlgorytmB::czyPusta() const noexcept {
    return lacznaLiczba == 0;
}

size_t AlgorytmB::rozmiarKolejki() const noexcept {
    return lacznaLiczba;
}

void AlgorytmB::wyczysc() {
    kolejki.clear();
    lacznaLiczba = 0;
}

string AlgorytmB::getNazwa() const {
    return "Strategia B: Highest Priority First (priority_queue)";
}

TypAlgorytmu AlgorytmB::getTyp() const noexcept {
    return TypAlgorytmu::Priorytetowy;
}
