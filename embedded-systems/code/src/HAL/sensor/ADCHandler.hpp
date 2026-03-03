/**
 * @file ADCHandler.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse ADCHandler.
 *
 * Diese Klasse verbindet den Low-Level ADC-Treiber mit dem Dispatcher.
 * Sie startet Messungen und verarbeitet die Ergebnisse in einem eigenen Thread.
 */

#ifndef HAL_SENSOR_ADCHANDLER_HPP_
#define HAL_SENSOR_ADCHANDLER_HPP_

#include <thread>
#include "../hight/ADC.h"
#include <sys/neutrino.h>
#include <iostream>
#include <sys/dispatch.h>
#include <stdio.h>
#include "WerkstueckErkennung.hpp"
#include "../HAL.hpp"
#include <new>
#include "../motor/motor.hpp"
#include "../../Dispatcher/dispatcher.hpp"

#define PRIO_SENSOR 10
#define HOESENSOR_SIGNAL 2

class ADCHandler{
public:
	ADCHandler(Dispatcher* dispatcher);
	virtual ~ADCHandler();
	ADC* adc;
	void receivingRoutine(int channelID,ADC* adc);
	std::thread receivingThread;
	Dispatcher* _dispatcher;


private:
	int chanID;
	int conID; //Connection
	int conIDDisp;
	TSCADC tsc;
	WerkstueckErkennung werk;

};

#endif /* HAL_SENSOR_ADCHANDLER_HPP_ */
