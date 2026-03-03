/**
 * @file FehlerbehandlungPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Startzustand der Fehlerbehandlung.
 *
 * Initialisiert die Fehlerbehandlung und wechselt in den Zustand `AnstehendUnquittiert`.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGPSEUDOSTARTSTATE_H

#include "FehlerbehandlungBaseState.h"

class FehlerbehandlungPseudoStartState : public FehlerbehandlungBaseState {
public:
    bool isPseudoStartState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "[Fehlerbehandlung] Pseudo Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGPSEUDOSTARTSTATE_H