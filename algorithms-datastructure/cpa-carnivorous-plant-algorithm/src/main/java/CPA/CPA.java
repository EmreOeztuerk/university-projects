package CPA;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

import Sitzordnung.Sitzordnung;

/**
 * Implementiert den Carnivorous Plant Algorithm (CPA) zur Optimierung der Sitzordnung.
 * Die Klasse koordiniert eine Population von Sitzordnungen und verbessert sie iterativ durch
 * Rekombination (Crossover) und Mutation.
 *
 * @author Emre Öztürk
 * @date 08.08.25
 */
public class CPA {
    static int idCounter;
    private int id;
    private final int popGroesse;                       // Populationsgröße
    private final int anzahlPflanzen;                   // Anzahl Elite-Individuen (Pflanzen)
    private final int anzahlBeute;                      // Anzahl Beute-Individuen
    private final int maxIterationen;                   // Maximale Anzahl der Generationen
    private final int spalte, reihe, numSchueler;       // Sitzordnung: Spalten, Reihen, Anzahl Schüler
    private final int belohnung, bestrafung;            // Werte für Bewertung der Sitzordnung
    private final Sitzordnung.umgebungTyp umgebungTyp;  // Umgebungstyp (z.B. Nachbarschaftslogik)
    private final Random random;                        // Zufallszahlengenerator
    private List<Sitzordnung> population;               // Aktuelle Population
    double mutationsrate;
    double reproduktionsrate;
    double wachstumsrate;
    boolean printLog;
    int seed;
    int beziehungenBsp;

    /**
     * Konstruktor für den CPA-Algorithmus.
     *
     * @param popGroesse      Anzahl Individuen pro Generation
     * @param anzahlPflanzen  Anzahl "Pflanzen" (Elite, beste Individuen)
     * @param spalte             Anzahl der Sitzplätze pro Reihe (Spalten)
     * @param reihe             Anzahl der Reihen
     * @param numSchueler     Anzahl der zu platzierenden Schüler
     * @param belohnung       Wert für erfüllte positive Beziehungen
     * @param bestrafung      Wert für verletzte negative Beziehungen
     * @param umgebungTyp     Art der Nachbarschaftsauswertung
     * @param maxIterationen  Maximale Generationen
     */
    public CPA(int popGroesse, int anzahlPflanzen,
               int spalte, int reihe, int numSchueler,
               int belohnung, int bestrafung,
               Sitzordnung.umgebungTyp umgebungTyp,
               int maxIterationen, int randomSeed,
               double mutationsrate, double reproduktionsRate,
               double wachstumsrate, boolean printDoc,
               int beziehungenBsp
    ) {
        this.id = idCounter++;
        this.popGroesse = popGroesse;
        this.anzahlPflanzen = anzahlPflanzen;
        this.anzahlBeute = popGroesse - anzahlPflanzen;
        this.maxIterationen = maxIterationen;
        this.spalte = spalte;
        this.reihe = reihe;
        this.numSchueler = numSchueler;
        this.belohnung = belohnung;
        this.bestrafung = bestrafung;
        this.umgebungTyp = umgebungTyp;
        this.random = new Random(randomSeed);
        this.population = new ArrayList<>();
        this.mutationsrate = mutationsrate;
        this.reproduktionsrate = reproduktionsRate;
        this.wachstumsrate = wachstumsrate;
        this.printLog = printDoc;
        this.seed = randomSeed;
        this.beziehungenBsp = beziehungenBsp;
    }

