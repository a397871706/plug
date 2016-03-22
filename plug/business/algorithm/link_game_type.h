#ifndef _LINK_GAME_TYPE_H_
#define _LINK_GAME_TYPE_H_

#include <wtypes.h>

namespace plug
{
struct ChessPoint
{
    ChessPoint(const POINT& p)
        : p_(p)
        , up_(p)
        , down_(p)
        , left_(p)
        , right_(p)
    {
        up_.y = p.y - 1;
        down_.y = p.y + 1;
        left_.x = p.x - 1;
        right_.x = p.x + 1;
    }

    POINT p_;
    POINT up_;
    POINT down_;
    POINT left_;
    POINT right_;
};

class LinkGame
{
public:
    virtual ~LinkGame() { }
    // ถþฮฌสýื้
    //virtual void SetBoardData(const BYTE** data, size_t x, size_t y) = 0;
    virtual void SetBoardData(BYTE (*data)[19]) = 0;
    virtual bool ClearPair() = 0;
};
}


#endif