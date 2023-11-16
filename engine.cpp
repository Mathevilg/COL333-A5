#include <algorithm>
#include <random>
#include <iostream>
#include <climits>
#include <chrono>
#include <set>

#include "board.hpp"
#include "engine.hpp"

std::multiset<std::string> gpos;

int evaluation(const Board* b)
{
    int eval = 0;

    // piece values for white
    if(b->in_check() && b->data.player_to_play == BLACK) eval += 10; 
    if(b->data.w_king!=DEAD) eval += 100;
    if(b->data.w_rook_ws!=DEAD) eval += 60;
    if(b->data.w_rook_bs!=DEAD) eval += 60;
    if(b->data.w_bishop!=DEAD) eval += 40;
    if(b->data.w_pawn_ws!=DEAD)
    {
        int x = getx(b->data.w_pawn_ws);
        int y = gety(b->data.w_pawn_ws);
        if(b->data.board_0[b->data.w_pawn_ws] == PAWN_ROOK) eval += 60;
        else if(b->data.board_0[b->data.w_pawn_ws] == PAWN_BISHOP) eval += 40;
        else
        {
            if(y<=1) eval += 20 + 2*y - x;
            else eval += 20 + 2*y + x;
        }
    }
    if(b->data.w_pawn_bs!=DEAD)
    {
        int x = getx(b->data.w_pawn_bs);
        int y = gety(b->data.w_pawn_bs);
        if(b->data.board_0[b->data.w_pawn_bs] == PAWN_ROOK) eval += 60;
        else if(b->data.board_0[b->data.w_pawn_bs] == PAWN_BISHOP) eval += 40;
        else
        {
            if(y<=1) eval += 20 + 2*y - x;
            else eval += 20 + 2*y + x;
        }
    }

    // piece values for black
    if(b->in_check() && b->data.player_to_play == WHITE) eval -= 10; 
    if(b->data.b_king!=DEAD) eval -= 100;
    if(b->data.b_rook_ws!=DEAD) eval -= 60;
    if(b->data.b_rook_bs!=DEAD) eval -= 60;
    if(b->data.b_bishop!=DEAD) eval -= 40;
    if(b->data.b_pawn_ws!=DEAD)
    {
        int x = getx(b->data.b_pawn_ws);
        int y = gety(b->data.b_pawn_ws);
        if(b->data.board_0[b->data.b_pawn_ws] == PAWN_ROOK) eval -= 60;
        else if(b->data.board_0[b->data.b_pawn_ws] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
            else eval -= (20 + 2*(6-y) + (6-x));
        }
    }
    if(b->data.b_pawn_bs!=DEAD)
    {
        int x = getx(b->data.b_pawn_bs);
        int y = gety(b->data.b_pawn_bs);
        if(b->data.board_0[b->data.b_pawn_bs] == PAWN_ROOK) eval -= 60;
        else if(b->data.board_0[b->data.b_pawn_bs] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
            else eval -= (20 + 2*(6-y) + (6-x));
        }
    }

    return eval;
}

bool white(U16 &move1,U16 &move2,Board *b)
{
    auto nb1 = b->copy();
    nb1->do_move(move1);
    int eval1 = evaluation(nb1);
    delete nb1;
    nb1 = NULL;
    auto nb2 = b->copy();
    nb2->do_move(move2);
    int eval2 = evaluation(nb2);
    delete nb2;
    nb2 = NULL;
    return eval1>eval2;
}

bool black(U16 &move1,U16 &move2,Board *b)
{
    auto nb1 = b->copy();
    nb1->do_move(move1);
    int eval1 = evaluation(nb1);
    delete nb1;
    nb1 = NULL;
    auto nb2 = b->copy();
    nb2->do_move(move2);
    int eval2 = evaluation(nb2);
    delete nb2;
    nb2 = NULL;
    return eval1<eval2;
}

void ordering(std::vector<U16>& v,Board *b)
{
    if(b->data.player_to_play==WHITE)
    {
        std::sort(v.begin(),v.end(),[b](U16 move1,U16 move2)
        {
            return white(move1, move2, b);
        });
    }
    else
    {
        std::sort(v.begin(),v.end(),[b](U16 move1,U16 move2)
        {
            return black(move1, move2, b);
        });
    }
}

std::pair<U16,int> alpha_beta(const Board* b,int depth,int alpha,int beta,int maxdepth,Engine* e,std::multiset<std::string> &pos)
{
    auto moveset = b->get_legal_moves();
    if(moveset.size()==0) 
    {
        if(!b->in_check()) return {0,0};
        if(b->data.player_to_play == WHITE) return {0,INT_MIN};
        if(b->data.player_to_play == BLACK) return {0,INT_MAX};
    }
    if(depth == maxdepth) return {0,evaluation(b)};

    if(b->data.player_to_play == WHITE)
    {
        int eval = INT_MIN;
        U16 move = 0;

        std::vector<U16> moves;
        for(auto m:moveset) moves.push_back(m);
        Board* newb = b->copy();
        ordering(moves,newb);
        delete newb;
        newb = NULL;

        for(auto m:moves)
        {
            if(!e->search) break;
            auto nb = b->copy();
            nb->do_move(m);
            
            std::pair<U16,int> node;
            std::string position = all_boards_to_str(*nb);
            pos.insert(position);
            if(gpos.count(position) + pos.count(position)==3) node = {m,0};
            else node = alpha_beta(nb,depth+1,alpha,beta,maxdepth,e,pos);
            pos.erase(pos.find(position));
            
            delete nb;
            nb = NULL;
            if(node.second>eval)
            {
                eval = node.second;
                move = m;
            }
            alpha = std::max(alpha,eval);
            if(beta <= alpha) break;
        }
        if(!e->search) return {1<<15,0};
        return {move,eval};
    }
    else
    {
        int eval = INT_MAX;
        U16 move = 0;

        std::vector<U16> moves;
        for(auto m:moveset) moves.push_back(m);
        Board* newb = b->copy();
        ordering(moves,newb);
        delete newb;
        newb = NULL;

        for(auto m:moves)
        {
            if(!e->search) break;
            auto nb = b->copy();
            nb->do_move(m);

            std::pair<U16,int> node;
            std::string position = all_boards_to_str(*nb);
            pos.insert(position);
            if(gpos.count(position) + pos.count(position)==3) node = {m,0};
            else node = alpha_beta(nb,depth+1,alpha,beta,maxdepth,e,pos);
            pos.erase(pos.find(position));

            delete nb;
            nb = NULL;
            if(node.second<eval)
            {
                eval = node.second;
                move = m;
            }
            beta = std::min(beta,eval);
            if(beta <= alpha) break;
        }
        if(!e->search) return {1<<15,0};
        return {move,eval};
    }
}

void Engine::find_best_move(const Board& b) {
    gpos.insert(all_boards_to_str(b));
    std::multiset<std::string> pos;
    for(int depth = 2;this->search;depth+=2)
    {
        auto p = alpha_beta(&b,0,INT_MIN,INT_MAX,depth,this,pos);
        if(search) this->best_move = p.first;
    }
    auto cb = b.copy();
    cb->do_move(this->best_move);
    gpos.insert(all_boards_to_str(*cb));
    delete cb;
    cb = NULL;
}