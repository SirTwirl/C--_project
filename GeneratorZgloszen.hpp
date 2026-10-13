#ifndef GENERATORZGLOSZEN_HPP
#define GENERATORZGLOSZEN_HPP

#include "KatalogUslug.hpp"
#include "input.hpp"
#include <string>
#include <vector>
#include <random>

using namespace std;

enum class ProfilObciazenia {
    Niskie,
    Zrownowazone,
    Przeciazenie,
    SkokoweBurst,
    DuzaSkala10k
};

class GeneratorZgloszen {
private:
    unsigned int seed;
    mt19937_64 rng;

public:
    explicit GeneratorZgloszen(unsigned int seed = 42);

    void ustawSeed(unsigned int nowySeed);

    vector<ZgloszenieInfo> generuj(size_t liczbaZgloszen,
                                   const KatalogUslug& katalog,
                                   ProfilObciazenia profil,
                                   int maxCzasHoryzontu = 480);

    static bool zapiszDoPliku(const string& sciezka, const vector<ZgloszenieInfo>& zgloszenia);
};

#endif
