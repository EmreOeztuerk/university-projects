/**
* @file Types.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Allgemeine Datentypen und Enums für das System.
 *
 * Diese Datei definiert grundlegende Typen, die projektweit
 * verwendet werden, insbesondere zur Klassifizierung von Werkstücken, Fehlern und der Anlage.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_TYPES_H
#define ESEP_2025WISE_TEAM_1_3_TYPES_H

// Welche Anlage ist betroffen?
enum class AnlageId {
    UNBEKANNT,
    FESTO1,
    FESTO2,
    BEIDE
};

// Wo ist es passiert?
enum class FehlerOrt {
    ANDEREMACHINE,
    DEFAULT,
    UNBEKANNT,
    LSA,
    LSA1,
    LSA2,
    LSH,
    LSH_BIS_LSW,
    LSW_BIS_LSE,
    LSW,
    LSE,
    LSE1,
    LSE2,
    LSR
};

// Was ist passiert?
enum class Fehlerart {
    keinFehler,
    unbekannt,
    neuWS,
    verlorenWS,
    rutscheVoll,
    abstandZuKlein,
    verbindungUnterbrochen,
    externerFehler
};

// Werkstück-Typ (für typ/naechsteTyp Guards)
enum class WerkstueckTyp {
    UNBEKANNT,
    HB, //Hoch mit Bohrung
    H, //Hoch ohne Bohrung
    F, //Flach
    HBM, //Hoch mit Bohrung und Metall
};

#endif //ESEP_2025WISE_TEAM_1_3_TYPES_H