#ifndef ALGORYTMB_HPP
#define ALGORYTMB_HPP

#include "IAlgorytm.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

struct KomparatorPriorytetowy {
    bool operator()(const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) const noexcept {
        if (!a || !b) return false;
        if (a->getPriorytetBazowy() != b->getPriorytetBazowy()) {
            return a->getPriorytetBazowy() < b->getPriorytetBazowy();
        }
        if (a->getCzasWejscia() != b->getCzasWejscia()) {
            return a->getCzasWejscia() > b->getCzasWejscia();
        }
        return a->getId() > b->getId();
    }
};

class AlgorytmB : public IAlgorytm {
private:
    using KolejkaPrio = priority_queue<shared_ptr<Zgloszenie>,
                                       vector<shared_ptr<Zgloszenie>>,
                                       KomparatorPriorytetowy>;
    unordered_map<string, KolejkaPrio> kolejki;
    size_t lacznaLiczba;

public:
    AlgorytmB();

    void dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) override;
    shared_ptr<Zgloszenie> pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) override;
    bool czyPusta() const noexcept override;
    size_t rozmiarKolejki() const noexcept override;
    void wyczysc() override;
    string getNazwa() const override;
    TypAlgorytmu getTyp() const noexcept override;
};

#endif
