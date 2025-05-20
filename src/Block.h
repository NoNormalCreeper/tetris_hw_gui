//
// Created by Rikka on 2025/5/16.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <qstring.h>
#include "Pos.h"
#include <utility>


class Block {
public:
    char label;
    QString color;
    std::vector<Pos> occupied; // 占用的坐标
    Pos anchor; // 旋转锚点坐标

    Block(const char label, QString color, std::vector<Pos> occupied, const Pos anchor = Pos(1, 0))
        : label(label), color(std::move(color)), occupied(std::move(occupied)), anchor(anchor) {}

    static const Block& getBlockByLabel(char label);
    const Block& rotate() const;    // 返回顺时针旋转90度的新方块
};


#endif //BLOCK_H
