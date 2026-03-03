/**
 * @file RegionLSHBisLSWWSDurchlauf.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Werkstück durchlassen.
 *
 * Das Werkstück ist gut und soll weitertransportiert werden. Die Weiche bleibt geschlossen.
 */

#ifndef REGIONLSHBISLSWWSDURCHLAUF_H
#define REGIONLSHBISLSWWSDURCHLAUF_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWWSDurchlauf : public RegionLSHBisLSWBaseState {
public:
    void entry() override;

    void showState() override;
};
#endif //REGIONLSHBISLSWWSDURCHLAUF_H