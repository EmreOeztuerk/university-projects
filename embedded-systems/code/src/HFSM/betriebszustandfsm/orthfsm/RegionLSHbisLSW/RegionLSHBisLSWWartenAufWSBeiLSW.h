/**
 * @file RegionLSHBisLSWWartenAufWSBeiLSW.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Ankunft bei der Weiche (LSW).
 *
 * Das Werkstück ist auf dem Weg zur Weiche. Es wird gewartet, bis die Lichtschranke
 * vor der Weiche (LSW) ausgelöst wird.
 */

#ifndef REGIONLSHBISLSWWARTENAUFWSBEILSW_H
#define REGIONLSHBISLSWWARTENAUFWSBEILSW_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWWartenAufWSBeiLSW : public RegionLSHBisLSWBaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsw_ub() override;

    TriggerProcessingState s_metal_an() override;

    TriggerProcessingState t_toleranzzeit_lsw() override;

    void showState() override;
};

#endif //REGIONLSHBISLSWWARTENAUFWSBEILSW_H