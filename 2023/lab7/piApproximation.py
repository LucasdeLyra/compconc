from math import pi, pow
from threading import Thread

class PiApproximationWorker(Thread):
    def __init__(self, id, iterations, threads, sum):
        super().__init__()
        self.id = id
        self.iterations = iterations
        self.threads = threads
        self.sum = sum

    def run(self):
        for i in range(self.id, self.iterations, self.threads):
            termo = pow(-1, i) / (2.0 * i + 1.0)
            self.sum[self.id] += termo

class PiApproximation:
    deviation = 0.0
    

    def teste(self, approximation_pi):
        return abs(pi - approximation_pi) / pi

    def main(self, NTHREADS, NITERATIONS):
        sum = [0.0] * NTHREADS
        pi = 0.0
        threads = [None] * NTHREADS

        for i in range(len(threads)):
            threads[i] = PiApproximationWorker(i, NITERATIONS, NTHREADS, sum)

        for thread in threads:
            thread.start()

        for thread in threads:
            thread.join()

        for i in range(len(sum)):
            pi += sum[i]

        pi = 4 * pi
        self.deviation = self.teste(pi)
        print(f"Usando {NTHREADS} Threads e {NITERATIONS} termos, nós temos {pi:.32f} como aproximação de Pi")
        print(f"O desvio foi de {self.deviation:.32f}")


NITERATIONS = int(pow(10, 7))
NTHREADS = 4

a = PiApproximation()
a.main(NTHREADS, NITERATIONS)