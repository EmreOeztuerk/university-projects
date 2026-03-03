/**
* @file ResetTasterWarten1.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Reset (Schritt 1).
 *
 * Wird angesprungen, wenn ein Fehler vorlag oder ein Reset erforderlich ist,
 * bevor die Anlage wieder startklar ist.
 */

#ifndef RESETTASTERWARTEN1_HPP
#define RESETTASTERWARTEN1_HPP


#include "RZBaseState.hpp"
#include "../subcommon/TriggerProcessingState.h"
#include "Betriebsbereit.hpp"

class ResetTasterWarten1 : public RZBaseState {
public:
    void entry() override;

    virtual TriggerProcessingState s_rt1_g() override;

    void showState() override;
};


#endif //RESETTASTERWARTEN1_HPP