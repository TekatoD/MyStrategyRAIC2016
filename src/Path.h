/**
 * @author arssivka
 * @date 12/3/16
 */

#pragma once


#include <forward_list>
#include <ostream>
#include "Point.h"

class Path {
public:
    Path() = default;

    Path(std::initializer_list<Point> initializerList);

    void push(const Point& point);

    Point pop();

    Point current();

    bool isFinished() const;

    size_t countVertex() const;

    friend std::ostream& operator<<(std::ostream& os, const Path& path);

private:
    std::forward_list<Point> mPoints;
    size_t mSize;
};


