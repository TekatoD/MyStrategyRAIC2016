/**
 * @author arssivka
 * @date 12/1/16
 */

#pragma once


#include <cmath>
#include "Grid.h"
#include "Graph.h"
#include "GridMap.h"

template <size_t W, size_t H>
class GridMapGraph : public GridMap<int, W, H>, public virtual Graph {
public:
    GridMapGraph(const Point& origin, double cellSize, const Point& offset = {0.0, 0.0})
            : GridMap<int, W, H>(origin, cellSize, offset, 0) {
        this->clear();
    }

    std::vector<std::pair<Point, double>> getNeighbourVertexes(const Point& point) const override {
        if (!this->isContainsPoint(point)) return std::vector<std::pair<Point, double>>();

        Point local = this->getLocalPoint(point);
        auto cellSize = this->getCellSize();
        Point scaled = local / cellSize;
        int x = (int) std::round(scaled.getX());
        int y = (int) std::round(scaled.getY());

        std::vector<std::pair<Point, double>> neighbours;
        neighbours.reserve(8);
        double z = std::sqrt(cellSize * cellSize * 2.0);
        for (int xoffset = -1; xoffset <= 1; ++xoffset) {
            for (int yoffset = -1; yoffset <= 1; ++yoffset) {
                if (xoffset == 0 && yoffset == 0) continue;
                int cx = x + xoffset;
                int cy = y + yoffset;
                if (!this->isValidIndex(cx, cy)) continue;
                auto value = this->getCellValueByIndex(cx, cy);
                if (value > this->getDefaultValue()) {
                    double path = (cx == cy) ? z * value : value;
                    neighbours.push_back({this->getGlobalPosition(cx, cy), path});
                }
            }
        }
        return neighbours;
    }

    Point getNearestVertex(const Point& point) const override {
        return this->getNearestPoint(point);
    }

    void clear() {
        this->fill(1);
    }
};
