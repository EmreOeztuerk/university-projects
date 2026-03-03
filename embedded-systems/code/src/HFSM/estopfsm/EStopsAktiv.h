/**
 * @file EStopsAktiv.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Beide E-Stops sind aktiv.
 *
 * Sowohl an Anlage 1 als auch an Anlage 2 sind die Not-Aus-Taster gedrückt.
 * Es müssen beide entriegelt werden, bevor ein Reset möglich ist.
 */

#ifndef ESTOPSAKTIV_H
#define ESTOPSAKTIV_H


#include "EstopBaseState.h"

/**
 * @class EStopsAktiv
 * @brief Zustand: E-Stop 1 und E-Stop 2 aktiv.
 *
 * Behandelt alle Trigger, die relevant sind,
 * während der E-Stop 1 und E-Stop 2 aktiv sind.
 */
class EStopsAktiv : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e1_ng() override;

    TriggerProcessingState s_e2_ng() override;

    void showState() override;
};

#endif /* ESTOPSAKTIV_H */