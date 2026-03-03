/**
 * @file FehlerbehandlungPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Endzustand der Fehlerbehandlung.
 *
 * Der Fehler wurde behoben und quittiert. Rückkehr zum Normalbetrieb.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGENDZUSTAND_H
#define ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGENDZUSTAND_H

#include "FehlerbehandlungBaseState.h"

class FehlerbehandlungPseudoEndState : public FehlerbehandlungBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "[Fehlerbehandlung] Endzustand" << std::endl; }; // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGENDZUSTAND_H