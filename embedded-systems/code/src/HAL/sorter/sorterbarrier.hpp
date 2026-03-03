/**
 * @file sorterbarrier.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse SortiererWeiche.
 *
 * Spezifische Implementierung eines Sortierers als Weiche. Enthält Mechanismen
 * zum Öffnen und Schließen, teilweise mit Zeitverzögerung.
 */

#ifndef SORTERBARRIER_HPP
#define SORTERBARRIER_HPP

#include "sorter.hpp"

#define MAX_ZEIT_OFFEN_MS 10 * 100000	//4 sec.


class SortiererWeiche : public Sortierer {
private:
    SortTyp mechanismus;
    bool sortiererStatus;

public:
    SortiererWeiche();

    bool getSortiererStatus();

    useconds_t getMaxZeitOffen();

    void aussortieren();

    void durchlassen();

    void weicheAuf();

    void weicheZu();
};

#endif