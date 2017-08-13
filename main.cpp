//
//#include <iostream>
//#define SIZE 4
//
//void printBoard(int board[SIZE][SIZE])
//{
//    for (int i = 0; i < SIZE; i++) {
//
//        //Divisor above lines
//        for (int j = 0; j < SIZE; j++) {
//            std::cout << "+---";
//        }
//        std::cout <<"+" << std::endl;
//
//        //Line Results
//        for (int j = 0; j < SIZE; j++) {
//            std::cout << "|";
//            if(board[i][j] == 0)    //Empty Place
//                std::cout << "   ";
//            else                    //Queen
//                std::cout << " Q ";
//        }
//        std::cout << "|" << std::endl;
//    }
//
//    //Line above board
//    for (int j = 0; j < SIZE; j++) {
//        std::cout << "+---";
//    }
//    std::cout<<"+";
//}
//
//bool canPlaceQueen(int lin, int col, int board[SIZE][SIZE]){
//
//    int i, j;
//
//    //Check Line safety
//    for (int i = 0; i < SIZE; i++){
//        if (board[i][col])
//            return false;
//    }
//
//    //Checks Diagonal Up Left
//    for (i = lin, j = col; i >= 0 && j >= 0; i--, j--){
//        if (board[i][j])
//            return false;
//    }
//
//    //Checks Diagonal Up Right
//    for (i = lin, j = col; i >= 0 && j >= 0; i--, j++){
//        if (board[i][j])
//            return false;
//    }
//
//    //Diagonal Down Covered By Line Method
//
//    return true;
//
//}
//
//bool chooseFromLine(int line, int board[SIZE][SIZE]){
//
//    if (line >= SIZE)   //Check if it's over
//        return true;
//
//    for (int col = 0; col < SIZE; col++){
//        if(canPlaceQueen(line, col, board)){   //Checks if can place queen
//            board[line][col] = 1;   //Places Queen
//            if(chooseFromLine(line + 1, board)) //Can continue game?
//                return true;
//            else
//                board[line][col] = 0;   //Backtracking
//        }
//    }
//
//    return false;
//}
//
//
//int main()
//{
//    int board[SIZE][SIZE] = {0};
//
//    std::cout << "Searching for solution with N = " << SIZE << "..." << std::endl;
//
//    if (!chooseFromLine(0, board))
//        std::cout << "No solution." << std::endl;
//    else
//        std::cout << "Solution found!" << std::endl;
//
//    printBoard(board);
//
//    return 0;
//}