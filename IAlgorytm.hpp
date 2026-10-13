#ifndef IALGORYTM_HPP
#define IALGORYTM_HPP

#include "Typy.hpp"
#include "Zgloszenie.hpp"
#include "Pracownik.hpp"
#include <memory>
#include <string>

using namespace std;

class IAlgorytm {
public:
    virtual ~IAlgorytm() = default;

    virtual void dodajZgloszenie(shared_ptr<Zgloszenie> zgloszenie) = 0;
    virtual shared_ptr<Zgloszenie> pobierzDlaPracownika(const Pracownik& pracownik, int aktualnyCzas) = 0;
    virtual bool czyPusta() const noexcept = 0;
    virtual size_t rozmiarKolejki() const noexcept = 0;
    virtual void aktualizuj(int aktualnyCzas) { (void)aktualnyCzas; }
    virtual void wyczysc() = 0;
    virtual string getNazwa() const = 0;
    virtual TypAlgorytmu getTyp() const noexcept = 0;
};

#endif
