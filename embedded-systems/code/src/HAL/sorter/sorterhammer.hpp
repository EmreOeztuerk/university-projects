/**
 * @file sorterhammer.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse SortiererAuswerfer.
 *
 * Spezifische Implementierung eines Sortierers, der einen Auswerfer (Hammer/Stempel) verwendet.
 */

#ifndef SORTERHAMMER_HPP
#define SORTERHAMMER_HPP

#include "sorter.hpp"

class SortiererAuswerfer : public Sortierer {
private:
    SortTyp mechanismus;
    bool sortiererStatus;

public:
    SortiererAuswerfer();

    bool getSortiererStatus();

    void durchlassen();

    void aussortieren();
};

#endif