# PRZEWODNIK DO OBRONY USTNEJ PROJEKTU (KRYTERIUM 5.0)

Niniejszy dokument to kompletna ściągawka przygotowana na ustną obronę projektu przed prowadzącym zajęcia. Zawiera opis architektury, instrukcję demonstracji oraz gotowe, eksperckie odpowiedzi na potencjalne pytania egzaminacyjne.

---

## 1. Jak zaprezentować program prowadzącemu (Krok po Kroku)

### Krok 1: Pokazanie czystej kompilacji
W terminalu (PowerShell lub CMD) w katalogu projektu:
```powershell
.\build.bat
```
*Komentarz:* Kod kompiluje się w standardzie **C++20** z włączonymi wszystkimi rygorystycznymi flagami ostrzeżeń (`-Wall -Wextra -pedantic`) oraz optymalizacją `-O3`. Tworzy w pełni samodzielny plik wykonywalny `symulator.exe`.

### Krok 2: Uruchomienie porównania 4 strategii
```powershell
.\symulator.exe --porownaj
```
*Komentarz:* Program natychmiast odczytuje konfigurację, wczytuje zgłoszenia, wykonuje symulację w czasie dyskretnym dla 4 strategii (FIFO, HPF, EDF, Aging) na identycznych danych wejściowych, generuje czytelną tabelę zbiorczą oraz zapisuje wyniki do `wyniki.txt` i `wyniki_porownanie.csv`.

### Krok 3: Demonstracja kompleksowych badań eksperymentalnych (wymóg 5.0)
```powershell
.\symulator.exe --eksperymenty
```
*Komentarz:* Wykonuje 5 scenariuszy badawczych (w tym wielką skalę $N = 10\,000$ zgłoszeń), zbiera statystyki opisowe, percentyle i SLA, demonstrując odporność i wydajność silnika C++ (mniej niż 3 sekundy na cały pakiet testów!).

---

## 2. Gotowe Odpowiedzi na Pytania Prowadzącego

### Pytanie 1: Dlaczego użyliście wzorca Strategia i inteligentnych wskaźników?
**Odpowiedź:**
> "Zastosowaliśmy wzorzec projektowy **Strategia** (`IAlgorytm`), aby oddzielić silnik symulacji (`SilnikSymulacji`) od konkretnego algorytmu kolejkowania i przydziału zgłoszeń. Silnik operuje na `std::unique_ptr<IAlgorytm>`, co jest realizacją zasady Otwarte-Zamknięte (OCP) – możemy dodać dowolną nową strategię bez ingerencji w pętlę symulacji.  
> Obiekty zgłoszeń są zarządzane przez `std::shared_ptr<Zgloszenie>`, ponieważ w trakcie symulacji referencja do zgłoszenia jest współdzielona między kolejką algorytmu, pracownikiem wykonującym zlecenie oraz modułem statystyk rejestrującym historię zdarzeń. Zapewnia to idiom **RAII** i stuprocentowe bezpieczeństwo pamięci bez wycieków."

---

### Pytanie 2: Jak działają komparatory w `std::priority_queue` dla HPF i EDF?
**Odpowiedź:**
> "`std::priority_queue` w C++ domyślnie tworzy max-heap przy użyciu funktora `std::less`. Oznacza to, że metoda `operator()(a, b)` zwraca `true`, gdy element `a` ma mniejszy priorytet niż `b` (wtedy `b` wędruje na szczyt kopca).  
> - Dla **HPF (Strategia B)**: jeśli `a->priorytet < b->priorytet`, zwracamy `true`. Gdy priorytety są równe, stosujemy stabilne rozstrzyganie remisów: `a->czasWejscia > b->czasWejscia` (wcześniejsze wejście ma pierwszeństwo).  
> - Dla **EDF (Strategia C)**: chcemy obsłużyć zgłoszenie o najkrótszym terminie (min-heap dla terminu), dlatego zwracamy `true`, gdy `a->termin > b->termin`.  
> Obie operacje wstawiania (`push`) i pobierania szczytu (`pop`) działają w czasie logarytmicznym $\mathcal{O}(\log N)$."

---

