/**
 * @file RegionLSWBisLSEWartenAufErlaubnisVonF2.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Übergabeerlaubnis von Anlage 2.
 *
 * (Nur relevant für Anlage 1) Das Werkstück nähert sich dem Ende von Anlage 1.
 * Es wird gewartet, bis Anlage 2 signalisiert, dass sie bereit ist ("Rutsche frei" / "Einlauf frei"),
 * das Werkstück zu übernehmen.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFERLAUBNISVONF2_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFERLAUBNISVONF2_H

#include "RegionLSWBisLSEBaseState.h"

class RegionLSWBisLSEWartenAufErlaubnisVonF2 : public RegionLSWBisLSEBaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_festo2_bereit() override; // -> WartenAufLSE

    void showState() override;
};
#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFERLAUBNISVONF2_H