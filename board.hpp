#ifndef TIC_TAC_TOE_BOARD_H
#define TIC_TAC_TOE_BOARD_H

#include <string>
#include <array>
#include <ostream>
#include <exception>
#include <sstream>
#include <iostream>


namespace tictactoe {
    

enum class Player {
    NONE,
    O,
    X
};

std::ostream& operator<<(std::ostream& os, const tictactoe::Player& p) {
    switch(p) {
        case tictactoe::Player::O:
            os << "O";
            break;
        case tictactoe::Player::X:
            os << "X";
            break;
        case tictactoe::Player::NONE:
        default:
            os << " ";
            break;
    }
    return os;
}
    
template <unsigned N>
class Board {
public:
    Board() : plays_{0}, dimension_{N}, boardstate_{Player::NONE} {}
    ~Board() {}
    std::string toString();
    bool isValidPlay(unsigned x, unsigned y);
    
    void play(unsigned x, unsigned y, Player p);
    Player winner();
    bool isTie() { return dimension_ * dimension_ == plays_; }
    Player getTile(int x, int y);
    unsigned dimension() const { return dimension_; }
    //friend std::ostream& operator<<(std::ostream& os, const Board<N>& b);
private:
    unsigned plays_;
    unsigned dimension_;
    std::array<Player, N * N> boardstate_;
    
    unsigned coordToIdx(unsigned x, unsigned y);
};


template <unsigned N>
unsigned Board<N>::coordToIdx(unsigned x, unsigned y)
{
    return (x-1) + (y-1) * dimension_;
}

template <unsigned N>
Player Board<N>::getTile(int x, int y)
{
    if(x < 1 || x > dimension_  || y < 1 || y > dimension_)
        throw new std::invalid_argument("X and Y must be between 1 and <dimension> and unpopulated");
    
    return boardstate_[coordToIdx(x,y)];
}

template <unsigned N>
std::string Board<N>::toString()
{
    std::stringstream output;
    auto horizontal = dimension_ * 4 - 1;
    
    for(auto y = 1; y <= dimension_; y++) {
        for(auto x = 1; x <= dimension_; x++) {
            auto value = boardstate_[coordToIdx(x, y)];
            output << " " << value << " ";
            
            if(x < dimension_)
                output << "|";
        }
        
        // draw the horizontal lines
        if(y < dimension_) {
            output << std::endl;
            for( auto x = 0; x < horizontal; ++x) 
                output << "-";
            output << std::endl;
        }
    }
    return output.str();
}

template <unsigned N>
bool Board<N>::isValidPlay(unsigned x, unsigned y)
{
    // check if points are in bounds
    if(x < 1 || x > dimension_  || y < 1 || y > dimension_)  return false;
    // check if space is already played
    if(boardstate_[coordToIdx(x, y)] == Player::NONE) return true;
    return false;
}

template <unsigned N>
void Board<N>::play(unsigned x, unsigned y, Player p)
{
    auto idx = coordToIdx(x,y);
    if(!isValidPlay(x, y)) {
        throw new std::invalid_argument("X and Y must be between 1 and <dimension> and unpopulated");
    }
    if(p == Player::NONE) {
        throw new std::invalid_argument("Invalid Player p.");
    }
    plays_++;
    boardstate_[idx] = p;
}

template <unsigned N>
Player Board<N>::winner()
{
    static Player winner = Player::NONE;
    if(winner != Player::NONE)
        return winner;
    
    bool fail = false;
    Player base;
    Player next;
    
    // check vertical direction matches
    for (auto x = 1; x <= dimension_; ++x) {
        fail = false;
        base = boardstate_[coordToIdx(x, 1)];
        for (auto y = 1; y <= dimension_; ++y) {
            next = boardstate_[coordToIdx(x, y)];
            if(base != next || next == Player::NONE) fail = true;
        }
        if(fail == false) winner = base;
    }
    
    // check horizontal direction matches
    for (auto y = 1; y <= dimension_; ++y) {
        fail = false;
        base = boardstate_[coordToIdx(1, y)];
        for (auto x = 1; x <= dimension_; ++x) {
            next = boardstate_[coordToIdx(x, y)];
            if(base != next || next == Player::NONE) fail = true;
        }
        if(fail == false) winner = base;
    }
    
    // check diagonal descending right
    base = boardstate_[coordToIdx(1, 1)];
    fail = false;
    for (auto d = 1; d <= dimension_; ++d) {
        next = boardstate_[coordToIdx(d, d)];
        if(base != next || next == Player::NONE) fail = true;
    }
    if(fail == false) winner = base;
    
    // check diagonal ascending right
    auto dx = 1;
    auto dy = dimension_;
    base = boardstate_[coordToIdx(1, dimension_)];
    fail = false;
    while(dx <= dimension_) {
        next = boardstate_[coordToIdx(dx, dy)];
        if(base != next || next == Player::NONE) fail = true;
        dx++;
        dy--;
    }
    if(fail == false) winner = base;
    
    return winner;
}

};




#endif // TIC_TAC_TOE_BOARD_H
