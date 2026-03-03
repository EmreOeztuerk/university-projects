/**
 * Trigger und Aktionen des Zustands "MetallsensorBereich3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "metallsensor_bereich3.h"
#include "../aussortierungsbereich/aussortierungsbereich3.h"

void MetallsensorBereich3::entry() {
    action->enteredMetallsensorBereich3();
    //data->zeitSpeichern(data->t_lsh_lsm, "");
}

void MetallsensorBereich3::exit() {
    data->zeitStarten();
}

TriggerProcessingState MetallsensorBereich3::s_metal_aus() {
    cout << "Service Mode BCKalibrierung MetallsensorBereich3: ms_aus" << endl;

    exitPointDefaultLeaveState();
    new(this) Aussortierungsbereich3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}