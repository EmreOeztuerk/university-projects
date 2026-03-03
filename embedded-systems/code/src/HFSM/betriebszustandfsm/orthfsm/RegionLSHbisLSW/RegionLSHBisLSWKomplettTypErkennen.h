/**
 * @file RegionLSHBisLSWKomplettTypErkennen.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Kompletten Typ des Werkstücks erkennen.
 *
 * Kombiniert die Höheninformationen mit den Metallsensor-Daten, um den finalen
 * Werkstücktyp zu bestimmen.
 */

#ifndef REGIONLSHBISLSWKOMPLETTTYPERKENNEN_H
#define REGIONLSHBISLSWKOMPLETTTYPERKENNEN_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWKomplettTypErkennen : public RegionLSHBisLSWBaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsw_ub() override;

    void showState() override;
};

#endif //REGIONLSHBISLSWKOMPLETTTYPERKENNEN_H