#include "link_game.h"

plug::LinkGameEraser::LinkGameEraser(const ClickCallback& callback)
    : LinkGame()
    , callback_(callback)
{
    memset(buffer_, 0, 11 * 19);
}

plug::LinkGameEraser::~LinkGameEraser()
{

}

bool plug::LinkGameEraser::ClearPair()
{
    POINT left = {};
    POINT right = {};
    for (int y1 = 0; y1 < 11; ++y1)
    {
        for (int x1 = 0; x1 < 19; ++x1)
        {
            for (int y2 = y1; y2 < 11; ++y2)
            {
                for (int x2 = 0; x2 < 19; ++x2)
                {
                    if ((buffer_[y1][x1] == buffer_[y2][x2]) &&
                        (!((x1 == x2) && (y1 == y2))))
                    {
                        left.x = x1;
                        left.y = y1;
                        right.x = x2;
                        right.y = y2;
                        if (CheckTwoPoint(left, right))
                        {
                            ClickTwoPoint(left, right);
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool plug::LinkGameEraser::CheckLine(POINT first, POINT secode)
{
    if ((first.x == secode.x) && (first.y == secode.y) &&
        (buffer_[first.y][first.x] == 0) &&
        (buffer_[secode.y][secode.x] == 0))
    {
        return true;
    }
    else if ((first.x < 0) || (first.x > 18) || (first.y < 0) || (first.y > 10) ||
             (secode.x < 0) || (secode.x > 18) || (secode.y < 0) || (secode.y > 10))
    {
        return false;
    }

    int x = 0;
    int y = 0;
    int temp = 0;
    if (first.x == secode.x)
    {
        if (first.y > secode.y)
        {
            temp = first.y;
            first.y = secode.y;
            secode.y = temp;
        }

        for (y = first.y; y <= secode.y; ++y)
        {
            if (buffer_[y][first.x] != 0)
                return false;
        }
    }
    else if (first.y == secode.y)
    {
        for (x = first.x; x <= secode.x; ++x)
        {
            if (buffer_[first.y][x] != 0)
                return false;
        }
    }

    return true;
}

/*void plug::LinkGameEraser::SetBoardData(const BYTE** data, size_t x, size_t y)
{
}*/

void plug::LinkGameEraser::SetBoardData(BYTE(*data)[19])
{
    if (!data)
        return;

    memcpy(buffer_, data, 11 * 19);
}

bool plug::LinkGameEraser::CheckTwoPoint(POINT first, POINT secode)
{
    plug::ChessPoint pLeft(first);
    plug::ChessPoint pRight(secode);
    POINT tempLeft = {};
    POINT tempRight = {};
    int left = 0;
    int right = 0;
    if ((first.x == secode.x) && (first.y == secode.y))
    {
        return false;
    }
    else if (buffer_[first.y][first.x] == 0 || buffer_[secode.y][secode.x] == 0)
    {
        return false;
    }
    else if (buffer_[first.y][first.x] != buffer_[secode.y][secode.x])
    {
        return false;
    }

    tempLeft = first;
    tempRight = secode;
    if (first.y == secode.y)
    {
        if ((pLeft.right_.x == pRight.p_.x) || (pLeft.left_.x == pRight.p_.x))
        {
            return true;
        }

        if (CheckLine(pLeft.right_, pRight.left_))
            return true;

        tempLeft = first;
        tempRight = secode;
        if ((pLeft.up_.y >= 0) && (pLeft.up_.y <= 10))
        {
            for (right = 0; right < pLeft.up_.y; ++right)
            {
                tempLeft.y = right;
                tempRight.y = right;
                if (CheckLine(tempLeft, pLeft.up_) &&
                    CheckLine(tempRight, pRight.up_) &&
                    CheckLine(tempLeft, tempRight))
                    return true;
            }
        }
        tempLeft = first;
        tempRight = secode;
        if (pLeft.down_.y >= 0 && (pLeft.down_.y <= 10))
        {
            for (right = pLeft.down_.y; right <= 10; ++right)
            {
                tempLeft.y = right;
                tempRight.y = right;
                if (CheckLine(tempLeft, pLeft.down_) &&
                    CheckLine(tempRight, pRight.down_) &&
                    CheckLine(tempLeft, tempRight))
                {
                    return true;
                }
            }
        }
    }
    else if (first.x == secode.x)
    {
        if ((pLeft.down_.y == pRight.p_.y) || (pLeft.up_.y == pRight.p_.y))
        {
            return true;
        }

        if (CheckLine(pLeft.down_, pRight.up_))
        {
            return true;
        }

        tempLeft = first;
        tempRight = secode;
        for (left = 0; left <= pLeft.left_.x; ++left)
        {
            tempLeft.x = left;
            tempRight.x = left;
            if (CheckLine(tempLeft, pLeft.left_) &&
                CheckLine(tempRight, pRight.left_) &&
                CheckLine(tempLeft, tempRight))
            {
                return true;
            }

            tempLeft = first;
            tempRight = secode;
            for (left = pLeft.right_.x; left <= 18; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;

                if (CheckLine(tempLeft, pLeft.right_) &&
                    CheckLine(tempRight, pRight.right_) &&
                    CheckLine(tempLeft, tempRight))
                {
                    return true;
                }
            }
        }
        if (CheckLine(pLeft.down_, pRight.up_))
            return true;
    }
    else
    {
        tempLeft = first;
        tempRight = secode;
        if (first.x > secode.x)
        {
            for (left = 0; left <= pRight.left_.x; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;
                if (CheckLine(tempLeft, pLeft.left_) &&
                    CheckLine(tempLeft, tempRight) &&
                    CheckLine(tempRight, pRight.left_))
                {
                    return true;
                }
            }

            for (left = pRight.right_.x; left <= pLeft.left_.x; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;
                if (CheckLine(pRight.right_, tempRight) &&
                    CheckLine(tempLeft, tempRight) &&
                    CheckLine(tempLeft, pLeft.left_))
                {
                    return true;
                }
            }

            for (left = pRight.right_.x; left <= 18; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;
                if (CheckLine(pLeft.right_, tempLeft) &&
                    CheckLine(pRight.right_, tempRight) &&
                    CheckLine(tempLeft, tempRight))
                {
                    return true;
                }
            }

            tempLeft.x = first.x;
            tempRight.x = secode.x;
            for (right = 0; right <= pLeft.up_.y; ++right)
            {
                tempLeft.y = right;
                tempRight.y = right;
                if (CheckLine(tempRight, tempLeft) &&
                    CheckLine(tempLeft, pLeft.up_) &&
                    CheckLine(tempRight, pRight.up_))
                {
                    return true;
                }
            }

            for (right = pLeft.down_.y; right <= pRight.up_.y; ++right)
            {
                tempRight.y = right;
                tempLeft.y = right;
                if (CheckLine(tempRight, tempLeft) &&
                    CheckLine(pLeft.down_, tempLeft) &&
                    CheckLine(tempRight, pRight.up_))
                {
                    return true;
                }
            }

            for (right = pRight.down_.y; right <= 10; ++right)
            {
                tempLeft.y = right;
                tempRight.y = right;
                if (CheckLine(tempRight, tempLeft) &&
                    CheckLine(pLeft.down_, tempLeft) &&
                    CheckLine(pRight.down_, tempRight))
                {

                    return true;
                }
            }
        }
        else
        {
            tempLeft.y = first.y;
            tempRight.y = secode.y;
            for (left = 0; left <= pLeft.left_.x; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;
                if (CheckLine(tempLeft, tempRight) &&
                    CheckLine(tempLeft, pLeft.left_) &&
                    CheckLine(tempRight, pRight.left_))
                {
                    return true;
                }
            }

            for (left = pLeft.right_.x; left < pRight.left_.x; ++left)
            {
                tempLeft.x = left;
                tempRight.x = left;
                if (CheckLine(tempLeft, tempRight) &&
                    CheckLine(pLeft.right_, tempLeft) &&
                    CheckLine(tempRight, pRight.left_))
                {
                    return true;
                }
            }

            for (left = pRight.right_.x; left <= 18; ++left)
            {
                tempLeft.x = 0;
                tempRight.x = left;
                if (CheckLine(tempLeft, tempRight) &&
                    CheckLine(pLeft.right_, tempLeft) &&
                    CheckLine(pRight.right_, tempRight))
                {
                    return true;
                }
            }

            tempLeft.x = first.x;
            tempRight.x = secode.x;
            if ((pLeft.up_.y >= 0) && (pLeft.up_.y <= 10))
            {
                for (right = 0; right <= pLeft.up_.y; ++right)
                {
                    tempLeft.y = right;
                    tempRight.y = right;
                    if (CheckLine(tempLeft, tempRight) &&
                        CheckLine(tempLeft, pLeft.up_) &&
                        CheckLine(tempRight, pRight.up_))
                    {
                        return true;
                    }
                }
            }

            tempLeft.x = first.x;
            tempRight.x = secode.x;
            if ((pLeft.down_.y <= 10) && (pRight.up_.y >= 0))
            {
                for (right = pLeft.down_.y; right <= pRight.up_.y; ++right)
                {
                    tempLeft.y = right;
                    tempRight.y = right;
                    if (CheckLine(tempLeft, tempRight) &&
                        CheckLine(pLeft.down_, tempLeft) &&
                        CheckLine(tempRight, pRight.up_))
                    {
                        return true;
                    }
                }
            }

            tempLeft.x = first.x;
            tempRight.x = secode.x;
            if (pRight.down_.y <= 10)
            {
                for (right = pRight.down_.y; right <= 10; ++right)
                {
                    tempLeft.y = right;
                    tempRight.y = right;
                    if (CheckLine(tempLeft, tempRight) &&
                        CheckLine(pLeft.down_, tempLeft) &&
                        CheckLine(pRight.down_, tempRight))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void plug::LinkGameEraser::ClickTwoPoint(POINT first, POINT secode)
{
    if (callback_)
        callback_(first, secode);
}
