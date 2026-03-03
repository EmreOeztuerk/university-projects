/**
 * @file light.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Licht.
 *
 * Dient zur Steuerung der zusätzlichen Leuchten Q1 (Anzeige 1) und Q2 (Anzeige 2)
 * auf dem Bedienpanel (nicht die Ampel).
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../../gpio_address.hpp"
#include "lighttype.hpp"

class Licht {
private:
	TypLicht licht;
	bool lichtStatus;
	void initLED();

public:
	Licht(TypLicht given_licht);
	bool getLichtStatus();
	TypLicht getTypLicht();
	void ledAn();
	void ledAus();
};

#endif
