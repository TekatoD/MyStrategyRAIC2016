/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#pragma once


#include <ostream>
#include "Point.h"
#include "Grid.h"

template <class T, size_t W, size_t H>
class GridMap {
public:
    GridMap(const Point& origin, double cellSize, const Point& offset = {0, 0}, const T& defaultValue = T())
            : mOrigin(origin), mOffset(offset), mCellSize(cellSize), mDefaultValue(defaultValue) {}

    Point getNearestPoint(Point point) const {
        if (!this->isContainsPoint(point)) {
            point = Point{this->normalize(point.getX(), this->getLeft(), this->getRight()),
                          this->normalize(point.getY(), this->getBottom(), this->getTop())};
        }
        point -= mOffset;
        point -= mOrigin;
        point /= mCellSize;
        return this->getGlobalPosition(point.getX(), point.getY());
        
    }

    void setCellValue(const Point& point, const T& value) {
        if (!this->isContainsPoint(point)) return;
        Point local{point - mOffset - mOrigin};
        int x = (int) std::round(local.getX() / mCellSize);
        int y = (int) std::round(local.getY() / mCellSize);
        this->setCellValue(x, y, value);
    }

    T getCellValue(const Point& point) const {
        Point local{point - mOffset - mOrigin};
        int x = (int) std::round(local.getX() / mCellSize);
        int y = (int) std::round(local.getY() / mCellSize);
        return mGrid.getValue(x, y);
    }

    void setCellValue(int x, int y, const T& value) {
        if (mGrid.isValidIndex(x, y)) mGrid.setValue(x, y, value);
    }

    T getCellValueByIndex(int x, int y) const {
        return mGrid.getValue(x, y);
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

    bool isContainsPoint(const Point& point) const {
        return this->isContains(point.getX(), point.getY());
    }

    bool isValidIndex(int x, int y) const {
        return mGrid.isValidIndex(x, y);
    }

    bool isContains(double x, double y) const {
        return    (x >= this->getLeft())   && (x <= this->getRight())
               && (y >= this->getBottom()) && (y <= this->getTop());
    }

    double getRight() const {
        return mOrigin.getX() + (W - 1) * mCellSize + mOffset.getX();
    }

    double getLeft() const {
        return mOrigin.getX() + mOffset.getX();
    }

    double getTop() const {
        return mOrigin.getY() + (H - 1) * mCellSize + mOffset.getY();
    }

    double getBottom() const {
        return mOrigin.getY() + mOffset.getY();
    }

    const Point& getOffset() const {
        return mOffset;
    }


    void setOffset(const Point& offset) {
        mOffset = offset;
//        mGrid.setIndexOffset((int) std::round(offset.getX() / mCellSize),
//                             (int) std::round(offset.getY() / mCellSize));
    }

    Point normalize(const Point& point) const {
        if (this->isContainsPoint(point)) return point;
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
        mGrid.fill(mDefaultValue);
    }

    void fill(const T& value) {
        mGrid.fill(value);
    }

    T getDefaultValue() const {
        return mDefaultValue;
    }

    void setDefaultValue(const T& defaultValue) {
        mDefaultValue = defaultValue;
    }


    const Grid<T, W, H>& getGrid() const {
        return mGrid;
    }

    const Point& getOrigin() const {
        return mOrigin;
    }


    void setOrigin(const Point& origin) {
        mOrigin = origin;
    }


    double getCellSize() const {
        return mCellSize;
    }


    void setCellSize(double cellSize) {
        mCellSize = cellSize;
    }


    template <class G>
    void setGrid(G&& grid) {
        mGrid = std::forward<G>(grid);
    }


    friend std::ostream& operator<<(std::ostream& os, const GridMap& map) {
        auto sector = map.getGrid();
        auto origin = map.getOrigin();
        auto offset = map.getOffset();
        auto cellSize = map.getCellSize();
        for (int yi = 0; yi < H; ++yi) {
            for (int xi = 0; xi < W; ++xi) {
                auto point = Point{xi, yi};
                os << point * cellSize + origin + offset;
                if (xi != W - 1) os << ' ';
            }
            os << std::endl;
        }
        return os;
    }


private:
    inline double normalize(double value, double lower, double upper) const {
        if (value < lower) {
            value = lower;
        } else if (value > upper) {
            value = upper;
        }
        return value;
    }

private:
    Point mOrigin;
    Point mOffset;
    double mCellSize;
    T mDefaultValue;
    Grid<T, W, H> mGrid;
};