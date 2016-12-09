/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Size.h"

Size::Size() : mWidth(0), mHeight(0) {}

Size::Size(size_t x, size_t y) : mWidth(x), mHeight(y) {}

size_t Size::getWidth() const {
    return mWidth;
}

void Size::setWidth(size_t x) {
    mWidth = x;
}

size_t Size::getHeight() const {
    return mHeight;
}

void Size::setHeight(size_t y) {
    mHeight = y;
}


void Size::move(size_t x, size_t y) {
    mWidth += x;
    mHeight += y;
}

bool Size::operator==(const Size &rhs) const {
    return mWidth == rhs.mWidth &&
           mHeight == rhs.mHeight;
}

bool Size::operator!=(const Size &rhs) const {
    return !(rhs == *this);
}

std::ostream& operator<<(std::ostream& os, const Size& Size) {
    os << "(" << Size.mWidth << "," << Size.mHeight << ")";
    return os;
}


Size Size::operator+(const Size& other) const {
    return Size(*this) += other;
}


const Size& Size::operator+=(const Size& other) {
    mWidth += other.mWidth;
    mHeight += other.mHeight;
    return *this;
}


Size Size::operator-(const Size& other) const {
    return Size(*this) -= other;
}


const Size& Size::operator-=(const Size& other) {
    mWidth -= other.mWidth;
    mHeight -= other.mHeight;
    return *this;
}


Size Size::operator*(size_t value) const {
    return Size(*this) * value;
}


const Size& Size::operator*=(size_t other) {
    mWidth *= other;
    mHeight *= other;
    return *this;
}


Size Size::operator/(size_t value) const {
    return Size(*this) /= value;
}


const Size& Size::operator/=(size_t value) {
    mWidth /= value;
    mHeight /= value;
    return *this;
}


bool Size::isContains(size_t x, size_t y) {
    return x >= 0 && x < mWidth && y >= 0 && y <= mHeight;
}

Size::Size(const Size& other) {
    mWidth = other.mWidth;
    mHeight = other.mHeight;
}
