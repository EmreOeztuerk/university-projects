/**
 * @file button.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Taste-Klasse.
 *
 * Enthält die ISR-Logik (Interrupt Service Routine) für die Tastererkennung
 * sowie Funktionen zur LED-Steuerung.
 */

#include "button.hpp"

Taste::Taste(TypTaste given_taste) {
	taste = given_taste;
	licht = setTasteLicht();
	tasteStatus = false;
	lichtStatus = false;
	interruptID = -1;
	receivingRunning = false;
	dispatcher = nullptr;
	startdruckZeit = 0;

	initLED();
}

Taste::Taste(TypTaste given_taste, IDispatcher* given_disp, int id)
    : taste(given_taste),
      dispatcher(given_disp),
      idd(id),
      tasteStatus(false),
      lichtStatus(false),
      interruptID(-1),
      receivingRunning(false),
      startdruckZeit(0)
{
    // Diese Zuweisungen benötigen Logik/Funktionsaufrufe,
    // daher können sie hier im Rumpf bleiben:
    licht = setTasteLicht();
   // timer = new AktivitaetsTimer();

    // Initialisierungs-Logik
    initLED();
}

TasteLicht Taste::getTasteLicht() {
	return licht;
}

TypTaste Taste::getTypTaste() {
	return taste;
}

bool Taste::getLichtStatus() {
	return lichtStatus;
}

bool Taste::getTasteStatus() {
	return tasteStatus;
}

int Taste::getDruckZeit() {
	return startdruckZeit;
}



