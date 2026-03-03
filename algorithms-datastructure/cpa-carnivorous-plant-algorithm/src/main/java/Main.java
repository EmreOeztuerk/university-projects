
import CPA.CPA;
import Sitzordnung.Sitzordnung;

/**
 * Einstiegspunkt zur Ausfuehrung des Carnivorous Plant Algorithm (CPA) mit
 * verschiedenen Parameterkombinationen.
 *
 * @author Emre Öztürk
 * @date 08.08.25
 */
public class Main {
    /**
     * Startet mehrere CPA-Laeufe ueber verschachtelte Parameter-Iterationen.
     *
     * @param args Kommandozeilenargumente (werden nicht verwendet)
     */
    public static void main(String[] args) {
        System.out.println("Fertig");
        for(double i = 0.3; i < 1; i+=0.4){//mutationsrate
            for(double j = 0.3; j < 1; j+=0.4){//growthrate
                for(double m = 0.3; m < 1; m+=0.4){//reproduktionsrate
                    for(int n = 0; n < 5; n++){ //Seed
                        for(int k = 1; k < 5; k++){ //popGroesse und anzahlPflanzen
                                    CPA cpa = new CPA(k*20, k*5, 6, 4, 24, -2, 3,
                                            Sitzordnung.umgebungTyp.VERTIKAL_HORIZONTAL, 1000,
                                            n, i,m,j,true,2);
                                    cpa.fuehreAus();


                        }
                    }
                }
            }
        }
    }
}
