/**
 * @author arssivka
 * @date 12/3/16
 */

#include "Path.h"


Path::Path(std::initializer_list<Point> initializerList)
        : mPoints(initializerList) {}


void Path::push(const Point& point) {
    mPoints.emplace_front(point);
}


Point Path::pop() {
    Point point = {mPoints.front()};
    mPoints.pop_front();
    return point;
}


bool Path::isFinished() const {
    return mPoints.empty();
}


std::ostream& operator<<(std::ostream& os, const Path& path) {
    bool first = true;
    for (auto&& point : path.mPoints) {
        if (first) {
            first = false;
        } else {
            os << " -> ";
        }
        os << point;
    }
    return os;
}


Point Path::current() {
    return mPoints.front();
}
