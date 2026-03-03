# ESEP - Sortieranlage Steuerungssoftware (WiSe 2025 - Team 1.3)

Dieses Repository enthält die Steuerungssoftware für ein automatisiertes Sortierungssystem, bestehend aus zwei Förderbandanlagen. Das System wurde im Rahmen des Embedded Systems Engineering Praktikums (ESEP) an der HAW Hamburg entwickelt.

## 📋 Projektübersicht

Das System steuert zwei unabhängige Festo-Anlagen, die über **QNX Qnet** miteinander kommunizieren. Ziel ist die korrekte Klassifizierung, Weiterleitung und Aussortierung von Werkstücken basierend auf einer definierten Soll-Reihenfolge.

**Technologie-Stack:**
* **OS:** QNX Neutrino RTOS
* **Sprache:** C++
* **Kommunikation:** Qnet (Asynchroner Message-Passing)

## 👥 Team 1.3

| Name | Rolle / Fokus |
| :--- | :--- |
| **Emre Öztürk** | Product Owner, Entwickler |
| **Alaa Al-Sowmali** |  Entwickler | 
| **Marta Muñoz Merino** | Entwickler | 
| **Oleksandr Maksymov** | Entwickler | 

## 🏗️ Software-Architektur

Die Software ist strikt in drei Schichten unterteilt, um Hardware-Unabhängigkeit und Testbarkeit zu gewährleisten:

1.  **Logik (FSM):** Enthält die Geschäftslogik in Form von endlichen Zustandsautomaten (Finite State Machines). Beide Anlagen nutzen dieselbe Logik-Struktur (Master-Master-Prinzip).
2.  **Dispatcher:** Fungiert als zentrale Vermittlungsschicht. Er wandelt Hardware-Interrupts und Qnet-Nachrichten in Events um und leitet sie an die FSMs weiter.
3.  **HAL (Hardware Abstraction Layer):** Kapselt den direkten Zugriff auf Sensoren, Aktoren und GPIOs.

### Kommunikation
Die Anlagen kommunizieren über **Qnet**. Dabei registriert sich jede Anlage als Server (GNS) und sendet asynchrone Pulse an die Partneranlage (z.B. für Werkstückübergabe oder E-Stop-Signale).

## ⚙️ Funktionalität & Sortierlogik

Das System sortiert Werkstücke basierend auf Höhe, Bohrung und Metallkern.

**Definierte Soll-Reihenfolge:**
1.  🟢 Hohes Werkstück mit Bohrung (HB)
2.  🔵 Hohes Werkstück (H)
3.  ⚪ Flaches Werkstück (F)
4.  🔴 Hohes Werkstück mit Bohrung und Metall (HBM)

Werkstücke, die dieser Reihenfolge nicht entsprechen oder als defekt/unbekannt erkannt werden, werden über die Weiche oder den Auswerfer aussortiert.

### Sicherheitsfeatures
* **E-Stop:** Ein Not-Halt an einer Anlage stoppt sofort beide Bänder. Ein Neustart erfordert einen Reset an beiden Bedienpulten.
* **Verbindungsüberwachung:** Ein Heartbeat-Mechanismus überwacht die Qnet-Verbindung. Bei Verlust geht die Anlage in den sicheren Zustand.

## 🚀 Installation & Ausführung

1.  **Klonen des Repositories:**
    ```bash
    git clone <repository-url>
    ```
2.  **Build:**
    Das Projekt ist für die QNX Momentics IDE ausgelegt.
    * Importiere das Projekt in den Workspace.
    * Build Targets: armv7le` (Hardware).
3.  **Starten:**
    * Die Software muss auf beiden Anlagen gestartet werden.

## ⚠️ Bekannte Probleme (Technical Debt)

Zum Zeitpunkt der Version 1.4 bestehen folgende Einschränkungen:
* **Werkstückerkennung:** Funktioniert zu ca. 95%.
* **LED-Verhalten:** Start- und Reset-LEDs entsprechen teilweise noch nicht vollständig der Anforderungsspezifikation.
* **Gleichzeitigkeit:** Die Behandlung von zwei Werkstücken in derselben logischen Region kann zu Fehlern führen.

## 📂 Dokumentation

Detaillierte Informationen finden sich im Ordner `doc/`:
* **RDD:** Requirements, Design und Test Documentation (PDF)
* **FSMs:** Diagramme der Zustandsautomaten

---
*Stand: 20. Januar 2026 (Version 1.4)*