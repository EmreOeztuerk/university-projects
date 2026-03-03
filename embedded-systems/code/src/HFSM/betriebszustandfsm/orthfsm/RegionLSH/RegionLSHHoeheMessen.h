/**
 * @file RegionLSHHoeheMessen.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Höhenmessung aktiv.
 *
 * Das Werkstück befindet sich unter dem Höhenmesser. Die analogen Werte werden erfasst,
 * um den Werkstücktyp (Flach, Hoch, Bohrung) zu bestimmen.
 */

#ifndef REGIONLSHHOEHEMESSEN_H
#define REGIONLSHHOEHEMESSEN_H

#include "RegionLSHBaseState.h"

class RegionLSHHoeheMessen : public RegionLSHBaseState {
public:
    void entry() override;

    //void exit() override;

    TriggerProcessingState s_lsh_nub() override; // zurück nach WartenAufLSH

    void showState() override;
};
#endif //REGIONLSHHOEHEMESSEN_H