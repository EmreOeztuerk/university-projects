/**
 * @file signal.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition aller Signal-Codes und Pulse-Nachrichten.
 *
 * Enthält das Enum `Signal`, das alle im System verwendeten Nachrichten-Codes definiert
 * (Sensoren, Aktoren, Fehler, Inter-Maschinen-Kommunikation, etc.).
 */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <cstdint>
#include <sys/neutrino.h>


using MessageData = int;

enum Signal : int8_t {
    // --------------------------------------------------------
    // SENSOR
    // --------------------------------------------------------

    // Lichtschranken
    S_LSA1_UB = _PULSE_CODE_MINAVAIL + 1, // Lichtschranke Anfang Festo1
    S_LSA1_NUB = _PULSE_CODE_MINAVAIL + 2,

    S_LSH_UB = _PULSE_CODE_MINAVAIL + 3, // Lichtschranke Höhensensor
    S_LSH_NUB = _PULSE_CODE_MINAVAIL + 4,

    S_LSW_UB = _PULSE_CODE_MINAVAIL + 5, // Lichtschranke Weiche
    S_LSW_NUB = _PULSE_CODE_MINAVAIL + 6,

    S_LSR1_UB = _PULSE_CODE_MINAVAIL + 7, // Lichtschranke Rutsche Festo 1
    S_LSR1_NUB = _PULSE_CODE_MINAVAIL + 8,

    S_LSE1_UB = _PULSE_CODE_MINAVAIL + 9, // Lichtschranke Ende Fetso 1
    S_LSE1_NUB = _PULSE_CODE_MINAVAIL + 10,

    S_LSA2_UB = _PULSE_CODE_MINAVAIL + 11, // Lichtschranke Anfang Festo 2
    S_LSA2_NUB = _PULSE_CODE_MINAVAIL + 12,

    S_LSE2_UB = _PULSE_CODE_MINAVAIL + 13, // Lichtschranke Ende Fetso 2
    S_LSE2_NUB = _PULSE_CODE_MINAVAIL + 14,

    S_LSR2_UB = _PULSE_CODE_MINAVAIL + 15, // Lichtschranke Rutsche Festo 2
    S_LSR2_NUB = _PULSE_CODE_MINAVAIL + 16,

    // Metallsensor
    S_METAL_AN = _PULSE_CODE_MINAVAIL + 17, // Es ist Metall
    S_METAL_AUS = _PULSE_CODE_MINAVAIL + 18, // Es ist nicht Metall

    // Hoehensensor
    S_HS_WERT = _PULSE_CODE_MINAVAIL + 93, // Wert des Hoehensensor


    S_UNKNOWN = _PULSE_CODE_MINAVAIL + 88,

    // --------------------------------------------------------
    // AKTORIK
    // --------------------------------------------------------

    // Taster
    S_ST_G = _PULSE_CODE_MINAVAIL + 20, // Start-Taster
    S_ST1_G = _PULSE_CODE_MINAVAIL + 21,
    S_ST2_G = _PULSE_CODE_MINAVAIL + 22,
    S_ST1_LG = _PULSE_CODE_MINAVAIL + 23, // Lang gedrueckt Festo 1
    S_ST2_LG = _PULSE_CODE_MINAVAIL + 24, // Lang gedrueckt Festo 2

    S_STP_G = _PULSE_CODE_MINAVAIL + 25, // Stop-Taster

    S_RT1_G = _PULSE_CODE_MINAVAIL + 26, // Reset-Taster Festo 1
    S_RT2_G = _PULSE_CODE_MINAVAIL + 27, // Festo 2

    //  (E-Stop)
    S_E1_G = _PULSE_CODE_MINAVAIL + 28, // E-Stop Festo 1
    S_E1_NG = _PULSE_CODE_MINAVAIL + 29, // Nicht gedruekt
    S_E2_G = _PULSE_CODE_MINAVAIL + 30, // E-Stop Festo 2
    S_E2_NG = _PULSE_CODE_MINAVAIL + 31, // Nicht gedrueckt

    // --------------------------------------------------------
    // VERBINDUNGSUEBERWACHUNG
    // --------------------------------------------------------

    S_VBDN = _PULSE_CODE_MINAVAIL + 32, // Festo 1 und 2 Verbunden
    S_NVBDN = _PULSE_CODE_MINAVAIL + 33, // Festo 1 und 2 nicht verbunden

    // --------------------------------------------------------
    // LOGIK CMD
    // --------------------------------------------------------

