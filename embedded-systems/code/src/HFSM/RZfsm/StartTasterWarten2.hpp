/**
 * @file StartTasterWarten2.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand zum Warten auf den Start-Taster (Schritt 2).
 *
 * Teil der Sequenz für Anlage 2.
 */

#ifndef STARTTASTERWARTEN2_HPP
#define STARTTASTERWARTEN2_HPP


#include "RZBaseState.hpp"


class StartTasterWarten2 : public RZBaseState {
    void entry() override;

    virtual TriggerProcessingState s_st2_lg() override;

    void showState() override;
};


#endif //STARTTASTERWARTEN2_HPP