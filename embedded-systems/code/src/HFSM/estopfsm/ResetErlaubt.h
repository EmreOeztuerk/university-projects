/**
 * @file ResetErlaubt.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand der EStop-Sub-State-Machine, in dem ein das Reset-Tastern erlaubt istund darauf gewartet wird .
 *
 * Dieser Zustand wird erreicht, wenn:
 *  - Kein E-Stop aktiv ist
 *  - Das System bereit ist, einen Reset anzunehmen
 *
 * Abhängig vom gedrückten Reset-Taster erfolgt
 * der Übergang in den entsprechenden Reset-Ablauf.
 */

#ifndef RESETERLAUBT_H
#define RESETERLAUBT_H

#include "estopbasestate.h"

/**
 * @class ResetErlaubt
 * @brief Zustand: Reset erlaubt.
 */
class ResetErlaubt : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e1_g() override;

    TriggerProcessingState s_e2_g() override;

    TriggerProcessingState s_rt1_g() override;

    TriggerProcessingState s_rt2_g() override;

    void showState() override;
};

#endif /* RESETERLAUBT_H */