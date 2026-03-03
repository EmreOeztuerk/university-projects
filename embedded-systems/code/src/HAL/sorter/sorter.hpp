/**
 * @file sorter.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Basisklasse für Sortiermechanismen.
 *
 * Definiert die abstrakte Schnittstelle für verschiedene Sortierer (z.B. Weiche oder Auswerfer).
 */

#ifndef SORTER_HPP
#define SORTER_HPP

#include "../../gpio_address.hpp"
#include "action.hpp"
#include "sorttype.hpp"

#define MECHANISMUS_BETAETIGEN 1 << 19

class Sortierer {
private:
    SortTyp mechanismus;
    bool sortiererStatus;

public:
    void durchlassen();

    void aussortieren();

    bool getSortiererStatus();

    void setSortTyp(SortTyp givenSortTyp);
};

#endif