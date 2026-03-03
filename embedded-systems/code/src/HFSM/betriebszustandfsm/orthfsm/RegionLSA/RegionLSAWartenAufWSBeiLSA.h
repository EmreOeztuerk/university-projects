/**
 * @file RegionLSAWartenAufWSBeiLSA.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf neues Werkstück bei LSA.
 *
 * In diesem Zustand ist der Einlauf frei und die Anlage wartet darauf, dass die
 * Lichtschranke am Anfang (LSA) unterbrochen wird (neues Werkstück eingelegt).
 */

#ifndef REGIONLSAWARTENAUFWSBEILSA_H
#define REGIONLSAWARTENAUFWSBEILSA_H

#include "RegionLSABaseState.h"

class RegionLSAWartenAufWSBeiLSA : public RegionLSABaseState {
public:
    void entry() override;

    TriggerProcessingState t_toleranzzeit_lsa() override; // -> Fehler (F-Exit)
    TriggerProcessingState s_lsa2_ub() override; // addWS_F2 -> BeginnSortierprozessF2

    void showState() override;
};

#endif //REGIONLSAWARTENAUFWSBEILSA_H