/**
* @file ContextData.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Datenhaltung für den FSM-Kontext.
 *
 * Speichert persistente Daten, die über Zustandswechsel hinweg erhalten bleiben müssen,
 * wie z.B. die Liste der Werkstücke auf dem Band, Zählerstände oder Kalibrierwerte.
 */

#ifndef CONTEXTDATA_H
#define CONTEXTDATA_H

#include <vector>
#include <deque>
#include <string>
#include <chrono>
#include <map>

#include "../HAL/sorter/sorttype.hpp"
#include "common/Types.h"
#include "../Dispatcher/signal.hpp"
#include "Werkstueck.hpp"
#include "regionTimer.hpp"


class ContextData {
public:
    ContextData(bool f1, bool f2, Actions *shared_action);

    bool festo1;
    bool festo2;

    Actions *action;

    /* Zeiten */
    double t_lsa_lsh = 0;
    double t_lsh_lsm = 0;
    double t_lsw_lse = 0;
    double t_lse1_lsa2 = 0;
    double t_lsa_lsh_lsw = 0;
    double t_lsm_lsr = 0;

    //Zeit beim langsamlaufen
    double t_lsa_lsh_l = 0;
    double t_lsa_q1_l = 0.000;
    double t_lsh_lsw_l = 0;
    double t_lsw_lse_l = 0;
    double t_lsa_lsh_s = 0;
    double t_lsa_q1_s = 0;
    double t_lsh_lsw_s = 0;
    double t_lsw_lse_s = 0;
    double t_lse1_lsa2_l = 0;
    double t_lsa_lsh_lsw_l = 0;
    double t_lsw_lsr_l = 5000.0;

    //Zeit beim schnelllaufen
    double t_lse1_lsa2_s = 0;
    double t_lsa_lsh_lsw_s = 0;
    double t_lsw_lsr_s = 4000.0;


    // Definition der Timer für jede orthogonale Region
    RegionTimer lsa_timer;
    RegionTimer lsh_timer;
    RegionTimer lsw_timer;
    RegionTimer lse_timer;
    RegionTimer lsr_timer;

    virtual ~ContextData();

    std::size_t order_index; // Um zu wissen welche Typ kommen sollte
    std::deque<Werkstueck> werkstueckListe; // Liste für alle Werkstuecke
    std::deque<WerkstueckTyp> erwarteteTypenQueue; // Nur relevant fuer Anlage 2
    std::vector<WerkstueckTyp> sollReihenfolge; // Reihenfolge der Werkstuecke
    WerkstueckTyp typErwartetVonF1 = WerkstueckTyp::UNBEKANNT;
    int anzahlWerkstuecke; // Fuer Werkstueck-ID
    WerkstueckTyp typ = WerkstueckTyp::UNBEKANNT;
    WerkstueckTyp naechsteTyp = WerkstueckTyp::UNBEKANNT;
    Signal wsTyp = Signal::WERKSTUECK_UNBEKANNT;
    bool hatMetall = false;
    bool wollteAussortieren = false;
    bool anders = false;

    /* Prozess-/Statusdaten  */
    bool f2_bereit = true; // Festo2 bereit?
    bool f2_voll = false; // Festo2 voll?
    bool rutsche_voll = false; // (LSR) Rutsche voll
    bool rutsche2_voll = false; // (LSR2) zweite Rutsche voll
    bool festo1Frei = false;
    bool festo2Frei = true;
    bool FehlerVerlorenBeiLAS2 = false;
    int anzahlWSF1 = 0;
    int anzahlWSF2 = 0;

    /* Service Mode */
    bool kalibrierungFertig = false;
    bool kalibrierungLangFertig = false;
    bool letztesMal = false;
    SortTyp sortTyp = SortTyp::UNBEKANNT;
    bool reset_abgeschlossen = false; // Ruhezustand
    bool motorSchnell = true;

    bool getMotorSchnell() const;

    bool enteredHoheMessen = false;
    bool enteredWartenAufErlaubnisVonF2 = false;
    bool enteredWartenBisWSEntfernt = false;
    bool enteredAussortieren = false;

    void updateTimers(double delta_ms, bool istSchnell) {
        lsa_timer.aktualisiere(delta_ms, istSchnell);
        lsh_timer.aktualisiere(delta_ms, istSchnell);
        lsw_timer.aktualisiere(delta_ms, istSchnell);
        lse_timer.aktualisiere(delta_ms, istSchnell);
        lsr_timer.aktualisiere(delta_ms, istSchnell);
    }

    /* Fehlerbehandlung */
    struct FehlerInfo {
        Fehlerart art{Fehlerart::keinFehler};
        FehlerOrt ort{FehlerOrt::DEFAULT};
        AnlageId anlage{AnlageId::UNBEKANNT};
    };

    int anzahlFehler = 0; // anzahlFehler zählt offene Fehler
    const FehlerInfo &getLetzterFehler() const;

    void clearFehler();

    bool keineFehler() const;

    void setFehler(Fehlerart art, FehlerOrt ort);

    void setFehler(Fehlerart art, FehlerOrt ort, AnlageId anlage);

