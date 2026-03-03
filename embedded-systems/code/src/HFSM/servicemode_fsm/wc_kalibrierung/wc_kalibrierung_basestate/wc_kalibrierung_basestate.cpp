/**
 * Trigger und Aktionen des Zustands "WCKalibrierung".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "wc_kalibrierung_basestate.h"

void WCKalibrierung_BaseState::setData(ContextData *data) {
    this->data = data;
}

void WCKalibrierung_BaseState::setAction(Actions *action) {
    this->action = action;
}