    // Ampel
    CMD_LED_AMP_ROT_AN = _PULSE_CODE_MINAVAIL + 34,
    CMD_LED_AMP_ROT_AUS = _PULSE_CODE_MINAVAIL + 35,
    CMD_LED_AMP_ROT_BS = _PULSE_CODE_MINAVAIL + 36, // Rote LED blinkend schnell
    CMD_LED_AMP_ROT_BL = _PULSE_CODE_MINAVAIL + 37, // Rote LED blinkend langsam
    CMD_LED_AMP_GRN_AN = _PULSE_CODE_MINAVAIL + 38,
    CMD_LED_AMP_GRN_AUS = _PULSE_CODE_MINAVAIL + 39,
    CMD_LED_AMP_GLB_AN = _PULSE_CODE_MINAVAIL + 40,
    CMD_LED_AMP_GLB_AUS = _PULSE_CODE_MINAVAIL + 41,
    CMD_LED_AMP_GLB_B = _PULSE_CODE_MINAVAIL + 42, // Gelbe LED blinkend


    // Motor
    CMD_MTR_RECHTS_S = _PULSE_CODE_MINAVAIL + 43,
    CMD_MTR_RECHTS_L = _PULSE_CODE_MINAVAIL + 44,
    CMD_MTR_STOP = _PULSE_CODE_MINAVAIL + 45,

    // Weiche
    CMD_WCH_AN = _PULSE_CODE_MINAVAIL + 46,
    CMD_WCH_AUS = _PULSE_CODE_MINAVAIL + 47,
    CMD_DURCHLASSEN = _PULSE_CODE_MINAVAIL + 48,


    // Auswerfer
    CMD_ASWERF_AN = _PULSE_CODE_MINAVAIL + 49,
    CMD_ASWERF_AUS = _PULSE_CODE_MINAVAIL + 50,
    CMD_AUSSORTIEREN = _PULSE_CODE_MINAVAIL + 51,

    // Rutsche
    CMD_RTSCH1_NV = _PULSE_CODE_MINAVAIL + 52, // Rutsche 1 nicht voll
    CMD_RTSCH2_NV = _PULSE_CODE_MINAVAIL + 53, // Rutsche 2 nicht voll

    // LEDs von Taster
    CMD_LED_ST_AN = _PULSE_CODE_MINAVAIL + 54,
    CMD_LED_ST_AUS = _PULSE_CODE_MINAVAIL + 55,
    CMD_LED_RT_AN = _PULSE_CODE_MINAVAIL + 56,
    CMD_LED_RT_AUS = _PULSE_CODE_MINAVAIL + 57,

    //LEDs
    CMD_LED_Q1_AN = _PULSE_CODE_MINAVAIL + 58,
    CMD_LED_Q1_AUS = _PULSE_CODE_MINAVAIL + 59,
    CMD_LED_Q2_AN = _PULSE_CODE_MINAVAIL + 60,
    CMD_LED_Q2_AUS = _PULSE_CODE_MINAVAIL + 61,

    // Uebergabe
    S_UEBERGABE_WS = _PULSE_CODE_MINAVAIL + 62,
    S_UEBERGABE_WS_FERTIG = _PULSE_CODE_MINAVAIL + 63,
    S_FESTO2_BEREIT = _PULSE_CODE_MINAVAIL + 64, // Uebergabe ist moeglich
    S_FESTO2_NBEREIT = _PULSE_CODE_MINAVAIL + 65, // Uebergabe ist NICHT moeglich

    // Zustand der Anlage
    CMD_SP_ZST_1 = _PULSE_CODE_MINAVAIL + 66, // Zustand speichern vor Fehler der Anlage 1
    CMD_SP_ZST_2 = _PULSE_CODE_MINAVAIL + 67, // Zustand speichern vor Fehler der Anlage 2
    CMD_WH_ZST_1 = _PULSE_CODE_MINAVAIL + 68, // Zustand wiederherstellen vor Fehler der Anlage 1
    CMD_WH_ZST_2 = _PULSE_CODE_MINAVAIL + 69, // Zustand wiederherstellen vor Fehler der Anlage 2

