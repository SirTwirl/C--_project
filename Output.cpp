#include "Output.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

static string generujRaportTekstowy(const Statystyki& s) {
    ostringstream oss;
    oss << "================================================================================\n";
    oss << "                 RAPORT Z SYMULACJI SYSTEMU OBSLUGI ZGLOSZEN                    \n";
    oss << "                      Dziedzina: Serwis Informatyczny                           \n";
    oss << "================================================================================\n\n";

    oss << "STRATEGIA: " << s.getNazwaAlgorytmu() << "\n";
    oss << "Horyzont czasowy symulacji: " << s.getHoryzontCzasowy() << " minut\n\n";

    oss << "--- BILANS ZGLOSZEN ---\n";
    oss << "Laczna liczba zgloszen w systemie: " << s.getLiczbaCalkowita() << "\n";
    oss << "Obsluzone zgloszenia (sukces):     " << s.getLiczbaObsluzonych() << "\n";
    oss << "Zgloszenia w toku (na koniec):     " << s.getLiczbaWToku() << "\n";
    oss << "Zgloszenia nieobsluzone (kolejka): " << s.getLiczbaNieobsluzonych() << "\n\n";

    oss << "--- STATYSTYKI CZASU OCZEKIWANIA (W MINUTACH) ---\n";
    oss << fixed << setprecision(2);
    oss << "Sredni czas oczekiwania (mu):      " << s.getSredniCzasOczekiwania() << " min\n";
    oss << "Odchylenie standardowe (sigma):    " << s.getOdchylenieStdOczekiwania() << " min\n";
    oss << "Minimalny czas oczekiwania:        " << s.getMinCzasOczekiwania() << " min\n";
    oss << "Maksymalny czas oczekiwania:       " << s.getMaxCzasOczekiwania() << " min\n";
    oss << "Mediana (p50):                     " << s.getP50Oczekiwania() << " min\n";
    oss << "Percentyl 90 (p90):                " << s.getP90Oczekiwania() << " min\n";
    oss << "Percentyl 95 (p95):                " << s.getP95Oczekiwania() << " min\n";
    oss << "Percentyl 99 (p99):                " << s.getP99Oczekiwania() << " min\n\n";

    oss << "--- STATYSTYKI TERMINOWOSCI I SLA ---\n";
    oss << "Sredni czas realizacji uslugi:     " << s.getSredniCzasRealizacji() << " min\n";
    oss << "Obsluzone w terminie (SLA OK):     " << s.getLiczbaWTerminie() << "\n";
    oss << "Obsluzone po terminie (SLA FAIL):  " << s.getLiczbaSpoznionych() << "\n";
    oss << "Wskaznik dotrzymania SLA:          " << s.getProcentSLA() << " %\n";
    oss << "Srednie spoznienie spoznionych:    " << s.getSrednieSpoznienie() << " min\n";
    oss << "Maksymalne spoznienie:             " << s.getMaxSpoznienie() << " min\n\n";

    oss << "--- UTYLIZACJA ZESPOLU SERWISOWEGO ---\n";
    oss << "Srednie wykorzystanie pracownikow: " << s.getSrednieWykorzystaniePracownikow() << " %\n\n";
    oss << left << setw(6) << "ID"
        << setw(28) << "Specjalizacja"
        << setw(14) << "Obsluzonych"
        << setw(16) << "Czas pracy"
        << setw(16) << "Wykorzystanie" << "\n";
    oss << string(80, '-') << "\n";

    for (const auto& p : s.getStatystykiPracownikow()) {
        oss << left << setw(6) << p.id
            << setw(28) << p.specjalizacja
            << setw(14) << p.liczbaObsluzonych
            << setw(16) << (to_string(p.lacznyCzasPracy) + " min")
            << setw(16) << (to_string(static_cast<int>(p.stopienWykorzystania)) + " %")
            << "\n";
    }
    oss << "\n";

    return oss.str();
}

bool Output::zapiszRaportDoPliku(const string& sciezka, const Statystyki& statystyki) {
    ofstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Zapisu] Nie mozna otworzyc pliku do zapisu: " << sciezka << endl;
        return false;
    }
    plik << generujRaportTekstowy(statystyki);
    plik.close();
    return true;
}

