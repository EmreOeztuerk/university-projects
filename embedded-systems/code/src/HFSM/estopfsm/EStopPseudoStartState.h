/**
 * @file EStopPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Start-Zustand der EStop-Sub-State-Machine.
 *
 * Dieser Zustand stellt den formalen Einstiegspunkt
 * in die Sub-State-Machine dar.
 */

#ifndef ESTOPPSEUDOSTARTSTATE_H
#define ESTOPPSEUDOSTARTSTATE_H
#include "estopbasestate.h"

/**
 * @class EStopPseudoStartState
 * @brief Pseudo-Start-Zustand der EStop-FSM.
 *
 * Wird beim Eintritt in die Sub-State-Machine verwendet
 * und leitet typischerweise in den initialen Zustand weiter.
 */
class EStopPseudoStartState : public EStopBaseState {
public:
    /**
     * @brief Kennzeichnet diesen Zustand als Pseudo-Start-State.
     */
    bool isPseudoStartState() override { return true; };

    /**
     * @brief Deep-History-Einstieg wird auf Pseudo-Start umgeleitet.
     */
    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    /**
     * @brief Ausgabe des Zustands (nur zu Debug-Zwecken).
     */
    void showState() override { std::cout << "State: Sub Pseudo Start State" << std::endl; };
};

#endif //ESTOPPSEUDOSTARTSTATE_H