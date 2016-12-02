/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <model/Unit.h>
#include <cmath>
#include <functional>
#include <ostream>
#include "Pointer.h"

class Size {
public:
    Size();

    Size(size_t x, size_t y);

    Size(const Size& other) {
        mWidth = other.mWidth;
        mHeight = other.mHeight;
    }


    virtual size_t getWidth() const;

    virtual void setWidth(size_t x);

    virtual size_t getHeight() const;

    virtual void setHeight(size_t y);

    virtual bool isContains(size_t x, size_t y);

    template <class T>
    Size operator+(const T& other) {
        auto Size = Size(other);
        Size += *this;
        return Size;
    }

    template <class T>
    Size& operator+=(const T& other) {
        mWidth += other.getX();
        mHeight += other.getY();
        return *this;
    }

    template <class T>
    Size& operator=(const T& other) {
        mWidth = other.getX();
        mHeight = other.getY();
        return *this;
    }

    bool operator==(const Size &rhs) const;

    bool operator!=(const Size &rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Size& Size);

    void move(size_t x, size_t y);

    Size operator+(const Size& other) const;

    const Size& operator+=(const Size& other);

    Size operator-(const Size& other) const;

    const Size& operator-=(const Size& other);

    Size operator*(size_t value) const;

    const Size& operator*=(size_t other);

    Size operator/(size_t value) const;

    const Size& operator/=(size_t value);

private:
    size_t mWidth;
    size_t mHeight;
};

namespace std {
    template <>
    struct hash<Size> {
        using argument_type = Size;
        using result_type = size_t;
        size_t operator()(const Size& Size) const {
            std::hash<size_t> h;
            return h(Size.getWidth()) ^ h(Size.getHeight());
        }
    };
}


