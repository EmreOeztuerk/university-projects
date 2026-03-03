/**
 * @file StartTasterWarten1.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand zum Warten auf den Start-Taster (Schritt 1).
 *
 * Teil der Einschaltsequenz. Wartet auf Betätigung des Start-Tasters an Anlage 1.
 */

#ifndef STARTTASTERWARTEN1_HPP
#define STARTTASTERWARTEN1_HPP

#include <iostream>
#include "RZBaseState.hpp"
#include "../subcommon/TriggerProcessingState.h"
#include "Betriebsbereit.hpp"

class StartTasterWarten1 : public RZBaseState {
    void entry() override;

    virtual TriggerProcessingState s_st1_lg() override;

    void showState() override;
};


#endif //STARTTASTERWARTEN1_HPP