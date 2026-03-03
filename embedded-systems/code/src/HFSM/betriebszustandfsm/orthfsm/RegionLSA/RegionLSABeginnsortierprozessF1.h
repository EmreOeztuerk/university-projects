/**
 * @file RegionLSABeginnsortierprozessF1.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Beginn Sortierprozess auf Anlage 1.
 *
 * Ein neues Werkstück wurde auf Anlage 1 erkannt und registriert.
 * Es wird geprüft, ob der Bereich für das nächste Werkstück frei ist.
 */

#ifndef REGIONLSABEGINNSORTIERPROZESSF1_H
#define REGIONLSABEGINNSORTIERPROZESSF1_H

#include "RegionLSABaseState.h"

class RegionLSABeginnSortierprozessF1 : public RegionLSABaseState {
public:
    void entry() override;

    TriggerProcessingState t_lsa_q1() override; // LSA_Q1 -> NaechsteWS_Erlaubt
    TriggerProcessingState t_lsa_lsh() override; // LSA_Q1 -> NaechsteWS_Erlaubt
    TriggerProcessingState s_lsa1_ub() override; // neuWS -> F

    void showState() override;
};

#endif //REGIONLSABEGINNSORTIERPROZESSF1_H