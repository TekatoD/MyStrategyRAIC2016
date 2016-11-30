/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <model/Unit.h>
#include <cmath>
#include <functional>

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

    double getDistanceTo(const Point& point);

    double getDistanceTo(double x, double y);

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

    void move(double x, double y);

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


