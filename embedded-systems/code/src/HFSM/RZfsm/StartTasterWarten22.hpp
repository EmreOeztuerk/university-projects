/**
* @file StartTasterWarten22.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zwischenzustand beim Warten auf Start (Anlage 2).
 */

#ifndef STARTTASTERWARTEN22_HPP
#define STARTTASTERWARTEN22_HPP

#include "RZBaseState.hpp"


class StartTasterWarten22 : public RZBaseState {
    void entry() override;

    virtual TriggerProcessingState s_st2_g() override;

    void showState() override;
};


#endif //STARTTASTERWARTEN22_HPP