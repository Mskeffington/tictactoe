#ifndef TIC_TAC_TOE_AI_H
#define TIC_TAC_TOE_AI_H

/**
 * AI is always the O player
 */
#include <vector>
#include <algorithm>
#include <cmath>
#include "board.hpp"
namespace tictactoe {
    
template <unsigned N>
class AI {
public:
    AI(Board<N>& b) : board_{b} {}
    ~AI() {}
    void makeNextMove();
private:
    struct Move {
        int x;
        int y;
        int rank;
        bool operator<(const Move &m) const { return rank < m.rank; }
    };
    Board<N>& board_;
    
    int RankMove(int x, int y);
};

template <unsigned N>
void AI<N>::makeNextMove() {
    std::vector<Move> rankings;
    // search through all the possible moves and rank them according
    // to how dangerous the player is currently and how many blocking
    // moves can be made.
    for(auto y = 1; y <= board_.dimension(); ++y) {
        for(auto x = 1; x <= board_.dimension(); ++x) {
            if(board_.isValidPlay(x,y)) {
                rankings.push_back({x,y,RankMove(x,y)});
            }
        }
    }
    
    if(rankings.size() == 0)
        throw new std::invalid_argument("No valid moves.");
    // sort the array and choose the best move... then play it
    sort(rankings.begin(), rankings.end());
    auto bestMove = rankings[rankings.size() -1];
    board_.play(bestMove.x, bestMove.y, Player::O);
}

int evaluateRank(int self, int player)
{
    auto rank = 0;
    if(self)
        // this move gets us nowhere so it gets no weight
        rank = 0;
    else if(!self && !player)
        // we can block a player move so give that 1 weight
        rank = 1;
    else
        // the player might win so give that a lot of weight
        rank = player * player + 1;
    return rank;
}

template <unsigned N>
int AI<N>::RankMove(int mx, int my) 
{
    if(mx < 1 || mx > board_.dimension()  || my < 1 || my > board_.dimension())
        throw new std::invalid_argument("X and Y must be between 1 and <board dimension>");
    
    int rank = 0;
    int self = 0;
    int player = 0;
    
    // test verical columns
    for(auto x = 1; x <= board_.dimension(); x++) {
        if(x == mx) continue;
        if(board_.getTile(x, my) == tictactoe::Player::X) player++;
        else if(board_.getTile(x, my) == tictactoe::Player::O) self++;
    }
    rank += evaluateRank(self, player);
    
    // test horizontal rows
    self = 0;
    player = 0;
    for(auto y = 1; y <= board_.dimension(); y++) {
        if(y == my) continue;
        if(board_.getTile(mx, y) == tictactoe::Player::X) player++;
        else if(board_.getTile(mx, y) == tictactoe::Player::O) self++;
    }
    rank += evaluateRank(self, player);
    
    // test descending right
    if(mx == my) {
        self = 0;
        player = 0;
        for(auto d = 1; d <= board_.dimension(); d++) {
            if(d == my && d == mx) continue;
            if(board_.getTile(d, d) == tictactoe::Player::X) player++;
            else if(board_.getTile(d, d) == tictactoe::Player::O) self++;
        }
        rank += evaluateRank(self, player);
    }
    
    // test ascending right
    if(mx + my == board_.dimension() + 1) {
        auto dx = 1;
        auto dy = board_.dimension();
        self = 0;
        player = 0;
        while(dx <= board_.dimension()) {
            if(board_.getTile(dx, dy) == tictactoe::Player::X) player++;
            else if(board_.getTile(dx, dy) == tictactoe::Player::O) self++;
            dx++;
            dy--;
        }
        rank += evaluateRank(self, player);
    }
    
    return rank;
}

};

#endif // TIC_TAC_TOE_AI_H
