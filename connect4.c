#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
// Console Colours
#define RESET   "\033[0m"
#define DIM     "\033[2m"
#define STRIKED "\033[9m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
// Players
#define PLAYER1 1
#define PLAYER2 2
// MAIN BOARD (6x7)
int BOARD[6][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
int TOP[7] = {6,6,6,6,6,6,6};   /* Gives index of the TOPMOST chip in a column. TOP[col] gives row index
                                Example: 6 = No chip. 5 = Chip at index 5 (i.e. 1 chip). 0 = Chip on the topmost row => Column full! */

int checkHorizontal(int player,int col) {
    /* Checks if there are 4 chips in a row horizontally
    Returns: (1 or 2) if 4 in a row, else 0 */
    int count = 1;
    int left = col-1;   // Check to the LEFT of the new chip
    while (left > -1 && BOARD[TOP[col]][left] == player) {
        count++;
        left--;
    }
    int right = col+1;  // Check to the RIGHT of the new chip
    while (right < 7 && BOARD[TOP[col]][right] == player) {
        count++;
        right++;
    }
    if (count >= 4) return player;  // 4 in a row found
    return 0;  // No match found
}

int checkVertical(int player, int col) {
    /* Checks if there are 4 chips in a row vertically
    Returns: (1 or 2) if 4 in a row, else 0 */
    int count = 1;
    int downRow = TOP[col]+1;  // Checks BELOW the new chip
    while (downRow < 6 && BOARD[downRow][col] == player) {
        count++;
        downRow++;
    }
    if (count >= 4) return player;
    return 0;
}

int checkMainDiagonal(int player, int col) {
    /* Checks if there are 4 chips in a row along the main diagonal (\)
    Returns: (1 or 2) if 4 in a row, else 0 */
    int count = 1;
    int upRow = TOP[col]-1;    // Checks to the UPLEFT of the new chip
    int upColumn = col-1;      // --                                --
    while (upRow > -1 && upColumn > -1 && BOARD[upRow][upColumn] == player) {
        count++;
        upRow--;
        upColumn--;
    }
    int downRow = TOP[col]+1;  // Checks to the BOTTOMRIGHT of the new chip
    int downColumn = col+1;    // --                                     --
    while (downRow < 6 && downColumn < 7 && BOARD[downRow][downColumn] == player) {
        count++;
        downRow++;
        downColumn++;
    }
    if (count >= 4) return player;  
    return 0;
}

int checkSecondaryDiagonal(int player, int col) {
    /* Checks if there are 4 chips in a row along the secondary diagonal (/)
    Returns: (1 or 2) if 4 in a row, else 0 */
    int count = 1;
    int upRow = TOP[col]-1;    // Checks to the UPRIGHT of the new chip
    int upColumn = col+1;      // --               --
    while (upRow > -1 && upColumn < 7 && BOARD[upRow][upColumn] == player) {
        count++;
        upRow--;
        upColumn++;
    }
    int downRow = TOP[col]+1;  // Checks to the DOWNLEFT of the new chip
    int downColumn = col-1;    // --                --
    while (downRow < 6 && downColumn > -1 && BOARD[downRow][downColumn] == player) {
        count++;
        downRow++;
        downColumn--;
    }
    if (count >= 4) return player;
    return 0;
}

int checkWinner(int player, int col) {
    /* Master function to check if any of the 4 conditions are met
    Returns: (1 or 2) if any condition is true, else 0(no match) */
    if (checkHorizontal(player, col)) return player;
    if (checkVertical(player, col)) return player;
    if (checkMainDiagonal(player, col)) return player;
    if (checkSecondaryDiagonal(player, col)) return player;
    return 0;
}

void updateBoard(int player, int col) {
    // Updates board after player's new chip is inserted
    TOP[col]--;
    BOARD[TOP[col]][col] = player;
}

void displayBoard() {
    // Displays the 6x7 Board. Replaces 1 with RED O for Player 1, and 2 with BLUE O for player 2. Replaces 0 with WHITE O (Empty slot)
    // Dims and strikes out column label if the column is full
    for (int i = 0;i<6;i++) {
        for (int j = 0;j<7;j++) {
            if (BOARD[i][j] == 1) printf(RED "  O  " RESET);        // PLAYER 1's RED Chip
            else if (BOARD[i][j] == 2) printf(BLUE "  O  " RESET);  // PLAYER 2's BLUE Chip
            else printf("  O  ");                                   // EMPTY SLOT
        }
        printf("\n");
    }
    for (int i = 1;i<=7;i++) {
        printf(" ");
        if (TOP[i-1] == 0) printf(STRIKED DIM);     // STRIKED + DIMMED Column label if column is full
        printf("[%d]",i);                           // Else, regular label
        printf(RESET " ");
    }
    printf("\n");
}

int isDraw() {
    /* Checks if TOP array is all 0
    Returns: 1 if true(i.e. BOARD is full), else 0 (i.e. BOARD is not full) */
    for (int i = 0; i < 7; i++) if (TOP[i] > 0) return 0;
    return 1;
}

int getSafeIntInput() {
    /* Gets input from user and validates it
    Returns: 0 if invalid input in anyway, else returns the correct value*/
    char input[100];
    if (!fgets(input, sizeof(input), stdin)) {  // Get user input
        printf(MAGENTA "!! Input error. Try again. !!\n" RESET);
        return 0;
    }
    input[strcspn(input, "\n")] = 0;     // Remove trailing newline

    int start = 0;
    while (isspace(input[start])) start++;  // Remove trailing spaces

    int end = strlen(input) - 1;
    while (end >= start && isspace(input[end])) end--;  // Remove leading spaces

    if (start > end) {  // If empty input
        printf(MAGENTA "!! Empty input. Try again. !!\n" RESET);
        return 0;
    }
    for (int i = start; i <= end; i++) {    // Only digits (0-9)
        if (!isdigit(input[i])) {
            printf(MAGENTA "!! Invalid input. Must be a number between 1 and 7. !!\n" RESET);
            return 0;
        }
    }
    int val = atoi(input);  // str to int
    if (val < 1 || val > 7) {   // Validate range
        printf(MAGENTA "!! Invalid input. Must be a number between 1 and 7. !!\n" RESET);
        return 0;
    }
    return val;
}

int main() {
    int player = PLAYER1;   // Start with Player 1
    while (1) {
        int col; // To store user input
        if (player == PLAYER1) printf(RED "PLAYER 1, Choose a column: " RESET);
        else printf(BLUE "PLAYER 2, Choose a column: " RESET);
        col = getSafeIntInput();
        if (col) col--;  // If not 0: 1-based to 0-based indexing
        else continue;   // if col == 0 i.e. invalid input, loops again
        if (TOP[col] > 0) updateBoard(player,col);  // Updates board only if column has space
        else {
            printf(MAGENTA "!! Column is full. Try another column. !!\n" RESET);
            continue;
        }
        displayBoard();
        if (checkWinner(player,col) != 0) {
            if (player == PLAYER1) printf(RED "PLAYER 1 WON!\n" RESET);
            else printf(BLUE "PLAYER 2 WON!\n" RESET);
            break;
        }
        if (isDraw()) {
            printf(MAGENTA "IT WAS A TIE!\n" RESET);
            break;
        }
        if (player == PLAYER1) player = PLAYER2;  // Swaps players if no winner and not a draw
        else player = PLAYER1;
    }
}