#include "AlgorytmC.hpp"

using namespace std;

AlgorytmC::AlgorytmC() : lacznaLiczba(0) {}

void AlgorytmC::dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) {
    if (!zgloszenie) return;
    const string& spec = zgloszenie->getWymaganaSpecjalizacja();
    kolejki[spec].push(zgloszenie);
    lacznaLiczba++;
}

shared_ptr<Zgloszenie> AlgorytmC::pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) {
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

bool AlgorytmC::czyPusta() const noexcept {
    return lacznaLiczba == 0;
}

size_t AlgorytmC::rozmiarKolejki() const noexcept {
    return lacznaLiczba;
}

void AlgorytmC::wyczysc() {
    kolejki.clear();
    lacznaLiczba = 0;
}

string AlgorytmC::getNazwa() const {
    return "Strategia C: Earliest Deadline First (EDF, priority_queue)";
}

TypAlgorytmu AlgorytmC::getTyp() const noexcept {
    return TypAlgorytmu::EDF;
}