### Pytanie 3: Na czym polega problem głodzenia (Starvation) i jak go rozwiązaliście?
**Odpowiedź:**
> "W strategii czysto priorytetowej (HPF) zgłoszenia o niskim priorytecie (np. proste czyszczenie laptopa, priorytet 1) mogą nigdy nie doczekać się obsługi, jeśli do serwisu nieustannie napływają zadania o wyższych priorytetach (np. naprawy płyt głównych po zalaniu, priorytet 3 i 4). Jest to klasyczne **zjawisko głodzenia (starvation)**.  
> Rozwiązaliśmy ten problem implementując **czwartą strategię z mechanizmem dynamicznego starzenia (Aging)**:
> $$P_{eff}(t) = P_{bazowy} + \left\lfloor \frac{t - czasWejscia}{\Delta t_{aging}} \right\rfloor$$
> Co 15 minut oczekiwania w kolejce efektywny priorytet zgłoszenia rośnie o 1. Zadanie z priorytetem 1 po 45 minutach oczekiwania osiąga priorytet 4, dzięki czemu wyprzedza świeżo nadesłane zadania i ma matematyczną gwarancję obsługi (własność starvation-free)."

---

### Pytanie 4: Dlaczego w warunkach silnego przeciążenia EDF ma niższy wskaźnik SLA niż FIFO?
**Odpowiedź:**
> "Jest to znane w teorii szeregowania zjawisko **efektu domina (domino effect)**. EDF jest algorytmem optymalnym wyłącznie w warunkach braku przeciążenia (twierdzenie Dertouzosa). Gdy system jest przeciążony ($\lambda > \mu$), EDF usilnie przydziela pracownikom zadania o najwcześniejszym deadline, których i tak często nie udaje się ukończyć przed terminem. Pracownik poświęca cenny czas na zadanie spóźnione, przez co kolejne zadania również stają się spóźnione. W takich warunkach FIFO lub HPF okazują się bardziej stabilne."

---

### Pytanie 5: Jak zapewniliście 100% powtarzalność badań?
**Odpowiedź:**
> "Po pierwsze: generator liczb losowych `GeneratorZgloszen` korzysta z silnika `std::mt19937_64` o stałym ziarnie `seed`, co daje identyczny, deterministyczny strumień liczb losowych.  
> Po drugie: w `SilnikSymulacji`, przed uruchomieniem każdej kolejnej strategii, tworzymy świeże, głębokie kopie obiektów `Zgloszenie` z szablonu bazowego oraz resetujemy stan pracowników (`zresetuj()`). Dzięki temu żadna strategia nie dziedziczy zmodyfikowanego stanu po poprzednim algorytmie."

---

### Pytanie 6: Jaka jest złożoność obliczeniowa i pamięciowa całego symulatora?
**Odpowiedź:**
> "- **Pamięciowa:** $\mathcal{O}(N + K)$, gdzie $N$ to liczba zgłoszeń, a $K$ to liczba wykonawców. Pamiętamy obiekty w wektorach i kolejkach STL.  
> - **Czasowa:** dla FIFO, HPF i EDF wynosi $\mathcal{O}(N \log N + T_{max} \cdot K)$. $N \log N$ wynika z sortowania nadejść i operacji na kopcu kolejki priorytetowej, natomiast $T_{max} \cdot K$ to sprawdzanie zwolnienia wykonawców w każdym z $T_{max}$ dyskretnych kroków czasowych.  
> Dzięki temu symulacja $10\,000$ zgłoszeń wykonuje się w C++ w czasie poniżej 1.5 sekundy."

---

### Pytanie 7: Dlaczego w `KatalogUslug` użyliście `std::optional` zamiast zwracania wskaźnika?
**Odpowiedź:**
> "`std::optional<Usluga>` (wprowadzone w C++17) w sposób bezpośredni i bezpieczny wyraża fakt, że usługa o podanym ID może nie istnieć w katalogu. Zmusza to programistę do sprawdzenia obecności wartości (`if (opt)`) i eliminuje ryzyko dereferencji wiszącego wskaźnika lub wskaźnika `nullptr`."

---

### Pytanie 8: Jak wyliczane są percentyle (p50, p90, p95, p99)?
**Odpowiedź:**
> "W module `Statystyki` czasy oczekiwania wszystkich obsłużonych zgłoszeń są zapisywane w wektorze i sortowane $\mathcal{O}(N \log N)$. Percentyl obliczany jest ze standardową interpolacją liniową (odpowiednik metody `linear` w NumPy): wyliczamy pozycję $k = \frac{P}{100} \cdot (N - 1)$, bierzemy indeks dolny i górny oraz ważoną średnią z wartości sąsiednich elementów. Gwarantuje to wysoką precyzję statystyczną bez błędów zaokrągleń."
