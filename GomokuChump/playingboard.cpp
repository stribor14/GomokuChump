#include "playingboard.h"

#include <cmath>
#include <iostream>

#include <yaml-cpp/yaml.h>

#define mp(x,y) std::pair<int,int>(x,y)

int move[8] = {1,0, 0,1, 1,1, 1,-1};

bool playingBoard::checkForWin(int x, int y)
{
    int val = get(x,y);
    for(int k = 0; k < 4; k++){
        int x1 = x;
        int y1 = y;
        int x2 = x;
        int y2 = y;

        while(1){
            int tmp_x = x1 - move[2*k];
            int tmp_y = y1 - move[2*k+1];
            if (tmp_x < 0 || tmp_x > 25 || tmp_y < 0 || tmp_y > 25) break;
            if (get(tmp_x, tmp_y) != val) break;
            x1 = tmp_x;
            y1 = tmp_y;
        }
        while(1){
            int tmp_x = x2 + move[2*k];
            int tmp_y = y2 + move[2*k+1];
            if (tmp_x < 0 || tmp_x > 25 || tmp_y < 0 || tmp_y > 25) break;
            if (get(tmp_x, tmp_y) != val) break;
            x2 = tmp_x;
            y2 = tmp_y;
        }

        if(std::abs(x2 - x1) == 4 || std::abs(y2 - y1) == 4){
            win.state = true;
            win.p1 = std::pair<int,int>(x1, y1);
            win.p2 = std::pair<int,int>(x2, y2);
            return true;
        }
    }
    return false;
}

void playingBoard::recalculate(int x, int y, int m)
{
    auto testIt = [&](int k, int ref, std::vector<int> mask){
        bool test = true;
        int tempVal = -1;
        for(uint i = 0; i < mask.size(); i++){
            int x1 = x + (i-ref)*move[2*k];
            int y1 = y + (i-ref)*move[2*k+1];
            if(x1 < 0 || x1 > 25 || y1 < 0 || y1 > 25){
                test = false;
                break;
            }
            int val = get(x1,y1);
            switch(mask[i]){
                case 1 :
                    if(tempVal == -1) tempVal = val ? val : -1;
                    if(val != tempVal) test = false;
                    break;
                case -1 :
                    if(val != 0){
                        if(tempVal == -1) tempVal = val == 1 ? 2 : 1;
                        else if(val == tempVal) test = false;
                    }
                    break;
                case -3 :
                case 0 :
                case 3 :
                    if(val != 0) test = false;
                    break;
            }
            if(!test) break;
        }
        if(test) return true;

        test = true;
        tempVal = -1;

        for(uint i = 0; i < mask.size(); i++){
            int x1 = x - (i-ref)*move[2*k];
            int y1 = y - (i-ref)*move[2*k+1];
            if(x1 < 0 || x1 > 25 || y1 < 0 || y1 > 25){
                test = false;
                break;
            }
            int val = get(x1,y1);
            switch(mask[i]){
                case 1 :
                    if(tempVal == -1) tempVal = val ? val : -1;
                    if(val != tempVal) test = false;
                    break;
                case -3 :
                    if(val != 0){
                        if(tempVal == -1) tempVal = val == 1 ? 2 : 1;
                        else if(val == tempVal) test = false;
                    }
                    break;
                case -1 :
                case 0 :
                case 3 :
                    if(val != 0) test = false;
                    break;
            }
            if(!test) break;
        }
        return test;
    };

    if(priority.find(mp(x,y)) == priority.end()){
        std::vector<double> temp(4,0);
        priority[mp(x,y)] = temp;
    }
    bool found = false;
    priority[mp(x,y)][m] = 0;
    for(fieldWeight& test : knowledge){
        for(int& ref : test.index.extra)
            if(testIt(m, ref, test.mask)){
                found = true;
                priority[mp(x,y)][m] = test.points * 2;
                break;
            }
         if(found) break;
         for(int& ref : test.index.half)
             if(testIt(m, ref, test.mask)){
                 found = true;
                 priority[mp(x,y)][m] = (double)test.points / 2;
                 break;
             }
          if(found) break;
          for(int& ref : test.index.normal)
              if(testIt(m, ref, test.mask)){
                  found = true;
                  priority[mp(x,y)][m] = test.points;
                  break;
              }
           if(found) break;
    }
}

playingBoard::playingBoard(int w, int h, std::string path) : height(h), width(w){
    board = new int[w*h]();
    win.state = false;
    max_size = 0;

    auto node = YAML::LoadFile(path.c_str());
    for(auto type : node){
        fieldWeight temp;
        std::string typeName = type.first.as<std::string>();
        temp.points = node[typeName]["points"].as<int>();
        temp.size   = node[typeName]["lenght"].as<int>();
        if (temp.size > max_size) max_size = temp.size;
        for(uint i = 0; i < node[typeName]["moves"].size(); i++){
            maskIndex tempIndex;
            std::vector<int> tempMask;
            for(uint j = 0; j < node[typeName]["moves"][i].size(); j++){
                tempMask.push_back(node[typeName]["moves"][i][j].as<int>());
                switch(tempMask[j]){
                case -1 :
                    tempIndex.neg.push_back(j);
                    break;
                case -3 :
                    tempIndex.half.push_back(j);
                    break;
                case 0 :
                    tempIndex.normal.push_back(j);
                    break;
                case 1 :
                    tempIndex.pos.push_back(j);
                    break;
                case 3 :
                    tempIndex.extra.push_back(j);
                    break;
                }
            }
            temp.index = tempIndex;
            temp.mask = tempMask;
            knowledge.push_back(temp);
        }
    }
    std::sort(knowledge.begin(),knowledge.end(),[](fieldWeight f1, fieldWeight f2){return f1.points > f2.points;});
}

playingBoard::~playingBoard(){
    //delete board;
}

bool playingBoard::set(int x, int y, int who){
    if(board[x*height + y]) return false;
    board[x*height + y] = who;
    if(priority.find(mp(x,y)) != priority.end()) priority.erase(mp(x,y));
    if(checkForWin(x,y)) return true;
    for(int k=1; k<max_size; k++)
        for(int i = 0; i < 4; i++){
            int x1 = x + k*move[2*i];
            int y1 = y + k*move[2*i+1];
            int x2 = x - k*move[2*i];
            int y2 = y - k*move[2*i+1];
            if(x1 > -1 && y1 > -1 && x1 < 26 && y2 < 26)
                if (get(x1, y1) == 0) recalculate(x1, y1, i);
            if(x2 > -1 && y2 > -1 && x2 < 26 && y2 < 26)
                if (get(x2, y2) == 0) recalculate(x2, y2, i);
        }

    return true;
}

int playingBoard::get(int x, int y){
    return board[x*height + y];
}

void playingBoard::doChumpsTurn()
{
    auto play = std::max_element(priority.begin(), priority.end(),
                                 [](std::pair<std::pair<int,int> ,std::vector<double>> p1,
                                 std::pair<std::pair<int,int> ,std::vector<double>> p2){
                                    return std::accumulate(p1.second.begin(),p1.second.end(),0) < std::accumulate(p2.second.begin(),p2.second.end(),0);});
    //std::cout << play->first.first << " " <<  play->first.second << " - " << std::accumulate(play->second.begin(),play->second.end(),0) << std::endl;
    set(play->first.first, play->first.second, 2);
}
