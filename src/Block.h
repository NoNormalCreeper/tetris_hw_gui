//
// Created by Rikka on 2025/5/16.
//

#ifndef BLOCK_H
#define BLOCK_H
#include "Pos.h"
#include <qstring.h>
#include <utility>

class Block {
public:
    char label;
    QString color;
    std::vector<Pos> occupied; // 占用的坐标
    Pos anchor;                // 旋转锚点坐标

    Block(const char label, QString color, std::vector<Pos> occupied,
          const Pos anchor = Pos(1, 0))
        : label(label), color(std::move(color)), occupied(std::move(occupied)),
          anchor(anchor) {}

    static const Block& getBlockByLabel(char label);

    [[nodiscard]] std::unique_ptr<Block> rotate() const; // 返回顺时针旋转90度的新方块
};

namespace k_Block {
// 方块类型常量
const auto I = Block('I', "lightblue", {Pos(0,0), Pos(1,0), Pos(2,0), Pos(3,0)}, Pos(1,0)); 
const auto L = Block('L', "orange",    {Pos(0,1), Pos(1,1), Pos(2,1), Pos(2,0)}, Pos(1,1));
const auto J = Block('J', "blue",      {Pos(0,0), Pos(0,1), Pos(1,1), Pos(2,1)}, Pos(1,1));
const auto O = Block('O', "yellow",    {Pos(0,0), Pos(1,0), Pos(0,1), Pos(1,1)}, Pos(0,0)); 
const auto S = Block('S', "green",     {Pos(1,0), Pos(2,0), Pos(0,1), Pos(1,1)}, Pos(1,1));
const auto T = Block('T', "purple",    {Pos(1,0), Pos(0,1), Pos(1,1), Pos(2,1)}, Pos(1,1));
const auto Z = Block('Z', "red",       {Pos(0,0), Pos(1,0), Pos(1,1), Pos(2,1)}, Pos(1,1));

const auto list = std::vector<Block>{I, L, J, O, S, T, Z}; 

} // namespace k_Block

struct Action {
    std::unique_ptr<Block> block;
    Pos anchor;
    void reset() { block.reset(); anchor = Pos(0, 0); }
};



#endif //BLOCK_H
