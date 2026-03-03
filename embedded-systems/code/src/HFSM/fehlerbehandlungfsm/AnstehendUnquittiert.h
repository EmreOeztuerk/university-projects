/**
 * @file AnstehendUnquittiert.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Fehler steht an und ist unquittiert.
 *
 * Ein neuer Fehler ist aufgetreten. Die Anlage stoppt oder warnt, und der Bediener
 * muss den Fehler zunächst zur Kenntnis nehmen (Quittieren).
 */

#ifndef ESEP_2025WISE_TEAM_1_3_ANSTEHENDUNQUITTIERT_H
#define ESEP_2025WISE_TEAM_1_3_ANSTEHENDUNQUITTIERT_H

#include "FehlerbehandlungBaseState.h"

class AnstehendUnquittiert : public FehlerbehandlungBaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_rt1_g() override;

    TriggerProcessingState s_lsr_nub() override;

    TriggerProcessingState s_lsa2_ub() override;


    void showState() override;
};

#endif //ESEP_2025WISE_TEAM_1_3_ANSTEHENDUNQUITTIERT_H