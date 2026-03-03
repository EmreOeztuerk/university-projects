/**
 * @file Actions.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief In dieser Klasse werden alle Befehle definiert, die an die HAL (Hardware Abstraction Layer)
 * gesendet werden. Jede Methode entspricht einem konkreten Aktor-Befehl
 * wie LED schalten, Motor steuern, Weiche stellen oder Auswerfer bedienen.
 *
 * Die konkrete Implementierung zum Senden (Dispatcher-Aufruf)
 * erfolgt in der abgeleiteten Klasse. Diese Klasse stellt lediglich das Interface dar,
 * das von der Zustandsmaschine (FSM) verwendet wird.
 */

#ifndef ACTIONS_H
#define ACTIONS_H

#include <string>
#include <chrono>
#include "../Dispatcher/dispatcher.hpp"

class Actions {
private:

    Dispatcher* dispatcher;

public:
	Actions( Dispatcher* disp);
	virtual ~Actions();



    /***************************************************************************
      * Ampel-LEDs
      * ---------------------
      * Befehle zur Steuerung der Ampelanzeigen: Rot, Grün, Gelb.
      * Zusätzlich werden Blinkmodi für die rote LED unterstützt.
      ***************************************************************************/

    virtual void cmd_led_amp_rot_an();   // Rote Ampel: EIN
    virtual void cmd_led_amp_rot_aus();  // Rote Ampel: AUS

    virtual void cmd_led_amp_rot_bs();   // Rote LED blinkend schnell
    virtual void cmd_led_amp_rot_bl();   // Rote LED blinkend langsam

    virtual void cmd_led_amp_grn_an();   // Grüne Ampel: EIN
    virtual void cmd_led_amp_grn_aus();  // Grüne Ampel: AUS


    virtual void cmd_led_amp_glb_an();   // Gelbe Ampel: EIN
    virtual void cmd_led_amp_glb_aus();  // Gelbe Ampel: AUS

    virtual void cmd_led_amp_gelb_b();   // Gelbe LED blinkend

    /***************************************************************************
     * Motorsteuerung
     * ---------------------
     * Steuert die Drehrichtung und Geschwindigkeit des Motors.
     ***************************************************************************/

    virtual void cmd_mtr_rechts_s();  // Motor rechts schnell
    virtual void cmd_mtr_rechts_l();  // Motor rechts langsam
    virtual void cmd_mtr_stop();      // Motor stoppen

    /***************************************************************************
     * Durchlass- / Aussortierlogik
     * ---------------------
     * Steuert, ob ein Werkstück auf dem Förderband weiterlaufen darf oder
     * aktiv aussortiert wird unabhägig ob die Anlage eine Weiche oder einen Auswerfer hat.
     ***************************************************************************/

    virtual void cmd_durchlassen();   // Werkstück passiert weiter auf dem Band
    virtual void cmd_aussortieren();  // Werkstück wird aussortiert

    /***************************************************************************
     * Weiche (Switch)
     * ---------------------
     * Schaltet die Weiche in zwei mögliche Positionen.
     ***************************************************************************/

    virtual void cmd_wch_an();       // Weiche: EIN / aktiv
    virtual void cmd_wch_aus();      // Weiche: AUS / inaktiv

    /***************************************************************************
     * Auswerfer
     * ---------------------
     * Steuert den pneumatischen Auswerfer.
     ***************************************************************************/

    virtual void cmd_aswerf_an();   // Auswerfer: aktivieren
    virtual void cmd_aswerf_aus();  // Auswerfer: deaktivieren

    /***************************************************************************
     * Taster-LEDs
     * ---------------------
     * LEDs an den Tastern.
     ***************************************************************************/

    virtual void cmd_led_st_an();   // Start-Taster LED: EIN
    virtual void cmd_led_st_aus();  // Start-Taster LED: AUS
    virtual void cmd_led_rt_an();   // Rote LED am Reset-Taster: EIN
    virtual void cmd_led_rt_aus();  // Rote LED am Reset-Taster: AUS

    /***************************************************************************
     * Weitere LEDs (Q1/Q2)
     * ---------------------
     * Q1 und Q2 LEDs
     ***************************************************************************/

    virtual void cmd_led_q1_an();   // Q1 LED: EIN
    virtual void cmd_led_q1_aus();  // Q1 LED: AUS
    virtual void cmd_led_q2_an();   // Q2 LED: EIN
    virtual void cmd_led_q2_aus();  // Q2 LED: AUS


    /***************************************************************************
     * Timer
     * ---------------------
     * Fuer Betriebszustand
     ***************************************************************************/

