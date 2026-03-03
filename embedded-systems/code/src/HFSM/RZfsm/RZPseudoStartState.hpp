/**
 * @file RZPseudoStartState.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Startzustand für die Ruhezustands-FSM.
 *
 * Initialisiert die Sub-FSM und wechselt sofort in den ersten echten Zustand (Wartezustand).
 */

#ifndef RZPSEUDOSTARTSTATE_HPP
#define RZPSEUDOSTARTSTATE_HPP

#include "RZBaseState.hpp"

class RZPseudoStartState : public RZBaseState {
public:
    bool isPseudoStartState() override { return true; };

    void showState() override { std::cout << "Ruhezustand: Sub Pseudo Start State" << std::endl; };
};

#endif /* RZPSEUDOSTARTSTATE_HPP_ */