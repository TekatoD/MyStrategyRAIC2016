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

class Point {
public:
    Point();

    Point(double x, double y);

    Point(model::Unit unit);

    Point(const Point& other) {
        mX = other.mX;
        mY = other.mY;
    }


    double getX() const;

    void setX(double x);

    double getY() const;

    void setY(double y);

    double getDistanceTo(model::Unit unit) const;

    double getDistanceTo(const Point& point) const;

    double getDistanceTo(double x, double y) const;

    bool inCircle(const Point& point, double radius);

    template <class T>
    Point operator+(const T& other) {
        auto point = Point(other);
        point += *this;
        return point;
    }

    template <class T>
    Point& operator+=(const T& other) {
        mX += other.getX();
        mY += other.getY();
        return *this;
    }

    template <class T>
    Point& operator=(const T& other) {
        mX = other.getX();
        mY = other.getY();
        return *this;
    }

    bool operator==(const Point &rhs) const;

    bool operator!=(const Point &rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    void move(double x, double y);

    Point operator+(const Point& other) const;

    const Point& operator+=(const Point& other);

    Point operator-(const Point& other) const;

    const Point& operator-=(const Point& other);

    Point operator*(double value) const;

    const Point& operator*=(double other);

    Point operator/(double value) const;

    const Point& operator/=(double value);

private:
    double mX;
    double mY;
};

namespace std {
    template <>
    struct hash<Point> {
        using argument_type = Point;
        using result_type = size_t;
        size_t operator()(const Point& point) const {
            std::hash<double> h;
            return h(point.getX()) ^ h(point.getY());
        }
    };
}