    virtual void t_lsa_q1();
    virtual void t_lse1_lsa2();         //Timer von LSE1 bis LSA2 abgelaufen
    virtual void t_toleranzzeit_lsa();  // Toleranzzeit nach T_LSE1_LSA2

    virtual void t_lsa_lsh();           //Timer von LSA bis LSH abgelaufen
    virtual void t_toleranzzeit_lsh();  // Toleranzzeit nach T_LSA_LSH

    virtual void t_lsh_lsw();           //Timer von LSH bis LSW abgelaufen
    virtual void t_lsw_lsr();           //Timer von LSW bis LSR abgelaufen
    virtual void t_toleranzzeit_lsw();  // Toleranzzeit nach T_LSH_LSW

    virtual void t_lsw_lse();           //Timer von LSW bis LSE abgelaufen
    virtual void t_toleranzzeit_lse();  // Toleranzzeit nach T_LSW_LSE



    /***************************************************************************
     * Fehler und Fehlerbehebung Signal
     * ---------------------
     * Fuer Betriebszustand und Fehlerbehandlung
     ***************************************************************************/


    virtual void cmd_partner_fehler_melden();
    virtual void cmd_partner_ok_melden();
    /***************************************************************************
       * Timer
       * ---------------------
       * Fuer Betriebszustand
       ***************************************************************************/
    virtual void wsTypSenden(int sig);
    virtual void wsAnders();


    /***************************************************************************
       * fFesto2 bereit
     ***************************************************************************/

    virtual void s_festo2_bereit();
    virtual void s_festo2_nbereit();
    virtual void s_festo1_frei();
    virtual void s_festo1_nfrei();
    virtual void s_festo2_frei();
    virtual void s_festo2_nfrei();
    virtual void cmd_festos_frei();
    virtual void s_festo1_stoppen();
    virtual void s_ws_an_festo2();
    /***************************************************************************
       * for didactic / testing purpose only
     ***************************************************************************/

    virtual void enteredEStop(){};
    virtual void enteredEStop1Aktiv(){};
    virtual void enteredEStop2Aktiv(){};
    virtual void enteredEStopsAktiv(){};
    virtual void enteredResetErlaubt(){};
    virtual void enteredWartenReset1(){};
    virtual void enteredWartenReset2(){};


    /***************************************************************************
         * ZUM TESTEN
         * ---------------------
         * Ruhezustand
         ***************************************************************************/
      virtual void enteredWartezustand(){};
      virtual void enteredStartTasterWarten11(){};
      virtual void enteredStartTasterWarten22(){};
      virtual void enteredStartTasterWarten1(){};
      virtual void enteredStartTasterWarten2(){};
      virtual void enteredResetTasterWarten1(){};
      virtual void enteredResetTasterWarten2(){};
      virtual void enteredBetriebsbereit(){};

      //virtual void enteredStartTasterWarten2(){};
      //virtual void enteredResetTasterWarten1(){};
      //virtual void enteredResetTasterWarten2(){};
      //virtual void enteredBetriebsbereit(){};

    /***************************************************************************
         * ZUM TESTEN
         * ---------------------
         * Betriebszustand
         ***************************************************************************/
    virtual void enteredBetriebszustand(){};
    virtual void enteredAufWSWarten(){};
    virtual void enteredSortierung(){};
    virtual void enteredWartenAufLSA(){};
    virtual void enteredBeginnSortierprozessF1(){};
    virtual void enteredBeginnSortierprozessF2(){};
    virtual void enteredNachstesWSErlaubt(){};
    virtual void enteredWartenAufWSBeiLSA(){};
    virtual void enteredWartenAufLSH(){};
    virtual void enteredWartenAufWSBeiLSH(){};
    virtual void enteredHoeheMessen(){};
    virtual void enteredWartenAufLSW(){};
    virtual void enteredWartenAufWSBeiLSW(){};
    virtual void enteredKomplettTypErkennen(){};
    virtual void enteredWSAussortieren(){};
    virtual void enteredWSDurchlauf(){};
    virtual void enteredWartenAufLSE(){};
    virtual void enteredWartenAufWSBeiLSE(){};
    virtual void enteredWartenBisWSEntfernt(){};
    virtual void enteredWartenAufErlaubnisVonF2(){};
    virtual void enteredWartenAufWarnung(){};
    virtual void enteredWarnsignalAktivieren(){};
    virtual void enteredRegionLSAWartenAufLSA(){};
    virtual void enteredRegionLSABeginnSortierprozessF1(){};
    virtual void enteredRegionLSABeginnSortierprozessF2(){};
    virtual void enteredRegionLSANaechstesWSErlaubt(){};
    virtual void enteredRegionLSAWartenAufWSBeiLSA(){};
    virtual void enteredRegionLSHWartenAufLSH(){};
    virtual void enteredRegionLSHWartenAufWSBeiLSH(){};
    virtual void enteredRegionLSHHoeheMessen(){};
    virtual void enteredRegionLSHBisLSWWartenAufLSW(){};
    virtual void enteredRegionLSHBisLSWWartenAufWSBeiLSW(){};
    virtual void enteredRegionLSHBisLSWKomplettTypErkennen(){};
    virtual void enteredRegionLSHBisLSWWSDurchlauf(){};
    virtual void enteredRegionLSHBisLSWWSAussortieren(){};
    virtual void enteredRegionLSWBisLSEWartenAufLSE(){};
    virtual void enteredRegionLSWBisLSEWartenAufWSBeiLSE(){};
    virtual void enteredRegionLSWBisLSEWartenAufErlaubnisVonF2(){};
    virtual void enteredRegionLSWBisLSEWartenBisWSEntfernt(){};
    virtual void enteredRegionWarnungWartenAufWarnung(){};
    virtual void enteredRegionWarnungWarnsignalAktivieren(){};