void Taste::taste_isr() {

    ThreadCtl(_NTO_TCTL_IO, 0);

    chanID = ChannelCreate(0);
    if (chanID < 0) {
        perror("[Taste] Could not create a channel!");
        exit(EXIT_FAILURE);
    }

    conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
    if (conID < 0) {
        perror("[Taste] Could not connect to channel!");
        exit(EXIT_FAILURE);
    }

    InterruptEnable();


    struct sigevent intr_event;
    SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_INTR_ON_PORT0, 0);
    interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
    if (interruptID < 0) {
        perror("[Taste] Interrupt was not able to be attached!");
        exit(EXIT_FAILURE);
    }


    uintptr_t port0BaseAddr = mmap_device_io(MAP_SIZE, GPIO_BANK_0);


    out32((uintptr_t) (port0BaseAddr + DEBOUCE_OFFSET), 0xffffffff);

    out8((uintptr_t) (port0BaseAddr + DEBOUCE_TIME_OFFSET), 0xff);


    out32((uintptr_t) port0BaseAddr + GPIO_IRQSTATUS_SET_1, taste);

    unsigned int intrID_reg;
    intrID_reg = in32((uintptr_t) (port0BaseAddr + GPIO_RISINGDETECT));
    intrID_reg |= taste;
    out32((uintptr_t) (port0BaseAddr + GPIO_RISINGDETECT), intrID_reg);

    intrID_reg = in32((uintptr_t) (port0BaseAddr + GPIO_FALLINGDETECT));
    intrID_reg |= taste;
    out32((uintptr_t) (port0BaseAddr + GPIO_FALLINGDETECT), intrID_reg);

    _pulse pulse_msg;


    for(;;) {
        int recvid = MsgReceivePulse(chanID, &pulse_msg, sizeof(_pulse), nullptr);

        if (recvid < 0) {
            perror("[Taste] MsgReceivePulse failed!");
            exit(EXIT_FAILURE);
        }

        if (recvid == 0) {
            uintptr_t gpioBase = mmap_device_io(MAP_SIZE, GPIO_BANK_0);
            unsigned int intrStatusReg = in32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1));


            out32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1), taste);
            InterruptUnmask(INTR_GPIO_PORT0, interruptID);

            if(pulse_msg.code == PULSE_INTR_ON_PORT0) {

                int intr_status = berechne(gpioBase, intrStatusReg);


                if (intr_status == letztesMal) {
                    continue;
                }
                letztesMal = intr_status;

                switch(intrStatusReg) {

                    case TypTaste::TASTE_START:
                        if(!intr_status) {
                           // timer->zeitSpeichern(startdruckZeit);
                            if(getDruckZeit() >= SERVICEMODE_START) {

                                if(dispatcher != nullptr){
                                    if(idd == 1) dispatcher->sendenNachricht(Signal::S_ST1_LG, 0);
                                    if(idd == 2) dispatcher->sendenNachricht(Signal::S_ST2_LG, 0);
                                }
                            } else {

                                if(dispatcher != nullptr) {
                                    if(idd == 1) dispatcher->sendenNachricht(Signal::S_ST1_G, 0);
                                    if(idd == 2) dispatcher->sendenNachricht(Signal::S_ST2_G, 0);
                                }
                            }
                        } else {
                           // timer->zeitStarten();
                        }
                        break;

                    case TypTaste::TASTE_STOP:
                        if(intr_status) {
                            if(dispatcher != nullptr) dispatcher->sendenNachricht(Signal::S_STP_G, 0);
                        }
                        break;

                    case TypTaste::TASTE_RESET:
                        if(!intr_status) {
                            if(dispatcher != nullptr) {
                                if(idd == 1) dispatcher->sendenNachricht(Signal::S_RT1_G, 0);
                                if(idd == 2) dispatcher->sendenNachricht(Signal::S_RT2_G, 0);
                            }
                        }
                        break;

                    case TypTaste::E_STOP:
                        if(intr_status) { // E-Stop
                            if(dispatcher != nullptr) {
                                if(idd == 1) dispatcher->sendenNachricht(Signal::S_E1_NG, 0);
                                if(idd == 2) dispatcher->sendenNachricht(Signal::S_E2_NG, 0);
                            }
                        } else { // E-Stop nicht gedrückt
                            if(dispatcher != nullptr) {
                                if(idd == 1) dispatcher->sendenNachricht(Signal::S_E1_G, 0);
                                if(idd == 2) dispatcher->sendenNachricht(Signal::S_E2_G, 0);
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }


    InterruptDetach(interruptID);
    ConnectDetach(conID);
    ChannelDestroy(chanID);
}

int Taste::berechne(uintptr_t gpioBase, int intrStatusReg) {

    unsigned int myMask = (uint32_t) taste;


    if (intrStatusReg & myMask) {

        int pin = 0;
        unsigned int tempMask = myMask;
        while (tempMask >>= 1) pin++;


        int current_level = (in32((uintptr_t)gpioBase + GPIO_DATAIN) >> pin) & 0x1;

        return current_level;
    }

    return -1;
}


void Taste::ledAn() {
	if(!getLichtStatus() && getTasteLicht() != TasteLicht::KEIN_LED) {
		ThreadCtl( _NTO_TCTL_IO, 0);
		uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
		out32((uintptr_t) (gpio_bank_2 + OFFSET_SET_REG), getTasteLicht());
		lichtStatus = true;
	}
}

/**
 *
 */
void Taste::ledAus() {
	if(getLichtStatus()) {
		ThreadCtl( _NTO_TCTL_IO, 0);
		uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
		out32((uintptr_t) (gpio_bank_2 + OFFSET_CRL_REG), getTasteLicht());
		lichtStatus = false;
	}
}

void Taste::initLED() {
	ThreadCtl(_NTO_TCTL_IO, 0);
	uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
	unsigned int oe = in32((uintptr_t) (gpio_bank_2 + GPIO_OE));
	oe &= ~licht;
	out32((uintptr_t) (gpio_bank_2 + GPIO_OE), oe);
	out32((uintptr_t) (gpio_bank_2 + OFFSET_CRL_REG), licht);
}

/**
 * Hilfmethode zur Bestimmung, ob die Taste eine Leuchte verfuegt.
 */
TasteLicht Taste::setTasteLicht() {
	switch(getTypTaste()) {
		case TypTaste::TASTE_START:
			return TasteLicht::LED_START;
		case TypTaste::TASTE_RESET:
			return TasteLicht::LED_RESET;
		default:
			return TasteLicht::KEIN_LED;
	}
}
