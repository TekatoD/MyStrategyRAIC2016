/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#pragma once


#include "Point.h"
#include "Sector.h"

template <class T, size_t W, size_t H>
class SectorMap {
public:
    SectorMap(const Point& origin, double cellSize, const Point& offset = {0, 0}, const T& defaultValue = T())
            : mOrigin(origin), mOffset(offset), mCellSize(cellSize), mDefaultValue(defaultValue) {}

    void setCellValue(const Point& point, bool value) {
        Point local{point - mOffset - mOrigin};
        int x = (int) local.getX() / mCellSize;
        int y = (int) local.getY() / mCellSize;
        this->setCellValue(x, y, value);
    }

    bool getCellValue(const Point& point) const {
        Point local{point - mOffset - mOrigin};
        int x = (int) local.getX() / mCellSize;
        int y = (int) local.getY() / mCellSize;
        return mSector.isValid(x, y) && mSector.getValue(x, y);
    }

    void setCellValue(int x, int y, const T& value) {
        if (mSector.isValid(x, y)) mSector.setValue(x, y, value);
    }

    T getCellValue(int x, int y) const {
        return mSector.isValid(x, y) && mSector.getValue(x, y);
    }


    Point getGlobalPosition(int x, int y) const {
        return mOrigin + mOffset + this->getLocalPosition(x, y);
    }

    Point getLocalPosition(int x, int y) const {
        return Point(x, y) * mCellSize;
    }

    Point getLocalPoint(const Point& point) const {
        return point - (mOrigin + mOffset);
    }

    bool isContains(const Point& point) const {
        return this->isContains(point.getX(), point.getY());
    }

    bool isContains(double x, double y) const {
        this->shift(x, y);
        return    (x >= this->getLeft())   && (x <= this->getRight())
                  && (y >= this->getBottom()) && (y <= this->getTop());
    }

    double getRight() const {
        return mOrigin.getX() + W * mCellSize;
    }

    double getLeft() const {
        return mOrigin.getX();
    }

    double getTop() const {
        return mOrigin.getY() + H * mCellSize;
    }

    double getBottom() const {
        return mOrigin.getY();
    }

    const Point& getOffset() const {
        return mOffset;
    }


    void setOffset(const Point& offset) {
        mOffset = offset;
        mSector.setIndexOffset((int) offset.getX() / mCellSize,
                               (int) offset.getY() / mCellSize);
    }

    Point normalize(const Point& point) const {
        if (this->isContains(point)) return point;
        Point local{ this->getLocalPoint(point) };
        double maxValue = (std::abs(local.getX()) > std::abs(local.getY()))
                          ? local.getX()
                          : local.getY();
        double maxSide = (W > H) ? W : H;
        double factor = maxValue / (mCellSize * maxSide);
        local /= factor;
        return local + mOffset + mOffset;
    }

    void reset() {
        mSector.fill(mDefaultValue);
    }

    T getDefaultValue() const {
        return mDefaultValue;
    }

    void setDefaultValue(const T& defaultValue) {
        mDefaultValue = defaultValue;
    }

protected:
    void shift(double& x, double& y) const {
        x -= mOffset.getX();
        y -= mOffset.getY();
    }


protected:
    Point mOrigin;
    Point mOffset;
    double mCellSize;
    T mDefaultValue;
    Sector<T, W, H> mSector;
};