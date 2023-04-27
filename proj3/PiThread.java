/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 3 */
/* Codigo: "Hello World" usando threads em Java */


//--PASSO 1: criar uma classe que implementa a interface Runnable 

//--PASSO 1: criar uma classe que implementa a interface Runnable 
class PiCalculator implements Runnable {
   private int startTerm;
   private int endTerm;
   private double result;

   //--construtor
   public PiCalculator(int startTerm, int endTerm) {
      this.startTerm = startTerm;
      this.endTerm = endTerm;
   }

   //--metodo para obter o resultado da serie calculado pela thread
   public double getResult() {
      return result;
   }

   //--metodo executado pela thread
   public void run() {
      double sum = 0;
      int sign = 1;
      for (int i = startTerm; i <= endTerm; i += 2) {
         sum += sign * (1.0 / i);
         sign *= -1;
      }
      result = sum;
   }
}

//--classe do metodo main
class PiThread {
   static final int NUM_THREADS = 4;
   static final int NUM_TERMS = 1000000;

   public static void main (String[] args) {
      //--reserva espaço para um vetor de threads
      Thread[] threads = new Thread[NUM_THREADS];
      PiCalculator[] calculators = new PiCalculator[NUM_THREADS];

      //--PASSO 2: transformar o objeto Runnable em Thread
      int termsPerThread = NUM_TERMS / NUM_THREADS;
      for (int i = 0; i < threads.length; i++) {
         int startTerm = i * termsPerThread + 1;
         int endTerm = (i + 1) * termsPerThread;
         if (i == NUM_THREADS - 1) {
            endTerm = NUM_TERMS;
         }
         calculators[i] = new PiCalculator(startTerm, endTerm);
         threads[i] = new Thread(calculators[i]);
      }

      //--PASSO 3: iniciar a thread
      for (int i = 0; i < threads.length; i++) {
         threads[i].start();
      }

      //--PASSO 4: esperar pelo termino das threads
      for (int i = 0; i < threads.length; i++) {
         try { 
            threads[i].join(); 
         } catch (InterruptedException e) { 
            return; 
         }
      } 

      //--PASSO 5: somar os resultados das threads e obter o valor final de pi
      double pi = 0;
      for (int i = 0; i < threads.length; i++) {
         pi += calculators[i].getResult();
      }
      pi *= 4;

      System.out.println("Valor de pi: " + pi);
   }
}
