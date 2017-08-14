#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#define N 999
//Solution
std::mutex solutionMutex;
int solution[N];

//Aux
std::mutex auxMutex;

std::mutex stopMutex;
bool stop = false;

bool verifySolution(int size, int copy[N-1], int threadId){

    int test[size];
    test[0] = threadId;
    for (int i = 0; i<size-1; i++)
        test[i+1] = copy[i];

//    for (int i = 0; i<N; i++){
//        for (int j = 0; j<N; j++){
//            if (i != j && test[i] == test[j])
//                return false;
//        }
//    }

    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            //Diagonal Inferior Direita
            if (test[j] == test[i] + (j - i))
                return false;
            //Diagonal Inferior Esquerda
            if (test[j] == test[i] - (j - i))
                return false;
        };
    };

    return true;
}


void printBoard(int size, int solution[N])
{
    std::fflush(stdout);
    for (int i = 0; i < size; i++) {

        //Divisor above lines
        for (int j = 0; j < size; j++) {
            std::cout << "+---";
        }
        std::cout <<"+" << std::endl;

        //Line Results
        for (int j = 0; j < size; j++) {
            std::cout << "|";
            if(solution[i] == j)    //Empty Place
                std::cout << " Q ";
            else                    //Queen
                std::cout << "   ";
        }
        std::cout << "| " << std::endl;
    }

    //Line below board
    for (int j = 0; j < size; j++) {
        std::cout << "+---";
    }
    std::cout<<"+" << std::endl;
    std::fflush(stdout);
}

void setSolution(int size, int copy[N-1], int threadId){

    std::lock_guard<std::mutex> stopGuard(stopMutex);
    std::lock_guard<std::mutex> solutionGuard(solutionMutex);

    stop = true;

    //Copia solução encontrada
    solution[0] = threadId;
    for (int i = 0; i < size-1; i++)
        solution[i+1] = copy[i];

    //Print
    std::fflush(stdout);
    std::cout << "Thread " << threadId << " found a solution!" << std::endl;
    for (int i =0; i<size; i++)
        std::cout << solution[i] << "|";
    std::cout << std::endl;
    std::fflush(stdout);

}

void permutate(int size, int aux[N-1], std::promise<bool>&& p, int threadId){

    //Copia inicial para local
    int copy[size-1];
    for (int i = 0; i < size-1; i ++)
        copy[i] = aux[i];

    //Libera inicial
    auxMutex.unlock();

    while(!stop && std::next_permutation(copy, copy + size-1)){
        //Verifica se outra thread achou a solução
//        for (int i = 0; i < N; i++){
//            if (f[i].valid() && f[i].get())
//                return;
//        }
        //Procura solução
        if (verifySolution(size, copy, threadId)) {
            p.set_value(true);

            setSolution(size, copy, threadId);

            return;
        }
    }
    p.set_value(false);
    return;
}

int main(){

    clock_t tStart = clock();

    int size = 0;
    std::cout << "Insira o número de rainhas/tamanho do tabuleiro: ";
    std::cin >> size;
    std::cout << std::endl;
    //Preenche vetor
    std::lock_guard<std::mutex> intializationGuard(solutionMutex);
    for (int i = 0; i < size; i++)
        solution[i] = i;
    solutionMutex.unlock();

    std::promise<bool> p[size];
    std::shared_future<bool> f[size];
    std::thread t[size];

    for (int i = 0 ; i < size; i++)
        f[i] = p[i].get_future();

    for (int i=0; i<size; i++){
        int aux[N-1];

        //Lock vetor inicial
        auxMutex.lock();
        //Cria array de permutação inicial dessa thread para dividir o trabalho
        for (int j = 0, k = 0; k<size-1, j<size; j++){
            if (i != j) {
                aux[k] = j;
                k++;
            }
        }

        //Cria thread
        t[i] = std::thread(permutate, size, aux, std::move(p[i]), i); //change solution each iteration

    }
    //Join nas threads
    for (int i = 0 ; i < size; i++)
        t[i].join();

    //Recupera futuros
    bool found;
    for (int i = 0 ; i < size; i++) {
        if (f[i].get())
            found = true;
    }

    if(found) {
        //Printa solução
        std::lock_guard<std::mutex> printGuard(solutionMutex);
        std::cout << std::endl << std::endl << "Solução final:" << std::endl;
        for (int i = 0; i < size; i++)
            std::cout << solution[i] << "|";
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << ((double) (clock() - tStart) / CLOCKS_PER_SEC) << " segundos" <<
        std::endl;
        printBoard(size, solution);
    }
    else
        std::cout << "No solution was found" <<std::endl;

    return 0;
}