    AnlageId aktuelleAnlage() const;

    // Fehler erzeugen / erhöhen
    void addFehler(Fehlerart art, FehlerOrt ort);

    void addFehler(Fehlerart art, FehlerOrt ort, AnlageId anlage);

    // Fehler als gelöst markieren / runterzählen
    void fehlerGeloest();

    // Reset/Quittierung (z.B. Reset-Taste)
    void resetFehlerZaehler();

    /**
     * @brief Startet eine neue Zeitmessung.
     *        Es wird der aktuelle Zeitpunkt gespeichert.
     */
    void zeitStarten();

    /**
     * @brief Beendet die Zeitmessung und speichert die gemessene Dauer
     *        unter dem gegebenen Namen.
     *
     * @param name Schlüsselname unter dem die Zeit abgelegt wird.
     */
    //void zeitSpeichern(double& timeToSafe, const std::string& name);
    void zeitSpeichern(const std::string &name);

    /**
     * @brief Ruft eine zuvor gespeicherte Zeit ab.
     *
     * @param name Schlüsselname der Zeit.
     * @return double Zeit in Millisekunden oder -1.0 falls nicht vorhanden.
     */
    double holeZeit(const std::string &name) const;

    /*
     * Fehlermeldung
     * --------------
     * Gibt eine deutliche, farbige Fehlermeldung aus, wenn ein Not-Halt (E-Stop)
     * ausgelöst wurde. Diese Funktion wird typischerweise aus der FSM
     * oder aus der Aktor-/Sensorlogik aufgerufen.
     *
     * Parameter:
     *    typ:
     *        "E1"  → EStop wurde in der ersten Anlage gedrückt
     *        "E2"  → EStop wurde in der zweiten Anlage gedrückt
     *        "E"   → EStop wurde in beiden Anlagen gedrückt
     *
     *
     * Funktion:
     *    - Wählt anhand des Parameters den korrekten Anlagentext.
     *    - Gibt die Warnung in roter, fetter Schrift aus (ANSI-Farbcodes).
     *    - Die Ausgabe ist gut sichtbar für Debugging und Monitoring.
     */
    virtual void Fehlermeldung(const std::string &typ);


    /*
     * set()
     * ----------
     * Setzt eine boolesche Variable auf den angegebenen Wert.
     *
     * Parameter:
     *   variable : Referenz auf die bool-Variable, die geändert werden soll
     *   wert     : Der Wert, auf den die Variable gesetzt werden soll (true/false)
     *
     * Beispiel:
     *   bool estop = false;
     *   set(estop, true);  // estop == true
     */
    void set(bool &variable, bool wert);

    /**
     * @brief: Setzt den Sortiermechanismus der Anlage. Eine Anlage hat eine
     * Weiche oder einen Auswerfer.
     *
     * @param given_sortTyp: zu setzender Sortiermechanismus.
     */
    void setSortTyp(SortTyp given_sortTyp);


    /*---------------------------------------------------------------------------*/

    /*
     * Löscht alle Prozessdaten und setzt die Reset-Variable auf false.
     */
    bool datenprozessLoeschen();

    /*
     * Prüft, ob der Reset-Vorgang vollständig abgeschlossen ist.
     */
    bool istReset_abgeschlossen();

    // Guards
    bool gFesto1() const;

    bool gFesto2() const;

    bool gF2Bereit() const;

    bool gF2Voll() const;

    bool gTypGleichNaechster() const;

    bool gTypUngleichNaechster() const;

    bool gGleicheTyp() const;

    bool gNichtGleicheTyp() const;

    bool gRutscheVoll() const;

    bool gRutsche2Voll() const;

    bool gLSWUBFehlerRutsche2Voll() const;

    bool gLSWUBAussortieren() const;

    bool gLSWUBDurchlauf() const;

    void registerId1();

    void addWS_F1();

    void addWS_F2();

    void loescheWS_F1(); //Übergabe an Festo 2 beginnt
    void loescheWS_F2(); //WS entnommen Festo 2
    void registerId2(Signal signal);

    void registerId3(bool wert);

    void warnung() {
    };

    void warnungAufgehoben() {
    };

    void speichereZustand() {
    };

    void wiederherstelleZustand() {
    };

    void fehlerBeheben() {
    };

    Signal getMeasuredSignal() const;

    void setMeasuredSignal(const Signal &s);

    void werkstueckVerlaesstAnlage();

    void ueberpruefeReihenfolge();

    void advanceOrder();

    int getTypCodeForSending();

    void empfangeTypVonF1(int typCode);

private:
    /**
     * @brief Interne Hilfsmethode zum Speichern einer gemessenen Zeit.
     *
     * @param name Schlüsselname.
     * @param millis Zeitwert in Millisekunden.
     */
    void speichereZeit(const std::string &name, double millis);

    // Speichert Zeitwerte (Millisekunden) pro Namen.
    std::map<std::string, double> zeiten;

    // Startzeitpunkt einer Messung.
    std::chrono::steady_clock::time_point startZeit;

    FehlerInfo letzterFehler;
};


#endif /* CONTEXTDATA_H */