/**
 * @file AnstehendQuittiert.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Fehler steht an, wurde aber quittiert.
 *
 * Der Bediener hat den Fehler bestätigt, aber die Ursache
 * besteht weiterhin. Sobald die Ursache behoben ist, gilt der Fehler als "Gelöst".
 */

#ifndef ESEP_2025WISE_TEAM_1_3_ANSTEHENDQUITTIERT_H
#define ESEP_2025WISE_TEAM_1_3_ANSTEHENDQUITTIERT_H

#include "FehlerbehandlungBaseState.h"
#include "../ContextData.h"

class AnstehendQuittiert : public FehlerbehandlungBaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_st_g() override;

    void showState() override;
};


#endif //ESEP_2025WISE_TEAM_1_3_ANSTEHENDQUITTIERT_H