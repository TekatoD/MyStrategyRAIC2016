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

double Point::getDistanceTo(const Point& point) {
    return this->getDistanceTo(point.getX(), point.getY());
}

double Point::getDistanceTo(double x, double y) {
    double dx = x - mX;
    double dy = y - mX;
    return std::sqrt((float) (dx * dx + dy * dy));
}

void Point::move(double x, double y) {
    mX += x;
    mY += y;
}
