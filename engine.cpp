#include <algorithm>
#include <random>
#include <iostream>
#include <thread>
#include <set>
#include <climits>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

std::multiset<std::string> gpos;

int rook_7_3[7][7] = {
    {4, 2, 2, 2, 2, 5, 4},
    {5, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 5},
    {4, 5, 2, 2, 2, 2, 4}
};

int rook_8_4[8][8] = {
    {5, 2, 2, 2, 2, 2, 5, 5},
    {5, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 5},
    {5, 5, 2, 2, 2, 2, 2, 5}
};

int rook_8_2[8][8] = {
    {5, 2, 2, 2, 2, 5, 5, 5},
    {5, 0, 0, 0, 0, 0, 0, 2},
    {5, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 5},
    {2, 0, 0, 0, 0, 0, 0, 5},
    {5, 5, 5, 2, 2, 2, 2, 5}
};

int evaluation(const Board& b)
{
    int eval = 0;

    // piece values for white
    if(b.in_check() && b.data.player_to_play == BLACK) eval += 10; 
    if(b.data.w_king!=DEAD) eval += 100;
    if(b.data.w_rook_1!=DEAD)
    {
        eval += 60;
        int x = getx(b.data.w_rook_1);
        int y = gety(b.data.w_rook_1);
        if(b.data.board_type==SEVEN_THREE) eval += rook_7_3[6-y][x];
        if(b.data.board_type==EIGHT_FOUR) eval += rook_8_4[7-y][x];
        if(b.data.board_type==EIGHT_TWO) eval += rook_8_2[7-y][x];
    }
    if(b.data.w_rook_2!=DEAD)
    {
        eval += 60;
        int x = getx(b.data.w_rook_2);
        int y = gety(b.data.w_rook_2);
        if(b.data.board_type==SEVEN_THREE) eval += rook_7_3[6-y][x];
        if(b.data.board_type==EIGHT_FOUR) eval += rook_8_4[7-y][x];
        if(b.data.board_type==EIGHT_TWO) eval += rook_8_2[7-y][x];
    }
    if(b.data.w_bishop!=DEAD)
    { 
        if(b.data.board_type==SEVEN_THREE) eval += 40;
        if(b.data.board_type==EIGHT_FOUR) eval += 40;
        if(b.data.board_type==EIGHT_TWO) eval += 35;
    }
    if(b.data.w_knight_1!=DEAD) eval += 40;
    if(b.data.w_knight_2!=DEAD) eval += 40;
    if(b.data.w_pawn_1!=DEAD)
    {
        int x = getx(b.data.w_pawn_1);
        int y = gety(b.data.w_pawn_1);
        if(b.data.board_0[b.data.w_pawn_1] == PAWN_ROOK) eval += 60;
        else if(b.data.board_0[b.data.w_pawn_1] == PAWN_BISHOP) eval += 40;
        else
        {
            if(b.data.board_type != EIGHT_TWO)
            {
                if(y<=1) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
            else
            {
                if(y<=2) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
        }
    }
    if(b.data.w_pawn_2!=DEAD)
    {
        int x = getx(b.data.w_pawn_2);
        int y = gety(b.data.w_pawn_2);
        if(b.data.board_0[b.data.w_pawn_2] == PAWN_ROOK) eval += 60;
        else if(b.data.board_0[b.data.w_pawn_2] == PAWN_BISHOP) eval += 40;
        else
        {
            if(b.data.board_type != EIGHT_TWO)
            {
                if(y<=1) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
            else
            {
                if(y<=2) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
        }
    }
    if(b.data.w_pawn_3!=DEAD)
    {
        int x = getx(b.data.w_pawn_3);
        int y = gety(b.data.w_pawn_3);
        if(b.data.board_0[b.data.w_pawn_3] == PAWN_ROOK) eval += 60;
        else if(b.data.board_0[b.data.w_pawn_3] == PAWN_BISHOP) eval += 40;
        else
        {
            if(b.data.board_type != EIGHT_TWO)
            {
                if(y<=1) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
            else
            {
                if(y<=2) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
        }
    }
    if(b.data.w_pawn_4!=DEAD)
    {
        int x = getx(b.data.w_pawn_4);
        int y = gety(b.data.w_pawn_4);
        if(b.data.board_0[b.data.w_pawn_4] == PAWN_ROOK) eval += 60;
        else if(b.data.board_0[b.data.w_pawn_4] == PAWN_BISHOP) eval += 40;
        else
        {
            if(b.data.board_type != EIGHT_TWO)
            {
                if(y<=1) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
            else
            {
                if(y<=2) eval += 20 + 2*y - x;
                else eval += 20 + 2*y + x;
            }
        }
    }

    // piece values for black
    if(b.in_check() && b.data.player_to_play == WHITE) eval -= 10; 
    if(b.data.b_king!=DEAD) eval -= 100;
    if(b.data.b_rook_1!=DEAD)
    {
        eval -= 60;
        int x = getx(b.data.b_rook_1);
        int y = gety(b.data.b_rook_1);
        if(b.data.board_type==SEVEN_THREE) eval -= rook_7_3[6-y][x];
        if(b.data.board_type==EIGHT_FOUR) eval -= rook_8_4[7-y][x];
        if(b.data.board_type==EIGHT_TWO) eval -= rook_8_2[7-y][x];
    }
    if(b.data.b_rook_2!=DEAD)
    {
        eval -= 60;
        int x = getx(b.data.b_rook_2);
        int y = gety(b.data.b_rook_2);
        if(b.data.board_type==SEVEN_THREE) eval -= rook_7_3[6-y][x];
        if(b.data.board_type==EIGHT_FOUR) eval -= rook_8_4[7-y][x];
        if(b.data.board_type==EIGHT_TWO) eval -= rook_8_2[7-y][x];
    }
    if(b.data.b_bishop!=DEAD)
    {
        if(b.data.board_type==SEVEN_THREE) eval -= 40;
        if(b.data.board_type==EIGHT_FOUR) eval -= 40;
        if(b.data.board_type==EIGHT_TWO) eval -= 35;
    }
    if(b.data.b_knight_1!=DEAD) eval -= 40;
    if(b.data.b_knight_2!=DEAD) eval -= 40;
    if(b.data.b_pawn_1!=DEAD)
    {
        int x = getx(b.data.b_pawn_1);
        int y = gety(b.data.b_pawn_1);
        if(b.data.board_0[b.data.b_pawn_1] == PAWN_ROOK) eval -= 60;
        else if(b.data.board_0[b.data.b_pawn_1] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(b.data.board_type==SEVEN_THREE)
            {
                if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
                else eval -= (20 + 2*(6-y) + (6-x));
            }
            else if(b.data.board_type==EIGHT_FOUR)
            {
                if(y<=1) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
            else
            {
                if(y<=2) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
        }
    }
    if(b.data.b_pawn_2!=DEAD)
    {
        int x = getx(b.data.b_pawn_2);
        int y = gety(b.data.b_pawn_2);
        if(b.data.board_0[b.data.b_pawn_2] == PAWN_ROOK) eval -= 60;
        else if(b.data.board_0[b.data.b_pawn_2] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(b.data.board_type==SEVEN_THREE)
            {
                if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
                else eval -= (20 + 2*(6-y) + (6-x));
            }
            else if(b.data.board_type==EIGHT_FOUR)
            {
                if(y<=1) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
            else
            {
                if(y<=2) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
        }
    }
    if(b.data.b_pawn_3!=DEAD)
    {
        int x = getx(b.data.b_pawn_3);
        int y = gety(b.data.b_pawn_3);
        if(b.data.board_0[b.data.b_pawn_3] == PAWN_ROOK) eval -= 60;
        else if(b.data.board_0[b.data.b_pawn_3] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(b.data.board_type==SEVEN_THREE)
            {
                if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
                else eval -= (20 + 2*(6-y) + (6-x));
            }
            else if(b.data.board_type==EIGHT_FOUR)
            {
                if(y<=1) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
            else
            {
                if(y<=2) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
        }
    }
    if(b.data.b_pawn_4!=DEAD)
    {
        int x = getx(b.data.b_pawn_4);
        int y = gety(b.data.b_pawn_4);
        if(b.data.board_0[b.data.b_pawn_4] == PAWN_ROOK) eval -= 60;
        else if(b.data.board_0[b.data.b_pawn_4] == PAWN_BISHOP) eval -= 40;
        else
        {
            if(b.data.board_type==SEVEN_THREE)
            {
                if(y<=1) eval -= (20 + 2*(6-y) - (6-x));
                else eval -= (20 + 2*(6-y) + (6-x));
            }
            else if(b.data.board_type==EIGHT_FOUR)
            {
                if(y<=1) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
            else
            {
                if(y<=2) eval -= (20 + 2*(7-y) - (7-x));
                else eval -= (20 + 2*(7-y) + (7-x));
            }
        }
    }

    return eval;
}

bool white(U16 &move1,U16 &move2,const Board &b)
{
    auto nb1 = b;
    nb1.do_move_(move1);
    int eval1 = evaluation(nb1);
    auto nb2 = b;
    nb2.do_move_(move2);
    int eval2 = evaluation(nb2);
    return eval1>eval2;
}

bool black(U16 &move1,U16 &move2,const Board &b)
{
    auto nb1 = b;
    nb1.do_move_(move1);
    int eval1 = evaluation(nb1);
    auto nb2 = b;
    nb2.do_move_(move2);
    int eval2 = evaluation(nb2);
    return eval1<eval2;
}

void ordering(std::vector<U16>& v,const Board &b)
{
    if(b.data.player_to_play==WHITE)
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

std::pair<U16,int> alpha_beta(const Board& b,int depth,int alpha,int beta,int maxdepth,Engine* e,std::multiset<std::string> &pos,const std::chrono::high_resolution_clock::time_point& search_start,std::chrono::duration<double, std::milli> time_limit)
{
    auto moveset = b.get_legal_moves();
    if(moveset.size()==0) 
    {
        if(!b.in_check()) return {0,0};
        if(b.data.player_to_play == WHITE) return {0,INT_MIN};
        if(b.data.player_to_play == BLACK) return {0,INT_MAX};
    }
    if(depth == maxdepth) return {0,evaluation(b)};

    if(b.data.player_to_play == WHITE)
    {
        int eval = INT_MIN;

        std::vector<U16> moves;
        for(auto m:moveset) moves.push_back(m);
        ordering(moves,b);
        U16 move = moves[0];

        int sz = moves.size();
        // int sz = (7*moves.size())/10;
        // if(b.data.board_type == EIGHT_FOUR) sz = (5*moves.size())/10;
        for(int i=0;i<std::max(1,sz);i++)
        {
            if(std::chrono::high_resolution_clock::now()-search_start>time_limit) break;
            auto nb = b;
            nb.do_move_(moves[i]);
            
            std::pair<U16,int> node;
            std::string position = all_boards_to_str(nb);
            pos.insert(position);
            if(gpos.count(position) + pos.count(position)==3) node = {moves[i],0};
            else node = alpha_beta(nb,depth+1,alpha,beta,maxdepth,e,pos,search_start,time_limit);
            pos.erase(pos.find(position));
            
            if(node.second>eval)
            {
                eval = node.second;
                move = moves[i];
            }
            if(depth == 0)
            {
                std::cout<<"Maxdepth : "<<maxdepth<<", eval : "<<node.second<<", move : "<<move_to_str(moves[i])<<'\n';
            }
            alpha = std::max(alpha,eval);
            if(beta <= alpha) break;
        }
        if(std::chrono::high_resolution_clock::now()-search_start>time_limit) return {1<<15,0};
        return {move,eval};
    }
    else
    {
        int eval = INT_MAX;

        std::vector<U16> moves;
        for(auto m:moveset) moves.push_back(m);
        ordering(moves,b);
        U16 move = moves[0];

        int sz = moves.size();
        // int sz = (7*moves.size())/10;
        // if(b.data.board_type == EIGHT_FOUR) sz = (5*moves.size())/10;
        for(int i=0;i<std::max(1,sz);i++)
        {
            if(std::chrono::high_resolution_clock::now()-search_start>time_limit) break;
            auto nb = b;
            nb.do_move_(moves[i]);

            std::pair<U16,int> node;
            std::string position = all_boards_to_str(nb);
            pos.insert(position);
            if(gpos.count(position) + pos.count(position)==3) node = {moves[i],0};
            else node = alpha_beta(nb,depth+1,alpha,beta,maxdepth,e,pos,search_start,time_limit);
            pos.erase(pos.find(position));

            if(node.second<eval)
            {
                eval = node.second;
                move = moves[i];
            }
            if(depth == 0)
            {
                std::cout<<"Maxdepth : "<<maxdepth<<", eval : "<<node.second<<", move : "<<move_to_str(moves[i])<<'\n';
            }
            beta = std::min(beta,eval);
            if(beta <= alpha) break;
        }
        if(std::chrono::high_resolution_clock::now()-search_start>time_limit) return {1<<15,0};
        return {move,eval};
    }
}

void Engine::find_best_move(const Board& b) {

    gpos.insert(all_boards_to_str(b));
    std::multiset<std::string> pos;

    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) this->best_move = 0;
    else {
        std::vector<U16> moves;
        std::sample(
            moveset.begin(),
            moveset.end(),
            std::back_inserter(moves),
            1,
            std::mt19937{std::random_device{}()}
        );
        this->best_move = moves[0];
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    for(int depth = 2;depth<21;depth++)
    {
        auto search_start = std::chrono::high_resolution_clock::now();

        if(b.data.board_type==SEVEN_THREE)
        {
            std::chrono::duration<double, std::milli> time_limit_7_3(2000.0);
            if(time_left < std::chrono::duration<double, std::milli>(30000.0)) time_limit_7_3 = std::chrono::duration<double, std::milli>(1000.0);
            if(time_left < std::chrono::duration<double, std::milli>(5000.0)) time_limit_7_3 = std::chrono::duration<double, std::milli>(200.0);

            auto p = alpha_beta(b,0,INT_MIN,INT_MAX,depth,this,pos,search_start,time_limit_7_3);
            auto current_time = std::chrono::high_resolution_clock::now()-start_time;
            if(current_time>time_limit_7_3) break;
            this->best_move = p.first;
        }
        else if(b.data.board_type==EIGHT_FOUR)
        {   
            std::chrono::duration<double, std::milli> time_limit_8_4(3000.0);
            if(time_left < std::chrono::duration<double, std::milli>(90000.0)) time_limit_8_4 = std::chrono::duration<double, std::milli>(2000.0);
            if(time_left < std::chrono::duration<double, std::milli>(30000.0)) time_limit_8_4 = std::chrono::duration<double, std::milli>(1000.0);
            if(time_left < std::chrono::duration<double, std::milli>(5000.0)) time_limit_8_4 = std::chrono::duration<double, std::milli>(200.0);

            auto p = alpha_beta(b,0,INT_MIN,INT_MAX,depth,this,pos,search_start,time_limit_8_4);
            auto current_time = std::chrono::high_resolution_clock::now()-start_time;
            if(current_time>time_limit_8_4) break;
            this->best_move = p.first;
        }
        else
        {
            std::chrono::duration<double, std::milli> time_limit_8_2(4000.0);
            if(time_left < std::chrono::duration<double, std::milli>(150000.0)) time_limit_8_2 = std::chrono::duration<double, std::milli>(3000.0);
            if(time_left < std::chrono::duration<double, std::milli>(90000.0)) time_limit_8_2 = std::chrono::duration<double, std::milli>(2000.0);
            if(time_left < std::chrono::duration<double, std::milli>(30000.0)) time_limit_8_2 = std::chrono::duration<double, std::milli>(1000.0);
            if(time_left < std::chrono::duration<double, std::milli>(5000.0)) time_limit_8_2 = std::chrono::duration<double, std::milli>(200.0);

            auto p = alpha_beta(b,0,INT_MIN,INT_MAX,depth,this,pos,search_start,time_limit_8_2);
            auto current_time = std::chrono::high_resolution_clock::now()-start_time;
            if(current_time>time_limit_8_2) break;
            this->best_move = p.first;
        }
    }
    auto cb = b;
    cb.do_move_(this->best_move);
    gpos.insert(all_boards_to_str(cb));
}
