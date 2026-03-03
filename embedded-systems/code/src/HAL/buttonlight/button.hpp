/**
 * @file button.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Taste.
 *
 * Verwaltet die Eingabe durch Taster (Interrupt-basiert) und die Ansteuerung
 * der zugehörigen LEDs.
 */

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../../gpio_address.hpp"
#include "buttonlight.hpp"
#include "buttontype.hpp"
#include "../../Dispatcher/signal.hpp"
#include "../../Dispatcher/IDispatcher.hpp"


/**
 * REQ_27: Der Zustand der Anlagen wechselt von Ruhezustand in Service-Mode, wenn
 * der Start-Taster laenger als 1 Sekunde gedrueckt wird.
 */
#define SERVICEMODE_START 1
#define DEBOUCE_OFFSET 0x150
#define DEBOUCE_TIME_OFFSET 0x154

class Taste {
private:
	TypTaste taste;
	IDispatcher* dispatcher;
	int idd;
	int letztesMal = -1;
	bool tasteStatus;
	bool lichtStatus;
	int interruptID;
	bool receivingRunning;
	int startdruckZeit;
	TasteLicht setTasteLicht();

	TasteLicht licht;



	void initLED();
	int chanID;
	int conID;

public:
	Taste(TypTaste given_taste);
	Taste(TypTaste given_taste, IDispatcher* given_disp,int id);
	TasteLicht getTasteLicht();
	TypTaste getTypTaste();
	bool getLichtStatus();
	bool getTasteStatus();
//	void pruefeDruck();
//	getTasteDruckZeit();
	void taste_isr();
	void receivingRoutine(int channelID);
	void handleInterrupt();
	void ledAn();
	void ledAus();
	int berechne(uintptr_t gpioBase, int intrStatusReg);
	int getDruckZeit();
};

#endif
