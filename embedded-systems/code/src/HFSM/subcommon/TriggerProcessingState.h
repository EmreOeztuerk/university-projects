/**
* @file TriggerProcessingState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Interface für Trigger-verarbeitende Zustände.
 *
 * Eine Basisklasse oder ein Interface für Zustände, die auf spezifische Trigger
 * reagieren müssen, die nicht über den Standard-Nachrichtenweg kommen.
 */

#ifndef TRIGGERPROCESSINGSTATE_H
#define TRIGGERPROCESSINGSTATE_H

enum class TriggerProcessingState {
    consumed,
    pending,
    endstatereached,
    expliciteexitfehlerbehandlung,
    explicitExitViaB,
    explicitExitViaS,
    explicitExitViaE1,
    explicitExitViaE2,
    explicitExitViaR
};
#endif // TRIGGERPROCESSINGSTATE_H