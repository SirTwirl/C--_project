#ifndef TYPY_HPP
#define TYPY_HPP

#include <string>

using namespace std;

enum class StanPracownika {
    Wolny,
    Zajety
};

enum class StanZgloszenia {
    Oczekuje,
    WRealizacji,
    Zakonczone,
    Przeterminowane
};

enum class TypAlgorytmu {
    FIFO = 1,
    Priorytetowy = 2,
    EDF = 3,
    PriorytetAging = 4
};

inline string typAlgorytmuNaString(TypAlgorytmu typ) {
    switch (typ) {
        case TypAlgorytmu::FIFO:
            return "FIFO / FCFS (Kolejnosc nadejscia)";
        case TypAlgorytmu::Priorytetowy:
            return "HPF (Highest Priority First)";
        case TypAlgorytmu::EDF:
            return "EDF (Earliest Deadline First)";
        case TypAlgorytmu::PriorytetAging:
            return "HPF z Aging (Priorytet ze starzeniem - przeciw glodzeniu)";
        default:
            return "Nieznany";
    }
}

inline string stanPracownikaNaString(StanPracownika stan) {
    return (stan == StanPracownika::Wolny) ? "WOLNY" : "ZAJETY";
}

inline string stanZgloszeniaNaString(StanZgloszenia stan) {
    switch (stan) {
        case StanZgloszenia::Oczekuje: return "OCZEKUJE";
        case StanZgloszenia::WRealizacji: return "W_REALIZACJI";
        case StanZgloszenia::Zakonczone: return "ZAKONCZONE";
        case StanZgloszenia::Przeterminowane: return "PRZETERMINOWANE";
        default: return "NIEZNANY";
    }
}

#endif
