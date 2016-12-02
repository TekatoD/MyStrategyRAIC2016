/**
 * @author arssivka
 * @date 12/1/16
 */

#pragma once


#include <vector>
#include <utility>
#include "Point.h"

template <class T, size_t W, size_t H>
class Sector {
public:
    Sector(std::pair<int, int> indexOffset={0, 0})
            : mIndexOffset(indexOffset), mData() { mData.resize(W * H); }

    Sector(const T& defaultValue, std::pair<int, int> indexOffset={0, 0})
            : mIndexOffset(indexOffset), mData(W * H, defaultValue) {}

    Sector(const Sector& other) = default;

    Sector(Sector&& other) = default;

    const Sector& operator=(const Sector& other) = default;

    const Sector& operator=(Sector&& other) = default;

    T getValue(int x, int y) const { return mData[this->index(x, y)]; }

    T setValue(int x, int y, const T& value) const { return mData[this->index(x, y)] = value; }

    bool isValid(int x, int y) const {
        return    x >= mIndexOffset.first  && x < W + mIndexOffset.first
               && y >= mIndexOffset.second && y < H + mIndexOffset.second;
    }

    const std::pair<int, int>& getIndexOffset() const { return mIndexOffset; }

    void setIndexOffset(int x, int y) { mIndexOffset = {x, y}; }

private:
    size_t index(int x, int y) const {
        return W * (y - mIndexOffset.first) + x - mIndexOffset.first;
    }


private:
    std::pair<int, int> mIndexOffset;
    std::vector<T> mData;
};


