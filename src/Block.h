//
// Created by Rikka on 2025/5/16.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <qstring.h>
#include "Pos.h"
#include <utility>


class Block
{
public:
    char label;
    QString color;
    std::vector<Pos> occupied; // 占用的坐标
    Pos anchor; // 旋转锚点坐标

    Block(const char label, QString color, std::vector<Pos> occupied, const Pos anchor = Pos(1, 0)) :
        label(label), color(std::move(color)), occupied(std::move(occupied)), anchor(anchor)
    {
    }

    static const Block &getBlockByLabel(char label);

    [[nodiscard]] Block rotate() const; // 返回顺时针旋转90度的新方块
};

namespace k_Block
{
    // 方块类型常量
    const auto I = Block('I', "lightblue", {Pos(0, 0), Pos(1, 0), Pos(2, 0), Pos(3, 0)});
    // TODO: 补全其他方块类型

    const auto list = std::vector{I};
}

using Action = struct
{
    // 玩家一次操作的动作
    const Block *block; // 旋转后的当前方块
    Pos anchor;
};


#endif //BLOCK_H
