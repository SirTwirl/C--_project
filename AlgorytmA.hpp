#ifndef ALGORYTMA_HPP
#define ALGORYTMA_HPP

#include "IAlgorytm.hpp"
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

class AlgorytmA : public IAlgorytm {
private:
    unordered_map<string, queue<shared_ptr<Zgloszenie>>> kolejki;
    size_t lacznaLiczba;

public:
    AlgorytmA();

    void dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) override;
    shared_ptr<Zgloszenie> pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) override;
    bool czyPusta() const noexcept override;
    size_t rozmiarKolejki() const noexcept override;
    void wyczysc() override;
    string getNazwa() const override;
    TypAlgorytmu getTyp() const noexcept override;
};

#endif
