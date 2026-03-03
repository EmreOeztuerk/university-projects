/**
 * Trigger und Aktionen der Zustands "BCKalibrierung".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "bc_kalibrierung_basestate.h"

void BCKalibrierung_BaseState::setData(ContextData *data) {
    this->data = data;
}

void BCKalibrierung_BaseState::setAction(Actions *action) {
    this->action = action;
}