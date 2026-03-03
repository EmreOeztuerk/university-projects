/**
 * @file RegionLSHBisLSWWartenAufLSW.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten bis LSW wieder frei ist.
 *
 * Das Werkstück passiert die Weiche/Sortierer-Lichtschranke.
 * Sobald sie wieder frei ist, ist der Vorgang für diese Region abgeschlossen.
 */

#ifndef REGIONLSHBISLSWWARTENAUFLSW_H
#define REGIONLSHBISLSWWARTENAUFLSW_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWWartenAufLSW : public RegionLSHBisLSWBaseState {
public:
    void entry() override;

    TriggerProcessingState t_lsh_lsw() override;

    //TriggerProcessingState t_lsh_ms() override;  // -> WartenAufWS_BeiLSW
    TriggerProcessingState s_lsw_ub() override;

    TriggerProcessingState s_metal_an() override;

    void showState() override;
};
#endif //REGIONLSHBISLSWWARTENAUFLSW_H