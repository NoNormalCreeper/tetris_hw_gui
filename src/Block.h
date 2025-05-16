//
// Created by Rikka on 2025/5/16.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <qstring.h>
#include <utility>


class Block {
public:
    char label;
    QString color;
    std::vector<std::tuple<int, int>> occupied; // 占用的坐标

    template<typename... Tuples>
    Block(char label, QString color, Tuples... tuples);
};


#endif //BLOCK_H
