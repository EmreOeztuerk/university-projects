package Sitzordnung;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Die Klasse {@code Beziehungen} verwaltet die Beziehungen zwischen Schülern
 * in einer Sitzordnung. Es werden verschiedene Beziehungstypen unterstützt,
 * wie z. B. Wünsche, Verbote und zwingende Anforderungen bezüglich Sitznachbarn.
 *
 * @author Emre Öztürk
 * @date 08.08.25
 */
public class Beziehungen {
    /**
     * Enthält die direkten Beziehungen zwischen Schülerpaaren.
     */
    private Map<Integer, Map<Integer, beziehungTyp>> beziehungen;

    /**
     * Enthält zwingende Sitznachbarschaften (MUESSEN) pro Schüler.
     */
    private Map<Integer, List<Integer>> positivConstraints;

    /**
     * Enthält verbotene Sitznachbarschaften (DUERFEN_NICHT) pro Schüler.
     */
    private Map<Integer, List<Integer>> negativConstraints;

    /**
     * Konstruktor zur Initialisierung der Datenstrukturen für eine gegebene Anzahl an Schülern.
     *
     * @param numSchueler Die Anzahl der Schüler (1-basiert, daher wird intern um 1 erhöht).
     */
    public Beziehungen(int numSchueler) {
        numSchueler++;
        beziehungen = new HashMap<>(numSchueler);
        positivConstraints = new HashMap<>(numSchueler);
        negativConstraints = new HashMap<>(numSchueler);
        for (int i = 0; i < numSchueler; i++) {
            beziehungen.put(i, new HashMap<>(numSchueler));
            positivConstraints.put(i, new ArrayList<>());
            negativConstraints.put(i, new ArrayList<>());
        }
    }

    /**
     * Fügt eine Beziehung zwischen zwei Schülern hinzu.
     *
     * @param schueler1 Der erste Schüler (Index).
     * @param schueler2 Der zweite Schüler (Index).
     * @param type      Der Typ der Beziehung.
     */
    public void addBeziehung(int schueler1, int schueler2, beziehungTyp type) {
        if (type == beziehungTyp.WOLLEN || type == beziehungTyp.WOLLEN_NICHT) {
            beziehungen.get(schueler1).put(schueler2, type);
        } else if (type == beziehungTyp.DUERFEN_NICHT) {
            negativConstraints.get(schueler1).add(schueler2);
        } else if (type == beziehungTyp.MUESSEN) {
            positivConstraints.get(schueler1).add(schueler2);
        }
    }

    /**
     * Gibt die Beziehung zwischen zwei Schülern zurück.
     *
     * @param schueler1 Der erste Schüler (Index).
     * @param schueler2 Der zweite Schüler (Index).
     * @return Der Typ der Beziehung, oder {@code KEINE_BEZIEHUNG}, wenn keine definiert ist.
     */
    public beziehungTyp getBeziehung(int schueler1, int schueler2) {
        if (beziehungen.get(schueler1).containsKey(schueler2)) {
            return beziehungen.get(schueler1).get(schueler2);
        }
        if (positivConstraints.get(schueler1).contains(schueler2)) {
            return beziehungTyp.MUESSEN;
        }
        if (negativConstraints.get(schueler1).contains(schueler2)) {
            return beziehungTyp.DUERFEN_NICHT;
        }
        return beziehungTyp.KEINE_BEZIEHUNG;
    }

    /**
     * Gibt die Liste der zwingenden Sitznachbarn (positiven Constraints) eines Schülers zurück.
     *
     * @param schueler Der Schüler (Index).
     * @return Liste der Schüler, neben denen dieser Schüler sitzen muss.
     */
    public List<Integer> getPositivConstraintsOfSchueler(int schueler) {
        return positivConstraints.get(schueler);
    }

    /**
     * Fügt Beispielbeziehungen zu einer Sitzordnung hinzu, abhängig vom gewählten Typ.
     * Diese Methode dient der Demonstration oder Testzwecken.
     *
     * @param sitzordnung Die Sitzordnung, zu der die Beziehungen hinzugefügt werden sollen.
     * @param type        Der Typ der Beispieldaten (1–3).
     */
    public static void addExampleRelations(Sitzordnung sitzordnung, int type) {
        Beziehungen beziehungen = new Beziehungen(sitzordnung.getNumSchueler());

        // Beispieltyp 1: Definiert verschiedene Beziehungen auf Basis von Modulo-Regeln.
        if (type == 1) {
            for (int i = 1; i < sitzordnung.getNumSchueler() + 1; i++) {
                beziehungen.addBeziehung(i, i + 1, beziehungTyp.WOLLEN);
                if (i % 2 == 0) {
                    beziehungen.addBeziehung(i, i - 1, beziehungTyp.WOLLEN_NICHT);
                }
                if (i % 3 == 0) {
                    beziehungen.addBeziehung(i, i + 1, beziehungTyp.DUERFEN_NICHT);
                }
                if (i % 5 == 0) {
                    beziehungen.addBeziehung(i, i - 1, beziehungTyp.MUESSEN);
                }
            }
        }

        // Beispiel2: echtes Beispiel, basiert auf echter Klasse, 24 Schueler, row=4, col=6
        if (type == 2) {
            beziehungen.addBeziehung(1,10, beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(1,17,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(1,10,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(2,6,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(2,7,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(2,6,beziehungTyp.MUESSEN);
            beziehungen.addBeziehung(3,21,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(4,10,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(4,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(5,9,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(5,13,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(5,16,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(5,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(5,16,beziehungTyp.MUESSEN);
            beziehungen.addBeziehung(5,12,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(6,12,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(6,7,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(6,2,beziehungTyp.MUESSEN);
            beziehungen.addBeziehung(6,8,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(6,10,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(6,17,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(6,20,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(7,17,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(7,8,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(7,10,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(7,20,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(7,23,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(8,18,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(8,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(8,5,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(8,9,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(8,14,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(8,16,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(8,15,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(9,18,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(9,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(10,4,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(10,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(11,18,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(11,8,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(12,6,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(12,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(12,15,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(12,6,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(12,2,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(13,20,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(13,17,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(14,10,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(14,15,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(15,3,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(15,21,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(15,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,22,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(17,22,beziehungTyp.MUESSEN);
            beziehungen.addBeziehung(17,7,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,8,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,10,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,20,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,23,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(17,11,beziehungTyp.DUERFEN_NICHT);
            beziehungen.addBeziehung(18,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(18,16,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(18,4,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(19,5,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(19,11,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(19,18,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(20,16,beziehungTyp.WOLLEN_NICHT);
            beziehungen.addBeziehung(21,3,beziehungTyp.WOLLEN);
            beziehungen.addBeziehung(22,17,beziehungTyp.WOLLEN);
        }
        if (type == 3) {
            // Drittes Beispiel
        }

        sitzordnung.setBeziehungTyp(beziehungen);
    }

    /**
     * Enum zur Darstellung der verschiedenen Beziehungstypen zwischen Schülern.
     */
    public enum beziehungTyp {
        /**
         * Schüler möchte neben einem anderen Schüler sitzen.
         */
        WOLLEN,

        /**
         * Schüler möchte nicht neben einem anderen Schüler sitzen.
         */
        WOLLEN_NICHT,

        /**
         * Schüler muss neben einem bestimmten anderen Schüler sitzen.
         */
        MUESSEN,

        /**
         * Schüler darf nicht neben einem bestimmten anderen Schüler sitzen.
         */
        DUERFEN_NICHT,

        /**
         * Es liegt keine spezifische Beziehung vor.
         */
        KEINE_BEZIEHUNG
    }
}
