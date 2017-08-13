//
// Created by naraujo on 13/08/17.
//

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>

#define N 20

std::mutex solutionMutex;

bool verifySolution(int solution[N]){
    for (int i = 0; i < N; i++){
        for (int j = i + 1; j < N; j++){
            //Diagonal Inferior Direita
            if (solution[j] == solution[i] + (j - i))
                return false;
            //Diagonal Inferior Esquerda
            if (solution[j] == solution[i] - (j - i))
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

void permutate(int solution[N], std::promise<bool>&& p, std::shared_future<bool> f[N], int id){

    int copy[N];
    for (int i = 0; i < N; i ++)
        copy[i] = solution[i];

    while(std::next_permutation(copy, copy + N)){
        //Verifica se outra thread achou a solução
//        for (int i = 0; i < N; i++){
//            if (f[i].valid() && f[i].get())
//                return;
//        }
        //Procura solução
        if (verifySolution(copy)) {
            p.set_value(true);

            solutionMutex.lock();

            for (int i = 0; i < N; i++)
                solution[i] = copy[i];
            std::fflush(stdout);
            std::cout << "Thread " << id << " found a solution!" << std::endl;
            for (int i =0; i<N; i++)
                std::cout << copy[i];
            std::cout << std::endl;
            std::fflush(stdout);

            solutionMutex.unlock();

            return;
        }
    }
    p.set_value(false);
    return;
}

int main(){

    //Solution
    int solution[N];

    //Preenche vetor
    for (int i = 0; i < N; i++)
        solution[i] = i;

    std::promise<bool> p[N];
    std::shared_future<bool> f[N];
    std::thread t[N];

    for (int i = 0 ; i < N; i++)
        f[i] = p[i].get_future();

    for (int i=0; i<N; i++){
        t[i] = std::thread(permutate, std::ref(solution), std::move(p[i]), std::ref(f), i); //change solution each iteration
        //t[i].detach();
    }

    for (int i = 0 ; i < N; i++)
        t[i].join();

    printBoard(solution);

    return 0;
}