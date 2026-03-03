/**
 * @file RegionLSAWartenAufLSA.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Werkstück passiert LSA.
 *
 * Das Werkstück unterbricht gerade die Lichtschranke LSA.
 * Es wird gewartet, bis die Lichtschranke wieder frei ist, um das Werkstück korrekt zu erfassen.
 */

#ifndef REGIONLSAWARTENAUFLSA_H
#define REGIONLSAWARTENAUFLSA_H

#include "RegionLSABaseState.h"

class RegionLSAWartenAufLSA : public RegionLSABaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa1_ub() override; // [Festo1] -> F1
    TriggerProcessingState s_lse1_ub() override; // [Festo2] && [F2voll] -> F2
    TriggerProcessingState s_lsa2_ub() override; // [Festo2] -> Fehler
    void showState() override;
};

#endif //REGIONLSAWARTENAUFLSA_H