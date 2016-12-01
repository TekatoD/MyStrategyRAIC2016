/**
 * @author arssivka
 * @date 12/1/16
 */

#pragma once


#include <vector>
#include "Point.h"

template <class T, size_t W, size_t H>
class Sector : public Pointer<Sector<T, W, H>> {
public:
    Sector() : mData() { mData.resize(W * H); }

    Sector(const T& defaultValue) : mData(W * H, defaultValue) {}

    Sector(const Sector& other) = default;

    Sector(Sector&& other) = default;

    const Sector& operator=(const Sector& other) = default;

    const Sector& operator=(Sector&& other) = default;

    T getValue(int x, int y) const { return mData[W * y + x]; }

    T setValue(int x, int y, const T& value) const { return mData[W * y + x] = value; }

    bool isValid(int x, int y) const { return x >= 0 && x < W && y >= 0 && y < H; }

private:
    std::vector<T> mData;
};


