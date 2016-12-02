/**
 * @author arssivka
 * @date 12/1/16
 */

#pragma once


#include <vector>
#include <utility>
#include <ostream>
#include "Point.h"

template <class T, size_t W, size_t H>
class Grid {
public:
    Grid(std::pair<int, int> indexOffset={0, 0})
            : mIndexOffset(indexOffset), mData() { mData.resize(W * H); }

    Grid(const T& defaultValue, std::pair<int, int> indexOffset={0, 0})
            : mIndexOffset(indexOffset), mData(W * H, defaultValue) {}

    Grid(const Grid<T, W, H>& other) = default;

    Grid(Grid<T, W, H>&& other) = default;

    Grid<T, W, H>& operator=(const Grid<T, W, H>& other) = default;

    Grid<T, W, H>& operator=(Grid<T, W, H>&& other) = default;

    T getValue(int x, int y) const { return mData[this->index(x, y)]; }

    T setValue(int x, int y, const T& value) { return mData[this->index(x, y)] = value; }

    bool isValidIndex(int x, int y) const {
        return    x >= mIndexOffset.first  && x < W + mIndexOffset.first
               && y >= mIndexOffset.second && y < H + mIndexOffset.second;
    }

    const std::pair<int, int>& getIndexOffset() const { return mIndexOffset; }

    void setIndexOffset(int x, int y) { mIndexOffset = {x, y}; }

    void fill(const T& value) {
        std::fill(mData.begin(), mData.end(), value);
    }

    friend std::ostream& operator<<(std::ostream& os, const Grid& sector) {
        const auto& offset = sector.getIndexOffset();
        for (int yi = 0; yi < H; ++yi) {
            for (int xi = 0; xi < W; ++xi) {
                os << sector.getValue(xi + offset.first, yi + offset.second);
                if (xi != W - 1) os << ' ';
            }
            os << std::endl;
        }
        return os;
    }


private:
    size_t index(int x, int y) const {
        return W * (y - mIndexOffset.first) + x - mIndexOffset.first;
    }

private:
    std::pair<int, int> mIndexOffset;
    std::vector<T> mData;
};


