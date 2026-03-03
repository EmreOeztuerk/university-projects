package Sitzordnung;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.stream.Collectors;

/**
 * Die Klasse {@code Sitzordnung} modelliert eine Sitzverteilung von Schülern
 * in einem zweidimensionalen Raum (Tabelle) unter Berücksichtigung von sozialen
 * Beziehungen wie Wunschpartnern oder Verboten.
 *
 * <p>Die Sitzordnung lässt sich mutieren, kombinieren und auswerten
 * (Fitness und Gültigkeit).</p>
 *
 * @author Emre Öztürk
 * @date 08.08.25
 */
public class Sitzordnung {

    /** Zähler zur Vergabe eindeutiger IDs. */
    private static int idCounter;

    /** Eindeutige ID dieser Sitzordnung. */
    private final int id;

    /** Anzahl der Spalten im Sitzplan. */
    private final int col;

    /** Anzahl der Reihen im Sitzplan. */
    private final int row;

    /** Anzahl der Schüler. */
    private final int numSchueler;

    /** 2D-Array zur Darstellung der Sitzplätze (0 = leer, sonst Schüler-ID). */
    private int[][] sitzordnung;

    /** Beziehungen der Schüler untereinander. */
    private Beziehungen beziehungen;

    /** Belohnung für erwünschte Beziehungen. */
    private final int belohnung;

    /** Bestrafung für unerwünschte Beziehungen. */
    private final int bestrafung;

    /** Fitnesswert der Sitzordnung (je höher, desto besser). */
    private int fitness;

    /** Gibt an, ob alle Muss-Beziehungen erfüllt wurden. */
    private boolean valid;

    /** Der Typ der zu berücksichtigenden Nachbarschaft (z. B. 4er oder 8er Umgebung). */
    private static umgebungTyp umgebungTyp;

    /**
     * Konstruktor zur Erstellung einer neuen zufälligen Sitzordnung.
     *
     * @param col         Spaltenanzahl.
     * @param row         Reihenanzahl (muss gerade sein).
     * @param numSchueler Anzahl der Schüler.
     * @param belohnung   Belohnung pro erfüllter Wunschbeziehung.
     * @param bestrafung  Bestrafung pro unerfüllter Wunschvermeidung.
     * @param arySeed     Zufalls-Seed zur Generierung der Sitzordnung.
     * @param umgebungTyp Umgebungstyp (4er oder 8er Nachbarschaft).
     * @param beziehungen Typ der Beispielbeziehungen.
     */
    public Sitzordnung(int col, int row, int numSchueler, int belohnung, int bestrafung,
                       int arySeed, umgebungTyp umgebungTyp, int beziehungen) {
        if (col <= 0 || row <= 0 || numSchueler <= 0) {
            throw new IllegalArgumentException("Col, Row und NumSchueler muessen positiv und nicht 0 sein.");
        }
        if (row % 2 != 0) {
            throw new IllegalArgumentException("Row muss gerade sein.");
        }
        this.id = ++idCounter;
        this.col = col;
        this.row = row;
        this.numSchueler = numSchueler;
        this.valid = true;
        this.sitzordnung = new int[row][col];
        this.beziehungen = new Beziehungen(numSchueler);
        Beziehungen.addExampleRelations(this, beziehungen);
        this.belohnung = belohnung;
        this.bestrafung = bestrafung;
        Sitzordnung.umgebungTyp = umgebungTyp;
        this.initAry(arySeed);
        this.eval();
    }

    /**
     * Privater Konstruktor für Rekombination.
     *
     * @param col          Spaltenanzahl.
     * @param row          Reihenanzahl.
     * @param numSchueler  Schüleranzahl.
     * @param belohnung    Belohnungswert.
     * @param bestrafung   Bestrafungswert.
     * @param umgebungTyp  Umgebungstyp.
     * @param sitzordnung  Sitzmatrix.
     * @param beziehungen  Beziehungen.
     */
    private Sitzordnung(int col, int row, int numSchueler, int belohnung, int bestrafung,
                        umgebungTyp umgebungTyp, int[][] sitzordnung, Beziehungen beziehungen) {
        if (col <= 0 || row <= 0 || numSchueler <= 0) {
            throw new IllegalArgumentException("Col, Row und NumSchueler muessen positiv und nicht 0 sein.");
        }
        if (row % 2 != 0) {
            throw new IllegalArgumentException("Row muss gerade sein.");
        }
        this.id = ++idCounter;
        this.col = col;
        this.row = row;
        this.numSchueler = numSchueler;
        this.valid = true;
        this.sitzordnung = sitzordnung;
        this.beziehungen = beziehungen;
        this.belohnung = belohnung;
        this.bestrafung = bestrafung;
        Sitzordnung.umgebungTyp = umgebungTyp;
        this.eval();
    }