    /**
     * Startet den CPA-Algorithmus und gibt die beste gefundene Sitzordnung zurück.
     *
     * @return Die Sitzordnung mit der besten Fitness nach allen Generationen.
     */
    public Sitzordnung fuehreAus() {
        if(printLog) {
            //Dateizeug
            try {
                File file = new File("./data/"+ reihe + "_" + spalte + "_" + numSchueler + "_" + belohnung + "_" + bestrafung+ "_TESTDATA.csv");
                if (file.createNewFile()) {
                    System.out.println("File created: " + file.getName());
                    try {
                        FileWriter myWriter = new FileWriter("./data/"+ reihe + "_" + spalte + "_" + numSchueler + "_" + belohnung + "_" + bestrafung +"_TESTDATA.csv",true);
                        myWriter.write("popSize,pflanzeCount,beuteCount,maxIterationen,col,row,numSchueler,belohnung,bestrafung,umgebungTyp,seed,mutationRate,reproductionRate,growthRate,bestFitness,durchschnitt,iteration,id\n");
                        myWriter.close();
                    } catch (IOException e) {
                        System.out.println("An error occurred.");
                        e.printStackTrace();
                    }
                } else {
                    System.out.println("File already exists.");
                }
            } catch (IOException e) {
                System.out.println("An error occurred.");
                e.printStackTrace();
            }

            //Dateizeug ende
        }
        int neuePflanzenCount;
        int neueBeuteCount;
        int neueRekombinationenCount;

        System.out.println("Initialisiere Startpopulation ...");
        // === Initialisierung: Startpopulation mit zufälligen Sitzordnungen ===
        population = new ArrayList<>();
        for (int i = 0; i < popGroesse; i++) {
            int seed = random.nextInt();
            Sitzordnung ind = new Sitzordnung(spalte, reihe, numSchueler, belohnung, bestrafung, seed, umgebungTyp, beziehungenBsp);
            population.add(ind);
        }
        System.out.println("Initialisierung abgeschlossen. Startpopulation erzeugt (" + popGroesse + " Individuen)");

        // === Iterativer Optimierungsprozess ===
        for (int iter = 1; iter <= maxIterationen; iter++) {
            // 1. Bewertung und Sortierung (beste Fitness zuerst)
            population.sort(Comparator.comparingInt(Sitzordnung::getFitness));


            // Listen für neue Individuen in dieser Generation
            List<Sitzordnung> neuePflanzen = new ArrayList<>();     // Nachkommen durch Rekombination
            List<Sitzordnung> neueBeute = new ArrayList<>();        // Nachkommen durch Mutation

            // 2. Wachstumsphase: Neue Pflanzen durch Rekombination (Pflanze dominiert)
            neuePflanzenCount = 0;
            for (int i = 0; i < anzahlPflanzen; i++) {
                if (random.nextDouble(0, 1) >= wachstumsrate) {

                Sitzordnung pflanze = population.get(i);
                int beuteIndex = anzahlPflanzen + random.nextInt(anzahlBeute);
                Sitzordnung beute = population.get(beuteIndex);
                Sitzordnung kind = Sitzordnung.recombine(pflanze, beute);
                if(kind.getFitness() > pflanze.getFitness()) {neuePflanzen.add(kind);
                    neuePflanzenCount++;} else {
                    neuePflanzen.add(kind);
                }
                } else {
                    neuePflanzen.add(population.get(i));
                }
            }
            neuePflanzen.sort(Comparator.comparingInt(Sitzordnung::getFitness));

            // 3. Beute wächst durch Mutation (Diversität erhalten)
            neueBeuteCount = 0;
            for (int j = 0; j < anzahlBeute; j++) {
                if (random.nextDouble(0, 1) >= mutationsrate) {
                    Sitzordnung beuteInd = population.get(anzahlPflanzen + j);
                    beuteInd.mutate(random.nextInt());
                    neueBeute.add(beuteInd);
                    neueBeuteCount++;
                } else {
                    neueBeute.add(population.get(anzahlPflanzen + j));
                }
            }

            // 4. Reproduktion: Beste Pflanze kombiniert sich mit anderen Pflanzen
            neueRekombinationenCount = 0;
            if (anzahlPflanzen > 1) {
                Sitzordnung bestePflanze = neuePflanzen.get(0);
                for (int k = 1; k < anzahlPflanzen; k++) {
                    if (random.nextDouble(0, 1) >= reproduktionsrate) {
                        Sitzordnung anderePflanze = population.get(k);
                        Sitzordnung kind = Sitzordnung.recombine(bestePflanze, anderePflanze);
                        if(kind.getFitness() > bestePflanze.getFitness()) {
                            neuePflanzen.set(k, kind);
                            neueRekombinationenCount++;
                        }
                    }
                }
            }

            // 5. Neue Population für nächste Generation selektieren
            List<Sitzordnung> gesamtPopulation = new ArrayList<>();
            gesamtPopulation.addAll(neuePflanzen);
            gesamtPopulation.addAll(neueBeute);

            // Ungültige Sitzordnungen bestrafen (maximale Fitness)
            for (Sitzordnung s : gesamtPopulation) {
                if (!s.isValid()) s.setFitness(Integer.MAX_VALUE);
            }

            // Sortieren und auf Populationsgröße kürzen
            gesamtPopulation.sort(Comparator.comparingInt(Sitzordnung::getFitness));
            population = new ArrayList<>(gesamtPopulation.subList(0, popGroesse));

            if(printLog) {
                int durchschnittOhneUngueltige = 0;
                int gueltigeIndividuen = 0;
                for (Sitzordnung s : population) {
                    if (s.isValid()) {
                        gueltigeIndividuen++;
                        durchschnittOhneUngueltige += s.getFitness();
                    }
                }
                if (gueltigeIndividuen > 0) {
                durchschnittOhneUngueltige /= gueltigeIndividuen;
                }
                //Konsolenausgaben
                System.out.println("\n[CPA] Generation " + iter + " startet ...");
                System.out.println("Anzahl neue Pflanzen: " + neuePflanzenCount);
                System.out.println("Neue Beute erzeugt. Anzahl neue Beute-Individuen: " + neueBeuteCount);
                System.out.println("Anzahl Reproduktionen: " + neueRekombinationenCount);
                System.out.println("Durchschnittliche fitness ohne ungültige Individuen: " + (gueltigeIndividuen > 0 ? durchschnittOhneUngueltige : "keine gültigen Individuen"));
                System.out.println("Beste Fitness der aktuellen Generation: " + population.get(0).getFitness() + "\n");
                //Datei schreiben
                try {
                    FileWriter myWriter = new FileWriter("./data/"+ reihe + "_" + spalte + "_" + numSchueler + "_" + belohnung + "_" + bestrafung +"_TESTDATA.csv",true);
                    myWriter.write(popGroesse + "," + anzahlPflanzen +"," + anzahlBeute +"," + maxIterationen +"," + spalte +"," + reihe +"," + numSchueler +"," + belohnung
                            + ","+ bestrafung + "," + umgebungTyp.name() +"," + seed +"," + mutationsrate +","+ reproduktionsrate + "," + wachstumsrate + "," + population.get(0).getFitness()+ ","
                            + (gueltigeIndividuen > 0 ? durchschnittOhneUngueltige : "keine gültigen Individuen")+"," + iter+ ","+id+"\n");

                    myWriter.close();
                } catch (IOException e) {
                    System.out.println("An error occurred.");
                    e.printStackTrace();
                }
            }
        }

        // === Nach allen Generationen: Beste gefundene Sitzordnung zurückgeben ===
        population.sort(Comparator.comparingInt(Sitzordnung::getFitness));
        System.out.println("CPA-Algorithmus abgeschlossen. Beste Fitness nach: " + maxIterationen +  ". Genrationen: " + population.get(0).getFitness() + ".");
        return population.get(0);
    }
}