    // Fehler
    EC_VBDNG_UB = _PULSE_CODE_MINAVAIL + 70, // Verbindung zwischen den Anlagen ist unterbrochen
    EC_WS_VRLN = _PULSE_CODE_MINAVAIL + 71, // Werkstueck verloren
    EC_WS_UNB = _PULSE_CODE_MINAVAIL + 72, // Werkstueck unbekannt
    EC_WS_VERSCHW = _PULSE_CODE_MINAVAIL + 73, // Werkstuck verschwunden
    EC_RTSCH1_VOLL = _PULSE_CODE_MINAVAIL + 74, // Rutche der Anlage 1 ist voll
    EC_RTSCH2_VOLL = _PULSE_CODE_MINAVAIL + 75, // Rutsche der Anlage 2 ist voll
    EC_ABSTAND_NE = _PULSE_CODE_MINAVAIL + 76, // Werkstueck wird nicht mit sicheren Abstand gelegt
    EC_RTSCH_VOLL = _PULSE_CODE_MINAVAIL + 77,

    // Werkstuecke
    CMD_WS_BEKANNT = _PULSE_CODE_MINAVAIL + 78, // Hoehe des WS ist bekannt
    CMD_WS_UNBEKANNT = _PULSE_CODE_MINAVAIL + 79, // Hoehe des WS ist UNbekannt

    // Festo
    CMD_FESTOS_FREI = _PULSE_CODE_MINAVAIL + 80, // Kein Werkstueck in der Gesamteanlage vorhanden
    S_FESTO1_FREI = _PULSE_CODE_MINAVAIL + 107,
    S_FESTO1_NFREI = _PULSE_CODE_MINAVAIL + 110,
    S_FESTO2_FREI = _PULSE_CODE_MINAVAIL + 111,
    S_FESTO2_NFREI = _PULSE_CODE_MINAVAIL + 112,
    S_FESTO1_STOPPEN = _PULSE_CODE_MINAVAIL + 113,
    S_WS_AN_FESTO2 = _PULSE_CODE_MINAVAIL + 114,

    // --------------------------------------------------------
    // HAL
    // --------------------------------------------------------

    // Timer
    T_LSA_LSH = _PULSE_CODE_MINAVAIL + 81,
    T_LSH_LSM = _PULSE_CODE_MINAVAIL + 82,
    T_LSW_LSE = _PULSE_CODE_MINAVAIL + 83,
    T_LSE1_LSA2 = _PULSE_CODE_MINAVAIL + 84,
    T_Q1_LSH = _PULSE_CODE_MINAVAIL + 85,
    T_LSA_Q1 = _PULSE_CODE_MINAVAIL + 86,
    T_PING_PONG = _PULSE_CODE_MINAVAIL + 87,
    T_PULSE_TIMER = _PULSE_CODE_MINAVAIL + 95,
    T_PULSE_TICK = _PULSE_CODE_MINAVAIL + 96,
    T_TOLERANZZEIT_LSA = _PULSE_CODE_MINAVAIL + 97,
    T_TOLERANZZEIT_LSH = _PULSE_CODE_MINAVAIL + 98,
    T_TOLERANZZEIT_LSW = _PULSE_CODE_MINAVAIL + 99,
    T_TOLERANZZEIT_LSE = _PULSE_CODE_MINAVAIL + 100,
    T_LSW_LSR = _PULSE_CODE_MINAVAIL + 101,
    T_PULSE_RUTSCHE_VOLL = _PULSE_CODE_MINAVAIL + 106,

    ///WerkstückeTyp
    WERKSTUECK_FLACH = _PULSE_CODE_MINAVAIL + 89,
    WERKSTUECK_HOCH = _PULSE_CODE_MINAVAIL + 90,
    WERKSTUECK_MIT_BOHRUNG = _PULSE_CODE_MINAVAIL + 91,
    WERKSTUECK_UNBEKANNT = _PULSE_CODE_MINAVAIL + 92,
    WERKSTUECK_ERKANNT = _PULSE_CODE_MINAVAIL + 94,
    WERKSTUECK_MIT_BOHRUNG_M = _PULSE_CODE_MINAVAIL + 108,
    WERKSTUECK_ANDERS = _PULSE_CODE_MINAVAIL + 109,

    //Fehlerbehandlung Partner Bescheid geben
    S_PARTNER_FEHLER_1 = _PULSE_CODE_MINAVAIL + 102,
    S_PARTNER_OK_1 = _PULSE_CODE_MINAVAIL + 103,
    S_PARTNER_FEHLER_2 = _PULSE_CODE_MINAVAIL + 104,
    S_PARTNER_OK_2 = _PULSE_CODE_MINAVAIL + 105,
};

#endif