/**
 * @file RegionLSABeginnsortierprozessF2.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Beginn Sortierprozess auf Anlage 2.
 *
 * Spezifische Logik für Anlage 2, wenn ein Werkstück am Einlauf erkannt wird.
 * Unterscheidet sich von Anlage 1, da Werkstücke hier von Anlage 1 übergeben werden.
 */

#ifndef REGIONLSABEGINNSORTIERPROZESSF2_H
#define REGIONLSABEGINNSORTIERPROZESSF2_H

#include "RegionLSABaseState.h"

class RegionLSABeginnSortierprozessF2 : public RegionLSABaseState {
public:
    void entry() override;

    TriggerProcessingState t_lse1_lsa2() override;

    TriggerProcessingState s_lsa2_ub() override;

    void showState() override;
};

#endif //REGIONLSABEGINNSORTIERPROZESSF2_H