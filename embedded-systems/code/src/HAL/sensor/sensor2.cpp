#include "sensor2.hpp"

Sensor2::Sensor2() {
    sensorZustand = Zustand::NICHT_UNTERBROCHEN;
    interruptID = -1;
    receivingRunning = false;
    chanID = -1;
    conID = -1;
}

Sensor2::Sensor2(SensorTyp given_sensorTyp) {
    sensorTyp = given_sensorTyp;
    sensorZustand = Zustand::NICHT_UNTERBROCHEN;
    interruptID = -1;
    receivingRunning = false;
    chanID = -1;
    conID = -1;
}

void Sensor2::setZustand(Zustand t) {
    sensorZustand = t;
}

void Sensor2::setSensor(SensorTyp giv) {
    //	sensorTyp |= giv;
}

int Sensor2::init_sens() {
    int val = 0;
    val |= SensorTyp::LS_EINLAUF | SensorTyp::LS_AUSLAUF;
    return val;
}

Zustand Sensor2::getZustand() {
    return sensorZustand;
}

void Sensor2::temp_isr() {
    ThreadCtl(_NTO_TCTL_IO, 0);

    chanID = ChannelCreate(0);
    if (chanID < 0) {
        perror("Could not create a channel!\n");
        exit(EXIT_FAILURE);
    }

    conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
    if (conID < 0) {
        perror("Could not connect to channel!");
        exit(EXIT_FAILURE);
    }

    InterruptEnable();
    struct sigevent intr_event;
    SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_INTR_ON_PORT0, 0);
    interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
    if (interruptID < 0) {
        perror("Interrupt was not able to be attached!");
        exit(EXIT_FAILURE);
    }

    uintptr_t port0BaseAddr = mmap_device_io(MAP_SIZE, GPIO_BANK_0);
    out32((uintptr_t) port0BaseAddr + GPIO_IRQSTATUS_SET_1, sensorTyp);

    unsigned int intrID_reg;
    intrID_reg = in32((uintptr_t)(port0BaseAddr + GPIO_RISINGDETECT));
    intrID_reg |= sensorTyp;
    out32((uintptr_t)(port0BaseAddr + GPIO_RISINGDETECT), intrID_reg);

    intrID_reg = in32((uintptr_t)(port0BaseAddr + GPIO_FALLINGDETECT));
    intrID_reg |= sensorTyp;
    out32((uintptr_t)(port0BaseAddr + GPIO_FALLINGDETECT), intrID_reg);

    _pulse pulse_msg;
    for (;;) {
        int recvid = MsgReceivePulse(chanID, &pulse_msg, sizeof(_pulse), nullptr);
        if (recvid < 0) {
            perror("MsgReceivePulse failed!");
            exit(EXIT_FAILURE);
        }
        if (recvid == 0) {
            uintptr_t gpioBase = mmap_device_io(MAP_SIZE, GPIO_BANK_0);
            unsigned int intrStatusReg = in32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1));

            out32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1), 0xffffffff);
            InterruptUnmask(INTR_GPIO_PORT0, interruptID);

            if (pulse_msg.code == PULSE_INTR_ON_PORT0) {
                int t = berechne(gpioBase, intrStatusReg);
                switch (intrStatusReg) {
                    case SensorTyp::LS_EINLAUF:
                        printf("Einlauf %d\n", t);
                        break;
                    case SensorTyp::METALL:
                        printf("Metall %d\n", t);
                        break;
                    case SensorTyp::HOEHENMESSER:
                        printf("Hoehe %d\n", t);
                        break;
                    case SensorTyp::LS_RUTSCHE:
                        printf("Rutsche %d\n", t);
                        break;
                    case SensorTyp::LS_AUSLAUF:
                        printf("Auslauf %d\n", t);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    if (InterruptDetach(interruptID) != EOK) {
        perror("Detaching interrupt failed!");
        exit(EXIT_FAILURE);
    }

    MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0);

    if (ConnectDetach(conID) != EOK) {
        perror("Detaching channel failed!");
        exit(EXIT_FAILURE);
    }

    if (ChannelDestroy(chanID) != EOK) {
        perror("Destroying channel failed!");
        exit(EXIT_FAILURE);
    }
}

int Sensor2::berechne(uintptr_t gpioBase, int intrStatusReg) {
    for (int pin = 0; pin < N_PINS; pin++) {
        unsigned int mask = (uint32_t) BIT_MASK(pin);
        if (intrStatusReg == mask) {
            int current_level = (in32((uintptr_t) gpioBase + GPIO_DATAIN) >> pin) & 0x1;
            printf("Interrupt on pin %d, now %d\n", pin, current_level); // FOR DEBUG
            setZustand(current_level ? Zustand::NICHT_UNTERBROCHEN : Zustand::UNTERBROCHEN);
            return current_level;
            //			if (current_level == 1) {
            //				setZustand(Zustand::NICHT_UNTERBROCHEN);
            //			} else {
            //				setZustand(Zustand::UNTERBROCHEN);
            //				//receivingRunning = false;
            //			}
        }
    }
    return -1;
}

/**
 *
 */
void Sensor2::receivingRoutine(int channelID) {
    ThreadCtl(_NTO_TCTL_IO, 0);

    _pulse pulse_msg;
    receivingRunning = true;
    //	while(receivingRunning) {
    for (;;) {
        int recvid = MsgReceivePulse(channelID, &pulse_msg, sizeof(_pulse), nullptr);
        if (recvid < 0) {
            perror("MsgReceivePulse failed!");
            exit(EXIT_FAILURE);
        }
        if (recvid == 0) {
            uintptr_t gpioBase = mmap_device_io(MAP_SIZE, GPIO_BANK_0);
            unsigned int intrStatusReg = in32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1));

            out32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1), 0xffffffff);
            InterruptUnmask(INTR_GPIO_PORT0, interruptID);

