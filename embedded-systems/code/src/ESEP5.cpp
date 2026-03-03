/**
 * @file ESEP5.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Hauptprogramm
 *
 * Dient für den Systemstart.
 */

#include "Anlage/Anlage.hpp"

int main(int argc, char *argv[]) {

	std::string partnerName = ""; // Name der Anlage

    int maschineID = 1; // default

    if (argc > 1) { // sonst 1 oder 2
        maschineID = std::atoi(argv[1]);
    }

    if (argc > 2) { // sonst 1 oder 2
            partnerName = argv[2];
        }

    std::cout << "========================================" << std::endl;
    std::cout << "   SYSTEM INITIALISIERT - FESTO  " << maschineID << std::endl;
    std::cout << "========================================" << std::endl;


    Anlage anlage(maschineID,partnerName);

    anlage.starten();

    anlage.warten();

    return 0;
}

