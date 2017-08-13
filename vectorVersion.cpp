//
// Created by naraujo on 13/08/17.
//

#include <iostream>
#include <algorithm>

#define N 15


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

bool permutate(int solution[N]){
    while(std::next_permutation(solution, solution + N)){
        if (verifySolution(solution))
            return true;
    }
    return false;
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
        std::cout << " ";
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
    std::cout<<"+";
}



int main(){

    int solution[N];

    //Preenche vetor
    for (int i = 0; i < N; i++)
        solution[i] = i;

    if (permutate(solution))
        printBoard(solution);
    else
        std::cout << "No solution found" << std::endl;


    return 0;
}