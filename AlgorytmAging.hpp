#ifndef ALGORYTMAGING_HPP
#define ALGORYTMAGING_HPP

#include "IAlgorytm.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

class AlgorytmAging : public IAlgorytm {
private:
    unordered_map<string, vector<shared_ptr<Zgloszenie>>> kolejki;
    size_t lacznaLiczba;
    int interwalAging;

public:
    explicit AlgorytmAging(int interwalAging = 15);

    void dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) override;
    shared_ptr<Zgloszenie> pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) override;
    bool czyPusta() const noexcept override;
    size_t rozmiarKolejki() const noexcept override;
    void aktualizuj(int aktualnyCzas) override;
    void wyczysc() override;
    string getNazwa() const override;
    TypAlgorytmu getTyp() const noexcept override;

    int getInterwalAging() const noexcept { return interwalAging; }
    void setInterwalAging(int interwal) noexcept { interwalAging = interwal; }
};

#endif
