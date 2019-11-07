//
//  gameState.cpp
//  TicTacTo
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Copyright © 2019 Tarek Abdelrahman. All rights reserved.

#include "tictactoe.h"
#include "gameState.h"

int gameState::get_clickedRow() {
    return clickedRow;
}
   
int gameState::get_clickedColumn() {
    return clickedColumn;
}  

void gameState::set_clickedRow(int value) {
    clickedRow = value;
}

void gameState::set_clickedColumn(int value) {
    clickedColumn = value;
}  

bool gameState::get_moveValid() {
    return moveValid;
}

void gameState::set_moveValid(bool value) {
    moveValid = value;
}

bool gameState::get_gameOver(){
    return gameOver;
}

void gameState::set_gameOver(bool value){
    gameOver = value;
}

int gameState::get_winCode(){
    return winCode;
}


void gameState::set_winCode(int value){
    winCode = value;
}


bool gameState::get_turn(){
    return turn;
}


void gameState::set_turn(bool value){
    turn = value;
}


int gameState::get_gameBoard(int row, int col){
    return gameBoard[row][col];
}


void gameState::set_gameBoard(int row, int col, int value){
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
        gameBoard[row][col] = value;
    }
}
