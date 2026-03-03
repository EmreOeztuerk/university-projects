/**
 * @file Context.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Die Klasse Context stellt den Kontext der Zustandsmaschine (FSM) dar.
 *
 * - Sie verwaltet:
 *      - den aktuellen Zustand (BaseState* state)
 *      - die Aktionen zur Ansteuerung der HAL (Actions* action)
 *      - zustandsbezogene Kontextdaten (ContextData data)
 *
 * - Alle Methoden s_*, cmd_*, ec_* und t_* repräsentieren Ereignisse:
 *      - s_* : Sensor-Ereignisse / Taster / E-Stop / Verbindung
 *      - cmd_*: interne Kommandos / Zustandswechsel der Anlage
 *      - ec_* : Fehlerereignisse
 *      - t_* : Timer-Ereignisse
 *
 * Die FSM ruft diese Methoden auf, um Zustandsübergänge auszulösen.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "Actions.h"
#include "rootfsm/basestate.h"
#include "ContextData.h"
#include <sys/dispatch.h>
#include <time.h>

#define ATTACH_POINT_HFSM1 "HFSM_1"
#define ATTACH_POINT_HFSM2 "HFSM_2"


class Context {
private:
    BaseState *state; // aktueller Zustand der Zustandsmaschine
    Actions *action; // Schnittstelle zu den Aktor-Befehlen (HAL)
    name_attach_t *attachHFSM;
    ContextData data; // Kontextdaten zur Laufzeit (z.B. Zähler, Flags)
    bool hatMetall;
    bool festo1;
    bool festo2;

    bool erstes = true;

public:
    Context(Actions *shared_action, bool Festo1, bool Festo2);

    virtual ~Context();

    uint64_t get_now_ms() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t) ts.tv_sec * 1000 + (uint64_t) ts.tv_nsec / 1000000;
    }

    // --------------------------------------------------------
    // SENSOR-EVENTS
    // --------------------------------------------------------
    // Diese Methoden werden aufgerufen, wenn ein bestimmter Sensor
    // der Anlage seinen Zustand ändert (unterbrochen / nicht unterbrochen,
    // betätigt / nicht betätigt etc.). Die FSM reagiert darauf mit
    // Zustandsübergängen und Aktor-Befehlen über Actions.


    // Lichtschranke Anfang (LSA)
    void s_lsa1_ub(); // LSA unterbrochen
    void s_lsa2_ub(); // LSA unterbrochen
    void s_lsa_nub(); // LSA nicht unterbrochen

    // Lichtschranke Höhe (LSH)
    void s_lsh_ub();

    void s_lsh_nub();

    // Lichtschranke Weiche (LSW)
    void s_lsw_ub();

    void s_lsw_nub();

    // Lichtschranke Rutsche (LSR)
    void s_lsr_ub();

    void s_lsr_nub();

    // Lichtschranken Ende (LSE1/LSE2)
    void s_lse1_ub(); // LSE1 unterbrochen (Werkstück am Ende erster Anlage)
    void s_lse1_nub();

    void s_lse2_ub(); // LSE2 unterbrochen (Werkstück am Ende zweiter Anlage)
    void s_lse2_nub();

    // Metallsensor
    void s_metal_an();

    //void s_metal_aus();

    // Hoehensensor-Wert
    void s_hs_wert(); // Wert des Hoehensensor

    // --------------------------------------------------------
    // AKTORIK / TASTER-EVENTS
    // --------------------------------------------------------
    // Diese Methoden werden aufgerufen, wenn Taster betätigt werden
    // (Start, Stop, Reset, E-Stop). Die FSM entscheidet dann über
    // Start/Stop/Reset der Anlage.

    // Start-Taster
    void s_st_g(); // Start-Taster kurz gedrueckt
    void s_st1_g(); // Start-Taster kurz gedrueckt
    void s_st2_g(); // Start-Taster kurz gedrueckt
    void s_st1_lg(); // Start-Taster1 lang gedrueckt
    void s_st2_lg();

    // Stop-Taster
    void s_stp_g(); // Stop-Taster

    // Reset-Taster
    void s_rt1_g(); // Reset-Taster Festo 1
    void s_rt2_g(); // Festo 2

    // E-Stop
    void s_e1_g(); // ESTOP1 gedrueckt
    void s_e1_ng(); // EStop1 herzogezogen

    void s_e2_g();

    void s_e2_ng();

    // --------------------------------------------------------
    // VERBINDUNGSÜBERWACHUNG
    // --------------------------------------------------------
    // Überwacht die Kommunikation zwischen den Anlagen ( Festo 1/2)
    void s_vbdn();

    void s_nvbdn();


    // --------------------------------------------------------
    // ÜBERGABE ZWISCHEN ANLAGEN / FESTO 1 & 2
    // --------------------------------------------------------
    // Ereignisse im Zusammenhang mit der Übergabe von Werkstücken
    // zwischen Festo 1 und Festo 2.

    void s_uebergabe_ws();

    void s_uebergabe_ws_fertig();

    void s_festo2_bereit(); // Festo 2 meldet: bereit für Übergabe
    void s_festo2_nbereit(); // Festo 2 meldet: nicht bereit
    void s_festo1_stoppen();

    void s_ws_an_festo2();

    // Zustand der Anlage
    void cmd_sp_zst_1();

    void cmd_sp_zst_2();

    void cmd_wh_zst_1();

    void cmd_wh_zst_2();

    void cmd_rtsch_voll();

    void cmd_rtsch_nv();

    // --------------------------------------------------------
    // FEHLER-EVENTS
    // --------------------------------------------------------
    // Diese Methoden signalisieren Fehlerzustände, welche die FSM in
    // einen Fehler- / Störungszustand überführen können.

    void ec_vbdng_ub(); // Verbindungsüberwachung meldet Fehler
    void ec_ws_vrln(); // Werkstück verloren (erwartet, aber nicht mehr gesehen)
    void ec_ws_unb(); // Werkstück unbekannt (z.B. kein Datensatz)
    // void ec_ws_verschw();  // Werkstück verschwunden (Sensorabfolge inkonsistent)
    // void ec_rtsch1_voll(); // Rutsche 1 voll
    // void ec_rtsch2_voll(); // Rutsche 2 voll
    void ec_abstand_ne(); // Abstand zwischen Werkstücken nicht eingehalten
    void ec_rtsch_voll(); // Rutsche voll (allgemein)

    // Werkstuecke
    void cmd_ws_bekannt();

    void cmd_ws_unbekannt();

    // Festo
    void cmd_festos_frei();

    void s_festo1_frei();

    void s_festo1_nfrei();

    void s_festo2_frei();

    void s_festo2_nfrei();

    void s_partner_fehler_1();

    void s_partner_ok_1();

    void s_partner_fehler_2();

    void s_partner_ok_2();

    //void  cmd_rtsch_nv();
    //void  cmd_rtsch2_nv();


    // --------------------------------------------------------
    // TIMER-EVENTS
    // --------------------------------------------------------
    // Diese Methoden werden von Timer-Interrupts / -Callbacks aufgerufen.
    // Sie modellieren Zeitüberschreitungen oder definierte Zeitabstände,
    // die in der FSM für Plausibilität und Ablaufüberwachung genutzt werden.

    void t_lsa_lsh(); // Zeit zwischen LSA und LSH abgelaufen
    void t_lsh_lsw(); // Zeit zwischen LSH und LSW abgelaufen
    void t_lsw_lse(); // Zeit zwischen LSW und LSE abgelaufen
    void t_lse1_lsa2(); // Zeit zwischen LSE1 und zweiter LSA (Festo 2) abgelaufen
    void t_lsw_lsr(); // Maximal Zeit zwischen LSW und LSR abgelaufen
    void t_q1_lsh(); // Zeit zwischen Q1 und LSH abgelaufen
    void t_lsa_q1(); // Zeit zwischen LSA und Q1 abgelaufen
    void t_toleranzzeit_lsa(); // LSA Region Toleranzzeit überschritten
    void t_toleranzzeit_lsh(); // LSH Region Toleranzzeit überschritten
    void t_toleranzzeit_lsw(); // LSW Region Toleranzzeit überschritten
    void t_toleranzzeit_lse(); // LSE Region Toleranzzeit überschritten
    void t_ping_pong(); // Ping/Pong-Timer für Verbindungsüberwachung


    void handleEvent(Signal sig, int wert);

    void run();

    void initFSM();
};

#endif /* CONTEXT_H */