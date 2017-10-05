#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h>
#include <unistd.h>
#include <sys/select.h>
#include "board.hpp"
#include "ttt_ai.hpp"
using namespace std;

static auto running = true;
#define DEFAULT_BOARD_DIMENSION 3

void print(const std::string& m)
{
    mvprintw(DEFAULT_BOARD_DIMENSION * 2 + 1, 0, m.c_str());
}

void drawCursor(unsigned x, unsigned y)
{
    mvprintw((y + 1) * 2 - 1, x * 4 +1 , "?");
}

template <unsigned N>
void findNextOpenSpot(tictactoe::Board<N>& b, int& cx, int& cy)
{
    for(auto y = 1; y <= DEFAULT_BOARD_DIMENSION; ++y) {
        for(auto x = 1; x <= DEFAULT_BOARD_DIMENSION; ++x) {
            if(b.isValidPlay(x, y)) {
                cx = x - 1;
                cy = y - 1;
                return;
            }
        }
    }
}

int main()
{
    tictactoe::Board<DEFAULT_BOARD_DIMENSION> tttBoard{};
    tictactoe::AI<DEFAULT_BOARD_DIMENSION> computerPlayer{tttBoard};
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(false);
    int cursorx = 0;
    int cursory = 0;
    tictactoe::Player turn = tictactoe::Player::X;
    
    
    while(running) {
        // render the screen
        clear();
        
        mvprintw(0, 0, "Press <q> to quit.");
        auto output = tttBoard.toString();
        mvprintw(1, 0, output.c_str());
        
        // check for the winner
        auto winner = tttBoard.winner();
        if(winner != tictactoe::Player::NONE) {
            stringstream winMsg;
            winMsg << winner << " Wins! Press the any key to quit.";
            print(winMsg.str().c_str());
            running = false;
            getch();
        } else if(tttBoard.isTie()) {
            print("Game ended in a tie.");
            running = false;
            getch();
        } else {
            drawCursor(cursorx, cursory);
        
            if(turn == tictactoe::Player::X) {
                auto code = getch();
                
                // read the current code for the input
                stringstream msg;
                switch(code) {
                    case KEY_LEFT:
                        cursorx -= 1;
                        break;
                    case KEY_RIGHT:
                        cursorx += 1;
                        break;
                    case KEY_UP:
                        cursory -= 1;
                        break;
                    case KEY_DOWN:
                        cursory += 1;
                        break;
                    case 'q':
                    case KEY_EXIT:
                        running = false;
                        break;
                    case KEY_ENTER:
                    case '\r':
                    case '\n':
                        // player selected a move
                        tttBoard.play(cursorx + 1, cursory + 1, turn);
                        
                        findNextOpenSpot(tttBoard, cursorx, cursory);
                        
                        // switch turns
                        turn = tictactoe::Player::O;
                        
                        break;
                    default:
                        // do nothing
                        break;
                }
                // make sure the imput remains on the board by clamping
                cursorx = min(DEFAULT_BOARD_DIMENSION -1, max(0, cursorx));
                cursory = min(DEFAULT_BOARD_DIMENSION -1, max(0, cursory));
            } else {
                // have the computer play
                computerPlayer.makeNextMove();
                
                findNextOpenSpot(tttBoard, cursorx, cursory);
                
                // switch turns
                turn = tictactoe::Player::X;
            }
        }
        refresh();
        // this random sleep helps even out the rendering so its not super choppy.
        usleep(300);
    }
    
    endwin();

    return 0;
}
