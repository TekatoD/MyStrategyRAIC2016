/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Point.h"

Point::Point() : mX(0), mY(0) {}

Point::Point(double x, double y) : mX(x), mY(y) {}

Point::Point(model::Unit unit) : mX(unit.getX()), mY(unit.getY()) {}

double Point::getX() const {
    return mX;
}

void Point::setX(double x) {
    mX = x;
}

double Point::getY() const {
    return mY;
}

void Point::setY(double y) {
    mY = y;
}

double Point::getDistanceTo(model::Unit unit) const {
    return unit.getDistanceTo(mX, mY);
}

double Point::getDistanceTo(const Point& point) const {
    return this->getDistanceTo(point.getX(), point.getY());
}

double Point::getDistanceTo(double x, double y) const {
    double dx = x - mX;
    double dy = y - mX;
    return std::sqrt((float) (dx * dx + dy * dy));
}

void Point::move(double x, double y) {
    mX += x;
    mY += y;
}

bool Point::operator==(const Point &rhs) const {
    return mX == rhs.mX &&
           mY == rhs.mY;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}

bool Point::inCircle(const Point& point, double radius) {
    double dx = mX - point.getX();
    double dy = mY - point.getY();
    return dx * dx + dy * dy <= radius * radius;
}


std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.mX << "," << point.mY << ")";
    return os;
}


Point Point::operator+(const Point& other) const {
    return Point(*this) += other;
}


const Point& Point::operator+=(const Point& other) {
    mX += other.mX;
    mY += other.mY;
    return *this;
}


Point Point::operator-(const Point& other) const {
    return Point(*this) -= other;
}


const Point& Point::operator-=(const Point& other) {
    mX -= other.mX;
    mY -= other.mY;
    return *this;
}


Point Point::operator*(double value) const {
    return Point(*this) * value;
}


const Point& Point::operator*=(double other) {
    mX *= other;
    mY *= other;
    return *this;
}


Point Point::operator/(double value) const {
    return Point(*this) /= value;
}


const Point& Point::operator/=(double value) {
    mX /= value;
    mY /= value;
    return *this;
}
