/**
 * Trigger und Aktionen des Zustands "MetallsensorBereich4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "metallsensor_bereich4.h"
#include "../aussortierungsbereich/aussortierungsbereich4.h"

void MetallsensorBereich4::entry() {
    action->enteredMetallsensorBereich4();
    //data->zeitSpeichern(data->t_lsh_lsm, "");
}

TriggerProcessingState MetallsensorBereich4::s_metal_aus() {
    cout << "Service Mode BCKalibrierung MetallsensorBereich4: ms_aus" << endl;

    exitPointDefaultLeaveState();
    new(this) Aussortierungsbereich4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}