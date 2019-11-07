//
//  playMove.cpp
//  TicTacTo
//
//  Updated by Tarek Abdelrahman on 2019-06-07.
//  Created by Tarek Abdelrahman on 2018-05-15.
//  Copyright © 2018-2019 Tarek Abdelrahman. All rights reserved.
//

#include "tictactoe.h"
#include "gameState.h"

void playMove (gameState& game_state) {
    
    if (game_state.get_gameBoard(game_state.get_clickedRow(),game_state.get_clickedColumn()) == Empty) { // Checks if selected square is empty
        game_state.set_moveValid(true);
        
        if (game_state.get_turn() == true){
            game_state.set_gameBoard(game_state.get_clickedRow(), game_state.get_clickedColumn(), X);
        }
        else {
            game_state.set_gameBoard(game_state.get_clickedRow(), game_state.get_clickedColumn(), O);
        }
        if (game_state.get_gameBoard(0,2) == game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1) == game_state.get_gameBoard(2,0) && game_state.get_gameBoard(2,0) != Empty){
            game_state.set_winCode(8); // Checking Diagonal from right to left
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(0,0) == game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1) == game_state.get_gameBoard(2,2) && game_state.get_gameBoard(2,2) != Empty){
            game_state.set_winCode(7); // Checking Diagonal from left to right
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(0,2) == game_state.get_gameBoard(1,2) && game_state.get_gameBoard(1,2) == game_state.get_gameBoard(2,2) && game_state.get_gameBoard(2,2) != Empty){
            game_state.set_winCode(6); // Checking for Column 2
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(0,1) == game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1) == game_state.get_gameBoard(2,1) && game_state.get_gameBoard(0,1) != Empty){
            game_state.set_winCode(5); // Checking for Column 1
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(0,0) == game_state.get_gameBoard(1,0) && game_state.get_gameBoard(1,0) == game_state.get_gameBoard(2,0) && game_state.get_gameBoard(2,0) != Empty){
            game_state.set_winCode(4); // Checking for Column 0
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(2,0) == game_state.get_gameBoard(2,1) && game_state.get_gameBoard(2,1) == game_state.get_gameBoard(2,2)&& game_state.get_gameBoard(2,2) != Empty){
            game_state.set_winCode(3); // Checking for Row 2
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(1,0) == game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1) == game_state.get_gameBoard(1,2) && game_state.get_gameBoard(1,2) != Empty){
            game_state.set_winCode(2); // Checking for Row 1
            game_state.set_gameOver(true);
        }
        else if (game_state.get_gameBoard(0,0) == game_state.get_gameBoard(0,1) && game_state.get_gameBoard(0,1) == game_state.get_gameBoard(0,2) && game_state.get_gameBoard(0,2) != Empty) {
            game_state.set_winCode(1); // Checking for Row 0
            game_state.set_gameOver(true);
        }     
        else {
            game_state.set_winCode(0); // If winning conditions are not matched, winCode is 0
        }
        
        if (game_state.get_winCode() == 0) { //Only switches turns if no one has won
            game_state.set_gameOver(false); 
        if (game_state.get_turn() == true){ // Switch the players' turns
            game_state.set_turn(false);
        }
        else if(game_state.get_turn() == false){ // Switch the players' turns
            game_state.set_turn(true);
        }
    }
   }
    else {
        game_state.set_moveValid(false);
        game_state.set_gameOver(false);
        game_state.set_winCode(0);
        game_state.set_turn(game_state.get_turn()); // Turn stays the same
    }
}