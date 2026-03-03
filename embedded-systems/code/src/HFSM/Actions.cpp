/**
 * @file Actions.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Actions-Klasse.
 *
 * Übersetzt die Methodenaufrufe der FSM in konkrete Nachrichten (`Signal`),
 * die über den Dispatcher an die HAL oder andere Komponenten gesendet werden.
 */

#include "../Dispatcher/dispatcher.hpp"

#include <iostream>

#include "Actions.h"
using namespace std;

Actions::Actions( Dispatcher* disp)
    :  dispatcher(disp) {}

Actions::~Actions(){

}


// Ampel
void Actions::cmd_led_amp_rot_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_ROT_AN);
}

void Actions::cmd_led_amp_rot_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_ROT_AUS);
}

void Actions::cmd_led_amp_rot_bs()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_ROT_BS);
}

void Actions::cmd_led_amp_rot_bl()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_ROT_BL);
}

void Actions::cmd_led_amp_grn_an()
{

	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_GRN_AN);
}
void Actions::cmd_led_amp_grn_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_GRN_AUS);
}

void Actions::cmd_led_amp_glb_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_GLB_AN);
}

void Actions::cmd_led_amp_glb_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_GLB_AUS);
}

void Actions::cmd_led_amp_gelb_b()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_AMP_GLB_B);
}

// Motor
void Actions::cmd_mtr_rechts_s()
{
	dispatcher->sendenNachricht(Signal::CMD_MTR_RECHTS_S);
}

void Actions::cmd_mtr_rechts_l()
{
	dispatcher->sendenNachricht(Signal::CMD_MTR_RECHTS_L);
}
void Actions::cmd_mtr_stop()
{
	dispatcher->sendenNachricht(Signal::CMD_MTR_STOP);
}

//Durchlassen / Aussortieren
void Actions::cmd_durchlassen()
{
	dispatcher->sendenNachricht(Signal::CMD_DURCHLASSEN);
}

void Actions::cmd_aussortieren()
{
	dispatcher->sendenNachricht(Signal::CMD_AUSSORTIEREN);
}

// Weiche
void Actions::cmd_wch_an()
{
	dispatcher->sendenNachricht(Signal::CMD_WCH_AN);
}

void Actions::cmd_wch_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_WCH_AUS);
}

// Auswerfer
void Actions::cmd_aswerf_an()
{
	dispatcher->sendenNachricht(Signal::CMD_ASWERF_AN);
}
void Actions::cmd_aswerf_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_ASWERF_AUS);
}


// LEDs von Taster
void Actions::cmd_led_st_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_ST_AN);
}

void Actions::cmd_led_st_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_ST_AUS);
}

void Actions::cmd_led_rt_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_RT_AN);
}

void Actions::cmd_led_rt_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_RT_AUS);
}

//LEDs
void Actions::cmd_led_q1_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_Q1_AN);
}

void Actions::cmd_led_q1_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_Q1_AUS);
}

void Actions::cmd_led_q2_an()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_Q2_AN);
}

void Actions::cmd_led_q2_aus()
{
	dispatcher->sendenNachricht(Signal::CMD_LED_Q2_AUS);
}


/*
 * Timer
 */

void Actions::t_lsa_q1()
{
	dispatcher->sendenNachricht(Signal::T_LSA_Q1);
}

void Actions::t_lse1_lsa2()
{
	dispatcher->sendenNachricht(Signal::T_LSE1_LSA2);
}

void Actions::t_toleranzzeit_lsa()
{
	dispatcher->sendenNachricht(Signal::T_TOLERANZZEIT_LSA);
}

void Actions::t_lsa_lsh()
{
	dispatcher->sendenNachricht(Signal::T_LSA_LSH);
}

void Actions::t_toleranzzeit_lsh()
{
	dispatcher->sendenNachricht(Signal::T_TOLERANZZEIT_LSH);
}


void Actions:: t_lsh_lsw()
{
	dispatcher->sendenNachricht(Signal::T_LSH_LSM);
}

void Actions::t_lsw_lsr()
{
	dispatcher->sendenNachricht(Signal::T_LSW_LSR);
}


void Actions::t_toleranzzeit_lsw()
{
	dispatcher->sendenNachricht(Signal::T_TOLERANZZEIT_LSW);
}

void Actions::t_lsw_lse()
{
	dispatcher->sendenNachricht(Signal::T_LSW_LSE);
}


void Actions::t_toleranzzeit_lse()
{
	dispatcher->sendenNachricht(Signal::T_TOLERANZZEIT_LSE);
}

void Actions::cmd_partner_fehler_melden() {
	// Sendet den Befehl an den EIGENEN Dispatcher
	if(dispatcher->idd == 1){
		dispatcher->sendenNachricht(Signal::S_PARTNER_FEHLER_1);
	}else{
		dispatcher->sendenNachricht(Signal::S_PARTNER_FEHLER_2);
	}
}

void Actions::cmd_partner_ok_melden() {
	if(dispatcher->idd == 1){
		dispatcher->sendenNachricht(Signal::S_PARTNER_OK_1);
	}else{
		dispatcher->sendenNachricht(Signal::S_PARTNER_OK_2);
	}
}

void Actions::s_festo2_bereit()
{
	dispatcher->sendenNachricht(Signal::S_FESTO2_BEREIT);

}

void Actions::s_festo2_nbereit()
{
	dispatcher->sendenNachricht(Signal::S_FESTO2_NBEREIT);

}

void Actions::s_festo1_frei()
{
	dispatcher->sendenNachricht(Signal::S_FESTO1_FREI);
}

void Actions::s_festo1_nfrei()
{
	dispatcher->sendenNachricht(Signal::S_FESTO1_NFREI);
}

void Actions::s_festo2_frei()
{
	dispatcher->sendenNachricht(Signal::S_FESTO2_FREI);
}

void Actions::s_festo2_nfrei()
{
	dispatcher->sendenNachricht(Signal::S_FESTO2_NFREI);
}

void Actions::s_ws_an_festo2()
{
	dispatcher->sendenNachricht(Signal::S_WS_AN_FESTO2);
}

void Actions::s_festo1_stoppen()
{
	dispatcher->sendenNachricht(Signal::S_FESTO1_STOPPEN);
}
void Actions::cmd_festos_frei()
{
	dispatcher->sendenNachricht(Signal::CMD_FESTOS_FREI);
}
void Actions::wsTypSenden(int sig){
	if(dispatcher->idd == 1){
		dispatcher->sendenNachricht(Signal::S_UEBERGABE_WS,sig);
		std::cout << "******TYPSENDEN GESENDET AN ANLAGE 2**********" << std::endl;
		}
	}

void Actions::wsAnders(){
	dispatcher->sendenNachricht(Signal::WERKSTUECK_ANDERS,0);
	std::cout << "******TYPSENDEN GESENDET AN ANLAGE 1**********" << std::endl;
}

