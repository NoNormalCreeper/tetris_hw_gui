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

    Block(const char label, QString color, std::vector<Pos> occupied, Pos anchor = Pos(1, 0))
        : label(label), color(std::move(color)), occupied(std::move(occupied)), anchor(std::move(anchor)) {}

    const Block& getBlockByLabel(const char label) const;
};


#endif //BLOCK_H
