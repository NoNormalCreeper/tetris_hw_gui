//
// Created by Rikka on 2025/5/21.
//

#ifndef CONTEXT_H
#define CONTEXT_H
#include "Game.h"
#include "Repository.h"

enum Status {
    MAIN_MENU,
    PLAYING,
    PAUSE,
    GAME_OVER,
    SETTINGS_MENU,
    LEADERBOARD,   
    INSTRUCTIONS 
}; // TODO: to be coutinued...

class Context {
public:
    Status status = MAIN_MENU; // 游戏状态，默认为主菜单
    Game game;                 // 游戏对象
    Repository repository;     // 数据库对象

    Context(); // TODO
    void reset();
};

#endif //CONTEXT_H
