#ifndef _LINK_GAME_H_
#define _LINK_GAME_H_

#include <functional>

#include "link_game_type.h"

namespace plug
{
class  LinkGameEraser: public LinkGame
{
public:
    typedef std::function<bool (const POINT p1, const POINT p2)> ClickCallback;
    LinkGameEraser(const ClickCallback& callback);
    ~LinkGameEraser();

    // ถþฮฌสýื้
    //virtual void SetBoardData(const BYTE** data, size_t x, size_t y) override;
    virtual void SetBoardData(BYTE(*data)[19]) override;
    virtual bool ClearPair() override;

private:
    bool CheckLine(POINT first, POINT secode);
    bool CheckTwoPoint(POINT first, POINT secode);
    void ClickTwoPoint(POINT first, POINT secode);

    BYTE buffer_[11][19];
    ClickCallback callback_;
};

}
#endif