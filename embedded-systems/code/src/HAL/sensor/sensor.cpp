/**
 * @file sensor.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Sensor-Klasse.
 *
 * Enthält die Logik für die Initialisierung der Interrupts, Debouncing und
 * das Versenden von Nachrichten an den Dispatcher bei Sensor-Events.
 */

Sensor::Sensor(SensorTyp given_sensorTyp, IDispatcher *given_disp, int id)
    : idd(id), // Initialisierung
      sensorTyp(given_sensorTyp),
      sensorZustand(Zustand::NICHT_UNTERBROCHEN),
      dispatcher(given_disp),
      interruptID(-1),
      receivingRunning(false),
      chanID(-1),
      conID(-1) {
}

void Sensor::setZustand(Zustand t) {
    sensorZustand = t;
}

Zustand Sensor::getZustand() {
    return sensorZustand;
}

void Sensor::sensor_isr() {
    ThreadCtl(_NTO_TCTL_IO, 0);


    chanID = ChannelCreate(0);
    if (chanID < 0) {
        perror("[Sensor] Could not create a channel!");
        exit(EXIT_FAILURE);
    }

    conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
    if (conID < 0) {
        perror("[Sensor] Could not connect to channel!");
        exit(EXIT_FAILURE);
    }

    InterruptEnable();

    struct sigevent intr_event;
    SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_INTR_ON_PORT0, 0);
    interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);

    if (interruptID < 0) {
        perror("[Sensor] Interrupt was not able to be attached!");
        exit(EXIT_FAILURE);
    }

    // --- DEBOUNCING ---
    uintptr_t port0BaseAddr = mmap_device_io(MAP_SIZE, GPIO_BANK_0);

    out32((uintptr_t)(port0BaseAddr + DEBOUCE_OFFSET), 0xffffffff);
    out8((uintptr_t)(port0BaseAddr + DEBOUCE_TIME_OFFSET), 0xff);


    out32((uintptr_t) port0BaseAddr + GPIO_IRQSTATUS_SET_1, sensorTyp);

    unsigned int intrID_reg;
    intrID_reg = in32((uintptr_t)(port0BaseAddr + GPIO_RISINGDETECT));
    intrID_reg |= sensorTyp;
    out32((uintptr_t)(port0BaseAddr + GPIO_RISINGDETECT), intrID_reg);

    intrID_reg = in32((uintptr_t)(port0BaseAddr + GPIO_FALLINGDETECT));
    intrID_reg |= sensorTyp;
    out32((uintptr_t)(port0BaseAddr + GPIO_FALLINGDETECT), intrID_reg);

    // 1. Timer außerhalb der Schleife definieren!
    // Er muss die gesamte Lebensdauer des Threads existieren.
    Timer *rutscheTimer = nullptr;
    if (sensorTyp == SensorTyp::LS_RUTSCHE) {
        // Erstellt den Timer, der T_PULSE_RUTSCHE_VOLL an chanID sendet
        rutscheTimer = new Timer(chanID, T_PULSE_RUTSCHE_VOLL, 0);
    }

    _pulse pulse_msg;
    for (;;) {
        int recvid = MsgReceivePulse(chanID, &pulse_msg, sizeof(_pulse), nullptr);

        if (recvid == 0) {
            // 2. Prüfen: Ist die Nachricht vom Timer gekommen?
            if (pulse_msg.code == T_PULSE_RUTSCHE_VOLL) {
                std::cout << "[Sensor] TIMER ABGELAUFEN -> Rutsche voll!" << std::endl;
                if (idd == 1) dispatcher->sendenNachricht(Signal::EC_RTSCH1_VOLL, 0);
                if (idd == 2) dispatcher->sendenNachricht(Signal::EC_RTSCH2_VOLL, 0);
                continue; // Wichtig: Springe zum nächsten Schleifendurchlauf
            }

            uintptr_t gpioBase = mmap_device_io(MAP_SIZE, GPIO_BANK_0);
            unsigned int intrStatusReg = in32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1));


            out32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1), sensorTyp);
            InterruptUnmask(INTR_GPIO_PORT0, interruptID);

            if (pulse_msg.code == PULSE_INTR_ON_PORT0) {
                int intr_status = berechne(gpioBase, intrStatusReg);


                if (intr_status == letzesMal || intr_status == -1) {
                    continue;
                }
                letzesMal = intr_status;


                switch (intrStatusReg) {
                    case SensorTyp::LS_EINLAUF:
                        if (!dispatcher) break;
                        if (intr_status) {
                            //(NUB)
                            if (idd == 1) dispatcher->sendenNachricht(Signal::S_LSA1_NUB, 0);
                            if (idd == 2) dispatcher->sendenNachricht(Signal::S_LSA2_NUB, 0);
                        } else {
                            // (UB)
                            if (idd == 1) dispatcher->sendenNachricht(Signal::S_LSA1_UB, 0);
                            if (idd == 2) dispatcher->sendenNachricht(Signal::S_LSA2_UB, 0);
                        }
                        break;

                    case SensorTyp::HOEHENMESSER:
                        // if(!dispatcher) break;
                        //if(intr_status) dispatcher->sendenNachricht(Signal::S_LSH_NUB, 0);
                        //else dispatcher->sendenNachricht(Signal::S_LSH_UB, 0);
                        break;

                    case SensorTyp::METALL:
                        if (!dispatcher) break;
                        if (intr_status) dispatcher->sendenNachricht(Signal::S_METAL_AN, 0);
                        break;

                    case SensorTyp::LS_SORTIERER:
                        if (!dispatcher) break;
                        if (intr_status) dispatcher->sendenNachricht(Signal::S_LSW_NUB, 0);
                        else dispatcher->sendenNachricht(Signal::S_LSW_UB, 0);
                        break;

                    case SensorTyp::LS_RUTSCHE:
                        if (!dispatcher) break;
                        if (intr_status) {
                            if (rutscheTimer) rutscheTimer->stoppen();
                            if (idd == 1) {
                                dispatcher->sendenNachricht(Signal::S_LSR1_NUB, 0);
                                dispatcher->sendenNachricht(Signal::CMD_RTSCH1_NV, 0);
                            }
                            if (idd == 2) {
                                dispatcher->sendenNachricht(Signal::S_LSR2_NUB, 0);
                                dispatcher->sendenNachricht(Signal::CMD_RTSCH2_NV, 0);
                            }
                        } else {
                            if (rutscheTimer)rutscheTimer->starten(3000, false);
                            if (idd == 1) dispatcher->sendenNachricht(Signal::S_LSR1_UB, 0);
                            if (idd == 2) dispatcher->sendenNachricht(Signal::S_LSR2_UB, 0);
                        }
                        break;

                    case SensorTyp::LS_AUSLAUF:
                        if (!dispatcher) break;
                        if (intr_status) {
                            if (idd == 1) dispatcher->sendenNachricht(Signal::S_LSE1_NUB, 0);
                            if (idd == 2) dispatcher->sendenNachricht(Signal::S_LSE2_NUB, 0);
                        } else {
                            if (idd == 1) dispatcher->sendenNachricht(Signal::S_LSE1_UB, 0);
                            if (idd == 2) dispatcher->sendenNachricht(Signal::S_LSE2_UB, 0);
                        }
                        break;
                }
            }
        }
    }
}