    /***************************************************************************
         * ZUM TESTEN
         * ---------------------
         * Betriebszustand
         ***************************************************************************/
    virtual void enteredFehlerbehandlung(){};
    virtual void enteredAnstehendUnquittiert(){};
    virtual void enteredAnstehendQuittiert(){};
    virtual void enteredGegangenUnquittiert(){};
    virtual void enteredRutscheLeeren(){};
    virtual void enteredUnbekanntenWSEntnehmen(){};
    virtual void enteredFehlendesWS(){};
    virtual void enteredAbstandRegeln(){};

    /***************************************************************************
     	 * ZUM TESTEN
     	 * ---------------------
     	 * Servicemode
     	 ***************************************************************************/
    virtual void enteredServicemode_Wartezustand() {};
    /* Best-Case Kalibrierung */
    virtual void enteredInitialisierung() {};

    virtual void enteredAnfangKalibrierung() {};
    virtual void enteredAnfangKalibrierungSchnell() {};

    virtual void enteredMotorLaeuft() {};
    virtual void enteredMotorLaeuft2() {};
    virtual void enteredMotorLaeuft3() {};
    virtual void enteredMotorLaeuft4() {};

    virtual void enteredWerkstueckLaeuft() {};
    virtual void enteredWerkstueckLaeuft2() {};
    virtual void enteredWerkstueckLaeuft3() {};
    virtual void enteredWerkstueckLaeuft4() {};
    virtual void enteredWerkstueckLaeuft5() {};
    virtual void enteredWerkstueckLaeuft6() {};

    virtual void enteredHSzustand() {};
    virtual void enteredHSzustand2() {};
    virtual void enteredHSzustand3() {};
    virtual void enteredHSzustand4() {};

    virtual void enteredMetallsensorBereich() {};
    virtual void enteredMetallsensorBereich2() {};
    virtual void enteredMetallsensorBereich3() {};
    virtual void enteredMetallsensorBereich4() {};

    virtual void enteredAussortierungsbereich() {};
    virtual void enteredAussortierungsbereich2() {};
    virtual void enteredAussortierungsbereich3() {};
    virtual void enteredAussortierungsbereich4() {};

    virtual void enteredRutschezustand() {};
    virtual void enteredRutschezustand2() {};
    virtual void enteredRutschezustand3() {};
    virtual void enteredRutschezustand4() {};

    virtual void enteredWeichezustand() {};
    virtual void enteredWeichezustand2() {};
    virtual void enteredWeichezustand3() {};
    virtual void enteredWeichezustand4() {};

    virtual void enteredWerkstueckAmEnde() {};
    virtual void enteredWerkstueckAmEnde2() {};

    virtual void enteredEndeKalibrierungAE() {};
    virtual void enteredEndeKalibrierungLangAE() {};
    virtual void enteredEndeKalibrierungLangsam() {};


    /* Worst-Case Kalibrierung */
    virtual void enteredAnfangWCKalibrierung() {};
    virtual void enteredMotorLaeuftWC() {};
    virtual void enteredWerkstueckLaeuftWC() {};
    virtual void enteredHSzustandWC() {};

};

#endif /* ACTIONS_H */