bool Output::zapiszPorownanieDoPliku(const string& sciezka, const vector<Statystyki>& listaStatystyk) {
    ofstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Zapisu] Nie mozna otworzyc pliku do zapisu: " << sciezka << endl;
        return false;
    }

    plik << "========================================================================================\n";
    plik << "               ZESTAWIENIE POROWNAWCZE STRATEGII OBSLUGI ZGLOSZEN                       \n";
    plik << "                        Badanie Eksperymentalne P1                                      \n";
    plik << "========================================================================================\n\n";

    for (const auto& s : listaStatystyk) {
        plik << generujRaportTekstowy(s);
        plik << "\n" << string(88, '=') << "\n\n";
    }

    plik.close();
    return true;
}

bool Output::eksportujDoCsv(const string& sciezka, const vector<Statystyki>& listaStatystyk) {
    ofstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "[Blad Zapisu] Nie mozna otworzyc pliku CSV: " << sciezka << endl;
        return false;
    }

    plik << "Strategia;Lacznie;Obsluzone;WToku;Nieobsluzone;SredniCzasOczek;OdchylenieStd;"
         << "MinOczek;MaxOczek;P50;P90;P95;P99;SredniCzasRealizacji;SLA_Procent;SrednieSpoznienie;"
         << "MaxSpoznienie;SredniaUtylizacja\n";

    for (const auto& s : listaStatystyk) {
        plik << s.getNazwaAlgorytmu() << ";"
             << s.getLiczbaCalkowita() << ";"
             << s.getLiczbaObsluzonych() << ";"
             << s.getLiczbaWToku() << ";"
             << s.getLiczbaNieobsluzonych() << ";"
             << s.getSredniCzasOczekiwania() << ";"
             << s.getOdchylenieStdOczekiwania() << ";"
             << s.getMinCzasOczekiwania() << ";"
             << s.getMaxCzasOczekiwania() << ";"
             << s.getP50Oczekiwania() << ";"
             << s.getP90Oczekiwania() << ";"
             << s.getP95Oczekiwania() << ";"
             << s.getP99Oczekiwania() << ";"
             << s.getSredniCzasRealizacji() << ";"
             << s.getProcentSLA() << ";"
             << s.getSrednieSpoznienie() << ";"
             << s.getMaxSpoznienie() << ";"
             << s.getSrednieWykorzystaniePracownikow() << "\n";
    }

    plik.close();
    return true;
}

void Output::wyswietlWKonsoli(const Statystyki& statystyki) {
    cout << generujRaportTekstowy(statystyki);
}

void Output::wyswietlTabelePorownawcza(const vector<Statystyki>& listaStatystyk) {
    cout << "\n========================================================================================================\n";
    cout << "                                  ZBIORCZA TABELA POROWNAWCZA                                          \n";
    cout << "========================================================================================================\n";
    cout << left
         << setw(28) << "Strategia"
         << setw(10) << "Obsluz."
         << setw(12) << "Sr.Oczek"
         << setw(10) << "StdDev"
         << setw(8)  << "p50"
         << setw(8)  << "p90"
         << setw(8)  << "p99"
         << setw(10) << "SLA %"
         << setw(12) << "Utylizacja" << "\n";
    cout << string(104, '-') << "\n";

    for (const auto& s : listaStatystyk) {
        cout << left
             << setw(28) << s.getNazwaAlgorytmu().substr(0, 26)
             << setw(10) << s.getLiczbaObsluzonych()
             << setw(12) << fixed << setprecision(1) << s.getSredniCzasOczekiwania()
             << setw(10) << s.getOdchylenieStdOczekiwania()
             << setw(8)  << s.getP50Oczekiwania()
             << setw(8)  << s.getP90Oczekiwania()
             << setw(8)  << s.getP99Oczekiwania()
             << setw(10) << (to_string(static_cast<int>(s.getProcentSLA())) + "%")
             << setw(12) << (to_string(static_cast<int>(s.getSrednieWykorzystaniePracownikow())) + "%")
             << "\n";
    }
    cout << "========================================================================================================\n\n";
}
