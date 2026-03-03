/**
 * @file WerkstueckErkennung.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der WerkstueckErkennung.
 *
 * Enthält die Algorithmen zur Mittelwertbildung und Klassifizierung der Werkstücke
 * anhand der definierten Toleranzbereiche.
 */

#include "WerkstueckErkennung.hpp"
#include <numeric> // Für std::accumulate (Summe)

// Konstruktor mit individuellem Threshold
WerkstueckErkennung::WerkstueckErkennung(float thr)
: hoeheOhneStueck(0.0f),
  toleranz(0.6f),
  measuring(false),
  threshold(18.0f),
  holeCounter(0),
  MAX_HOLE_SIZE(80000)
{
	}


std::string WerkstueckErkennung::processValue(float value) {

	// Werkstueck wird detektiert
    if (!measuring && value < threshold) {
        measuring = true;
        holeCounter = 0;
        currentValues.clear();
        currentValues.push_back(value);
    }

    //Werkstueck wird gemesset
    else if (measuring) {

        if (value < threshold) { // Bohrung bei WS zaehlt nicht als Ende des WS
            holeCounter = 0;
            currentValues.push_back(value);
        }

        // Ende von WS oder keine WS vorhanden
        else {
            holeCounter++;

            currentValues.push_back(value);

            // Wenn die Bohrung zu lang ist, dann heisst es ,dass das keine echte Bohrung ist
            if (holeCounter > MAX_HOLE_SIZE) {
                measuring = false;

                while (!currentValues.empty() && currentValues.back() >= threshold) {
                                    currentValues.pop_back();
                                }


                if (currentValues.size() > 20) {
                    std::string typ = recognizeWerkstueck(currentValues);
                    currentValues.clear();
                    return typ;
                }
                currentValues.clear();
            }
        }
    }

    return "";
}


std::string WerkstueckErkennung::recognizeWerkstueck(const std::vector<float>& values) {

    if (values.empty()) return "Unbekannt";

   // Um zu ignorieren die Erste und Letzte Abweichungen, damit das nicht als Bohrung erkannt werden
    size_t startIndex = static_cast<size_t>(values.size() * ANFANG_OFFSET);
    size_t endIndex   = static_cast<size_t>(values.size() * ENDE_OFFSET);


    if (endIndex <= startIndex || endIndex > values.size()) {
        startIndex = 0;
        endIndex = values.size();
    }


       std::vector<float> safeZoneValues;
        try {

        	safeZoneValues.reserve(endIndex - startIndex);

        	for(size_t i = startIndex; i < endIndex; ++i) {
        	        safeZoneValues.push_back(values[i]);
        	    }
        } catch (...) {

            std::cout << "[Werkstueck Erkennung] Fehler bei vector safeZoneValues.\n";

        }

    float measuredStd  = calculateStd(values);
    float minSensorValue = *std::min_element(safeZoneValues.begin(), safeZoneValues.end());

   // Echte Hoehe in mm von WS
    float measuredHeight = 0.0f;
    if (hoeheOhneStueck > 0.1f) measuredHeight = hoeheOhneStueck - minSensorValue;
    else measuredHeight = 25.0f - minSensorValue;

    // Zaehler von Bohrungen
    int detectedHoles = 0;
    bool insideHole = false;
    int holeWidthCounter = 0; // Wie breit ist Bohrung?
    float holeThreshold = minSensorValue + 1.5f;


    for (size_t i = startIndex; i < endIndex; ++i) {
        float v = values[i];

       // Ist Bohrung?
        if (v > holeThreshold) {
            holeWidthCounter++;
        }
        else {

            if (holeWidthCounter >= 2) {
                 detectedHoles++;
            }

            holeWidthCounter = 0; // Reset
        }
    }


    if (holeWidthCounter >= 3) {
        detectedHoles++;
    }

    /*std::cout << "\n=== Werkstueck ===\n";
    std::cout << "Werte: Hoehe: " << measuredHeight << " | Std: " << measuredStd << " | Bohrung: " << detectedHoles << "\n";


    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << " TYP                    | Diff H  | Diff Std | Diff Holes | >> SCORE <<\n";*/

    float wHeight = 4.0f;
    float wStd    = 8.0f;
    float wHoles  = 20.0f;

    std::string bestLabel = "Unbekannt";
    float minTotalDist = 1000.0f;

    for (const auto& sample : dataset) {
        float dH = std::abs(measuredHeight - sample.height);
        float dS = std::abs(measuredStd - sample.std);
        float dHoles = std::abs((float)detectedHoles - (float)sample.holes);

        float totalDist = std::sqrt(
            (dH * wHeight) * (dH * wHeight) +
            (dS * wStd)    * (dS * wStd) +
            (dHoles * wHoles) * (dHoles * wHoles)
        );

       /* std::cout << " " << std::left << std::setw(20) << sample.label
                  << " | " << std::fixed << std::setprecision(2) << dH
                  << "    | " << std::setprecision(2) << dS
                  << "     | " << std::setprecision(0) << dHoles
                  << "          | " << std::setprecision(2) << totalDist;*/

        if (totalDist < minTotalDist) {
            minTotalDist = totalDist;
            bestLabel = sample.label;
           // std::cout << " <---";
        }
      //  std::cout << "\n";
    }

    if (minTotalDist > 5.0f) return "Unbekannt / Sonderfall";
    if (bestLabel == "Unbekannt / Sonderfall") return "Unbekannt / Sonderfall";

    return bestLabel;
}

void WerkstueckErkennung::kalibrieren(float currentSensorValue) {
    this->hoeheOhneStueck = currentSensorValue;
}

float WerkstueckErkennung::calculateMean(const std::vector<float>& data) {
    if (data.empty()) return 0.0f;
    float begin = static_cast<int>(data.size() * ANFANG_OFFSET);
    float ende = static_cast<int>(data.size() * ENDE_OFFSET);
    std::vector<int> data_mitte(data.begin() + begin, data.begin() + ende);

    long sum = 0;
    for (float v : data_mitte) {
        sum += v;
    }
    return static_cast<float>(sum) / data_mitte.size();
}

float WerkstueckErkennung::calculateStd(const std::vector<float>& data) {
    if (data.size() < 2) return 0.0f;
    float begin = static_cast<int>(data.size() * ANFANG_OFFSET);
    float ende = static_cast<int>(data.size() * ENDE_OFFSET);
    float mean = calculateMean(data);
    float acc = 0.0f;


   std::vector<float> data_mitte(data.begin() + begin, data.begin() + ende);

    for (int v : data_mitte) {
        float diff = static_cast<float>(v) - mean;
        acc += diff * diff;
    }

    return std::sqrt(acc / data_mitte.size());
}
