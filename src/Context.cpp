//
// Created by Rikka on 2025/5/21.
//

#include "Context.h"

Context::Context() : game(Game()), repository(Repository("game_data.json")) {
    // TODO: 这里的路径需要根据实际情况修改
}
