#ifndef PLAYINGBOARD_H
#define PLAYINGBOARD_H

#include <utility>
#include <vector>
#include <map>

struct boardState{
    bool state;
    std::pair<int,int> p1;
    std::pair<int,int> p2;

    bool operator==(bool a){
        return state==a;
    }
};
struct maskIndex{
    std::vector<int> pos;
    std::vector<int> neg;
    std::vector<int> normal;
    std::vector<int> extra;
    std::vector<int> half;
};
struct fieldWeight{
    int size;
    int points;
    maskIndex index;
    std::vector<int> mask;
    };

class playingBoard{
private:
    int height;
    int width;
    int* board;
    std::map<std::pair<int,int>, std::vector<double>> priority;

    int max_size;
    std::vector<fieldWeight> knowledge;

    bool checkForWin(int x, int y);
    void recalculate(int x, int y, int m, playingBoard* b) const;
public:
    boardState win;
    playingBoard(int w, int h, std::__cxx11::string path);
    ~playingBoard();

    bool set(int x, int y, int who);
    int get(int x, int y);
    void doChumpsTurn();

};


#endif // PLAYINGBOARD_H
