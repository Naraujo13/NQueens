//
// Created by naraujo on 13/08/17.
//

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>

#define N 5
//Solution
int solution[N];
std::mutex solutionMutex;

bool verifySolution(int copy[N-1], int threadId){

    int test[N];
    test[0] = threadId;
    for (int i = 0; i<N-1; i++)
        test[i+1] = copy[i];

    for (int i = 0; i < N; i++){
        for (int j = i + 1; j < N; j++){
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


void printBoard(int solution[N])
{
    for (int i = 0; i < N; i++) {

        //Divisor above lines
        for (int j = 0; j < N; j++) {
            std::cout << "+---";
        }
        std::cout <<"+" << std::endl;

        //Line Results
        for (int j = 0; j < N; j++) {
            std::cout << "|";
            if(solution[i] == j)    //Empty Place
                std::cout << " Q ";
            else                    //Queen
                std::cout << "   ";
        }
        std::cout << "| " << std::endl;
    }

    //Line below board
    for (int j = 0; j < N; j++) {
        std::cout << "+---";
    }
    std::cout<<"+" << std::endl;
}

void setSolution(int copy[N-1], int threadId){

    solutionMutex.lock();

    //Copia solução encontrada
    solution[0] = threadId;
    for (int i = 0; i < N-1; i++)
        solution[i+1] = copy[i];

    //Print
    std::fflush(stdout);
    std::cout << "Thread " << threadId << " found a solution!" << std::endl;
    for (int i =0; i<N; i++)
        std::cout << solution[i];
    std::cout << std::endl;
    std::fflush(stdout);

    solutionMutex.unlock();
}

void permutate(int original[N], std::promise<bool>&& p, std::shared_future<bool> f[N], int threadId){

    int copy[N-1];
    for (int i = 0; i < N-1; i ++)
        copy[i] = original[i+1];

    while(std::next_permutation(copy, copy + N-1)){
        //Verifica se outra thread achou a solução
//        for (int i = 0; i < N; i++){
//            if (f[i].valid() && f[i].get())
//                return;
//        }
        //Procura solução
        if (verifySolution(copy, threadId)) {
            p.set_value(true);

            setSolution(copy, threadId);

            return;
        }
    }
    p.set_value(false);
    return;
}

int main(){



    //Preenche vetor
    for (int i = 0; i < N; i++)
        solution[i] = i;

    std::promise<bool> p[N];
    std::shared_future<bool> f[N];
    std::thread t[N];

    for (int i = 0 ; i < N; i++)
        f[i] = p[i].get_future();

    for (int i=0; i<N; i++){
        int aux[N-1];


        for (int j = 0, k = 0; k<N-1, j<N; j++, k++){
            if (i != j)
                aux[k] = j;
        }

        t[i] = std::thread(permutate, std::ref(aux), std::move(p[i]), std::ref(f), i); //change solution each iteration
        //t[i].detach();
    }

    for (int i = 0 ; i < N; i++)
        t[i].join();

    printBoard(solution);

    return 0;
}