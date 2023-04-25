import java.lang.reflect.Array;
import static java.lang.Math.*;
/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 3 */
/* Codigo: Calculando pi com a aproximação de Leibniz */


class piApproximation {
    static final int NTHREADS = 8;
    private static int NITERATIONS = 100000000;
    private static double[] sum = new double[NTHREADS];

    static class piApproximationWorker extends Thread {
        private int id;
        //--construtor
        public piApproximationWorker(int id) { 
           this.id = id;
        }
     
        //--metodo executado pela thread
        public void run() {
            for (int i = id * NITERATIONS; i < (id + 1) * NITERATIONS; i++) {
                double term = Math.pow(-1, i) / (2.0 * i + 1.0);
                sum[id] += term;    
            }
            
        }
    }

 
    public static void main (String[] args) {
        double pi=0;
        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[NTHREADS];
        
        //--PASSO 2: cria threads da classe que estende Thread
        for (int i=0; i<threads.length; i++) {
            threads[i] = new Thread(new piApproximationWorker(i));
        }

        //--PASSO 3: iniciar as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //--PASSO 4: esperar pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } 
            catch (InterruptedException e) { return; }
        }
        for (int i = 0; i<sum.length; i++){
            pi += sum[i];
        }
        pi = 4*pi;
        System.out.println(pi); 
    }
}