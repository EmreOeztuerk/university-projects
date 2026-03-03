/**
 * @file GegangenUnquittiert.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Fehler ist weg, aber noch nicht quittiert.
 *
 * Die Fehlerursache hat sich von selbst erledigt,
 * aber der Bediener muss dies noch bestätigen, bevor die Anlage weiterläuft.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_GEGANGENUNQUITTIERT_H
#define ESEP_2025WISE_TEAM_1_3_GEGANGENUNQUITTIERT_H

#include "FehlerbehandlungBaseState.h"

class GegangenUnquittiert : public FehlerbehandlungBaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_rt1_g() override;

    void showState() override;
};


#endif //ESEP_2025WISE_TEAM_1_3_GEGANGENUNQUITTIERT_H