    /**
     * Bewertet die aktuelle Sitzordnung basierend auf den definierten Beziehungen.
     * Aktualisiert sowohl den Fitnesswert als auch die Validität.
     */
    private void eval() {
        this.fitness = 0;
        this.valid = true;
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                if(sitzordnung[i][j] != 0){
                    int currSchueler = sitzordnung[i][j];
                    int currNachbar;
                    List<Integer> mustHaveNeighbors = beziehungen.getPositivConstraintsOfSchueler(currSchueler);
                    if(umgebungTyp == Sitzordnung.umgebungTyp.VERTIKAL_HORIZONTAL){
                        //vorne
                        if(i > 0){
                            currNachbar = sitzordnung[i-1][j];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));
                            }

                        }
                        //rechts
                        if(j < col-1){
                            currNachbar = sitzordnung[i][j+1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //hinten
                        if(i < row-1){
                            currNachbar = sitzordnung[i+1][j];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //links
                        if(j > 0){
                            currNachbar = sitzordnung[i][j-1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        if(!mustHaveNeighbors.isEmpty()){
                            valid = false;
                        }
                    } else if(umgebungTyp == umgebungTyp.VERTIKAL_HORIZONTAL_DIAGONAL){
                        //vorne
                        if(i > 0){
                            currNachbar = sitzordnung[i-1][j];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));
                            }

                        }
                        //rechts
                        if(j < col-1){
                            currNachbar = sitzordnung[i][j+1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //hinten
                        if(i < row-1){
                            currNachbar = sitzordnung[i+1][j];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //links
                        if(j > 0){
                            currNachbar = sitzordnung[i][j-1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //vorneLinks
                        if(i > 0 && j > 0){
                            currNachbar = sitzordnung[i-1][j-1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //vorneRechts
                        if(i>0 && j<col-1){
                            currNachbar = sitzordnung[i-1][j+1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //hintenRechts
                        if(i<row-1 && j <col-1){
                            currNachbar = sitzordnung[i+1][j+1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        //hintenLinks
                        if(i<row-1&&j>0){
                            currNachbar = sitzordnung[i+1][j-1];
                            if(currNachbar != 0){
                                Beziehungen.beziehungTyp currBeziehung = beziehungen.getBeziehung(currSchueler, currNachbar);
                                evalBeziehungTyp(currBeziehung);
                                mustHaveNeighbors.remove(Integer.valueOf(currNachbar));}
                        }
                        if(!mustHaveNeighbors.isEmpty()){
                            valid = false;
                        }
                    }
                }
            }
        }
    }

    /**
     * Bewertet einen bestimmten Beziehungstyp und passt Fitness/Validität entsprechend an.
     *
     * @param type Der Beziehungstyp.
     */
    private void evalBeziehungTyp(Beziehungen.beziehungTyp type) {
        switch (type) {
            case KEINE_BEZIEHUNG -> {}
            case WOLLEN -> this.fitness += this.belohnung;
            case WOLLEN_NICHT -> this.fitness += this.bestrafung;
            case DUERFEN_NICHT -> this.valid = false;
        }
    }

    /**
     * Vertauscht zufällig zwei Plätze in der Sitzordnung.
     *
     * @param seed Zufalls-Seed für die Mutation.
     */
    public void mutate(int seed) {
        Random rand = new Random(seed);
        int x1 = rand.nextInt(row);
        int y1 = rand.nextInt(col);
        int x2 = rand.nextInt(row);
        int y2 = rand.nextInt(col);
        int a = this.sitzordnung[x1][y1];
        int b = this.sitzordnung[x2][y2];
        this.sitzordnung[x1][y1] = b;
        this.sitzordnung[x2][y2] = a;
        eval();
    }

    /**
     * Kombiniert zwei Eltern-Sitzordnungen zu einer neuen (Kind).
     *
     * @param parent1 Erste Eltern-Sitzordnung.
     * @param parent2 Zweite Eltern-Sitzordnung.
     * @return Neue Sitzordnung (Kind) mit gemischter Vererbung.
     */
    public static Sitzordnung recombine(Sitzordnung parent1, Sitzordnung parent2) {
        List<Integer> kind = new ArrayList<>();
        List<Integer> flatParent1 = Arrays.stream(parent1.sitzordnung)
                .flatMapToInt(Arrays::stream)
                .boxed()
                .collect(Collectors.toList());
        List<Integer> flatParent2 = Arrays.stream(parent2.sitzordnung)
                .flatMapToInt(Arrays::stream)
                .boxed()
                .collect(Collectors.toList());

        for (int i = 0; i < flatParent1.size(); i++) {
            //erste Haelfte parent 1 dominant
            if(i < flatParent1.size()/2) {
                if (!kind.contains(flatParent1.get(i)) && flatParent1.get(i) != 0) {
                    kind.add(flatParent1.get(i));
                } else if (!kind.contains(flatParent2.get(i)) && flatParent2.get(i) != 0) {
                    kind.add(flatParent2.get(i));
                } else {
                    kind.add(0);
                }
            } else { //zweite Haelfte parent 2 dominant
                if (!kind.contains(flatParent2.get(i)) && flatParent2.get(i) != 0) {
                    kind.add(flatParent2.get(i));
                } else if (!kind.contains(flatParent1.get(i)) && flatParent1.get(i) != 0) {
                    kind.add(flatParent1.get(i));
                } else {
                    kind.add(0);
                }
            }
        }
        //von hinten die uebrigen schueler auffuellen
        for (int i = 1; i < parent1.numSchueler+1; i++) {
            if(!kind.contains(i)){
                for(int j = parent1.row * parent1.col - 1; j >= 0; j--){
                    if(kind.get(j) == 0){
                        kind.set(j, i);
                        break;
                    }
                }
            }
        }
        int[][] kindSitzordnung = listeZu2DArray(kind,parent1.row,parent1.col);
        return new Sitzordnung(parent1.col,parent1.row,parent1.numSchueler,parent1.belohnung,parent1.bestrafung, umgebungTyp,kindSitzordnung,parent1.beziehungen);
    }

    /**
     * Wandelt eine flache Liste in eine 2D-Matrix um.
     *
     * @param liste Liste von Integern.
     * @param rows  Anzahl der Reihen.
     * @param cols  Anzahl der Spalten.
     * @return 2D-Array als Sitzplan.
     */
    public static int[][] listeZu2DArray(List<Integer> liste, int rows, int cols) {
        if (liste.size() > rows * cols) {
            throw new IllegalArgumentException("Listengröße passt nicht zur gewünschten Matrixgröße.");
        }
        int[][] array2D = new int[rows][cols];
        for (int i = 0; i < liste.size(); i++) {
            int row = i / cols;
            int col = i % cols;
            array2D[row][col] = liste.get(i);
        }
        return array2D;
    }

    /**
     * Initialisiert die Sitzordnung zufällig.
     *
     * @param seed Zufalls-Seed.
     */
    private void initAry(int seed) {
        List<Integer> list = new ArrayList<>(Collections.nCopies(this.col * this.row, 0));
        for (int i = 0; i < this.numSchueler; i++) {
            list.set(i, i + 1);
        }
        Collections.shuffle(list, new Random(seed));
        this.sitzordnung = listeZu2DArray(list, this.row, this.col);
    }

    /**
     * Gibt den Fitnesswert dieser Sitzordnung zurück.
     *
     * @return Fitnesswert.
     */
    public int getFitness() {
        return fitness;
    }

    /**
     * Gibt die Anzahl der Schüler zurück.
     *
     * @return Anzahl der Schüler.
     */
    public int getNumSchueler() {
        return numSchueler;
    }

    /**
     * Setzt eine neue Beziehungen-Instanz und bewertet die Sitzordnung neu.
     *
     * @param beziehungen Neue Beziehungen.
     */
    public void setBeziehungTyp(Beziehungen beziehungen) {
        this.beziehungen = beziehungen;
        eval();
    }

    /**
     * Gibt an, ob diese Sitzordnung alle harten Nebenbedingungen erfuellt.
     *
     * @return {@code true}, wenn die Sitzordnung gueltig ist, sonst {@code false}
     */
    public boolean isValid() {
        return valid;
    }

    /**
     * Setzt den Fitnesswert explizit, z. B. fuer Strafwerte bei ungueltigen Individuen.
     *
     * @param fitness neuer Fitnesswert
     */
    public void setFitness(int fitness) {
        this.fitness = fitness;
    }

    /**
     * Enum zur Definition der zu bewertenden Sitznachbarschaften.
     */
    public enum umgebungTyp {
        /** Nur direkte Nachbarn (oben, unten, links, rechts). */
        VERTIKAL_HORIZONTAL,

        /** Alle 8 Nachbarn (auch diagonal). */
        VERTIKAL_HORIZONTAL_DIAGONAL
    }
}
