#ifndef ALGORYTMC_HPP
#define ALGORYTMC_HPP

#include "IAlgorytm.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

struct KomparatorEDF {
    bool operator()(const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) const noexcept {
        if (!a || !b) return false;
        if (a->getTermin() != b->getTermin()) {
            return a->getTermin() > b->getTermin();
        }
        if (a->getCzasWejscia() != b->getCzasWejscia()) {
            return a->getCzasWejscia() > b->getCzasWejscia();
        }
        return a->getId() > b->getId();
    }
};

class AlgorytmC : public IAlgorytm {
private:
    using KolejkaEDF = priority_queue<shared_ptr<Zgloszenie>,
                                      vector<shared_ptr<Zgloszenie>>,
                                      KomparatorEDF>;
    unordered_map<string, KolejkaEDF> kolejki;
    size_t lacznaLiczba;

public:
    AlgorytmC();

    void dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) override;
    shared_ptr<Zgloszenie> pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) override;
    bool czyPusta() const noexcept override;
    size_t rozmiarKolejki() const noexcept override;
    void wyczysc() override;
    string getNazwa() const override;
    TypAlgorytmu getTyp() const noexcept override;
};

#endif
