/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 3 */
/* Codigo: Calculando pi com a aproximação de Leibniz 
*/

class piApproximationWorker extends Thread {
    private int id, threads;
    private double iterations;
    private double[] sum;
    //--construtor
    public piApproximationWorker(int id, double iterations, int threads, double[] sum) { 
       this.id = id;
       this.iterations = iterations;
       this.threads = threads;
       this.sum = sum;
    }
 
    //--metodo executado pela thread
    public void run() {
        for (int i = id; i <iterations; i+=threads) {
            double termo = Math.pow(-1, i) / (2.0 * i + 1.0);
            sum[id] += termo;    
        }
        
    }
}

class piApproximation {
    static final int NTHREADS = 4;
    static double deviation;
    private static double NITERATIONS = Math.pow(10, 9);
    private static double[] sum = new double[NTHREADS];

    public static double teste(double approximationPI){
        return Math.abs(Math.PI-approximationPI)/Math.PI;
    }

    public static void main (String[] args) {
        double pi=0;
        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[NTHREADS];
        
        //--PASSO 2: cria threads da classe que estende Thread
        for (int i=0; i<threads.length; i++) {
            threads[i] = new Thread(new piApproximationWorker(i, NITERATIONS, NTHREADS, sum));
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
        deviation = teste(pi);
        System.out.printf("Usando %d Threads e %.0f termos, temos %.16f como aprox de PI\n", NTHREADS, NITERATIONS, pi);
        System.out.printf("O desvio foi de %.12f", deviation); 
        
    }
}

/*
/* Usando 1 Threads e 1000000000 termos, temos 3,1415926525880504 como aprox de PI
/* O desvio foi de 0,000000000319
/* 
/* Usando 2 Threads e 1000000000 termos, temos 3,1415926525855546 como aprox de PI
/* O desvio foi de 0,000000000320
/* 
/* Usando 4 Threads e 1000000000 termos, temos 3,1415926525914433 como aprox de PI
/* O desvio foi de 0,000000000318
/* 
/* Usando 8 Threads e 1000000000 termos, temos 3,1415926525930800 como aprox de PI
/* O desvio foi de 0,000000000317
*/