int Sensor::berechne(uintptr_t gpioBase, int intrStatusReg) {
    unsigned int myMask = (uint32_t) sensorTyp;


    if (intrStatusReg & myMask) {
        int pin = 0;
        unsigned int tempMask = myMask;
        while (tempMask >>= 1) pin++;


        int current_level = (in32((uintptr_t) gpioBase + GPIO_DATAIN) >> pin) & 0x1;


        setZustand(current_level ? Zustand::NICHT_UNTERBROCHEN : Zustand::UNTERBROCHEN);

        return current_level;
    }

    return -1;
}


void Sensor::tempUB() {
    switch (sensorTyp) {
        case SensorTyp::LS_EINLAUF:
            //			dispatcher->sendenNachricht(Signal::S_LSA1_UB, 0);
            printf("S_LSA1_UB\n");
            break;

        //		case SensorTyp::HOEHENMESSER:
        //			dispatcher->sendenNachricht(Signal::S_LSH_UB, 0);
        //			break;


        case SensorTyp::LS_SORTIERER:
            //			dispatcher->sendenNachricht(Signal::S_LSW_UB, 0);
            printf("S_LSW_UB\n");
            break;

        case SensorTyp::LS_RUTSCHE:
            //			dispatcher->sendenNachricht(Signal::S_LSR1_UB, 0);
            printf("S_LSR1_UB\n");
            break;

        case SensorTyp::LS_AUSLAUF:
            //			dispatcher->sendenNachricht(Signal::S_LSE1_UB, 0);
            printf("S_LSE1_UB\n");
            break;

        default:
            break;
    }
}

void Sensor::tempNUB() {
    switch (sensorTyp) {
        case SensorTyp::LS_EINLAUF:
            //			dispatcher->sendenNachricht(Signal::S_LSA1_NUB, 0);
            printf("S_LSA1_NUB\n");
            break;

        //		case SensorTyp::HOEHENMESSER:
        //			dispatcher->sendenNachricht(Signal::S_LSH_UB, 0);
        //			break;
        case SensorTyp::METALL:
            //			dispatcher->sendenNachricht(Signal::S_METAL_AN, 0);
            printf("S_METAL_AN\n");
            break;

        case SensorTyp::LS_SORTIERER:
            //			dispatcher->sendenNachricht(Signal::S_LSW_NUB, 0);
            printf("S_LSW_NUB\n");
            break;

        case SensorTyp::LS_RUTSCHE:
            //			dispatcher->sendenNachricht(Signal::S_LSR1_NUB, 0);
            printf("S_LSR1_NUB\n");
            break;

        case SensorTyp::LS_AUSLAUF:
            //			dispatcher->sendenNachricht(Signal::S_LSE1_NUB, 0);
            printf("S_LSE1_NUB\n");
            break;

        default:
            break;
    }
}