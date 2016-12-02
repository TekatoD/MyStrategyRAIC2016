/**
 * @author arssivka
 * @date 12/1/16
 */

#pragma once


#include <cmath>
#include "Sector.h"
#include "Graph.h"
#include "SectorMap.h"

template <size_t W, size_t H>
class GridGraph : public SectorMap<int, W, H>, public virtual Graph {
public:
    GridGraph(const Point& origin, double cellSize, const Point& offset = {0.0, 0.0}, int obstacle = 0)
            : SectorMap<int, W, H>(origin, cellSize, offset, obstacle) {
        SectorMap<int, W, H>::mSector.fill(1);
    }

    std::vector<std::pair<Point, double>> getNeighbours(const Point& point) const override {
        if (!this->isContains(point)) return std::vector<std::pair<Point, double>>();

        Point local = this->getLocalPoint(point);
        Point scaled = local / SectorMap<int, W, H>::mCellSize;
        int x = (int) scaled.getX();
        int y = (int) scaled.getY();

        std::vector<std::pair<Point, double>> neighbours;
        neighbours.reserve(8);
        double z = std::sqrt(SectorMap<int, W, H>::mCellSize * SectorMap<int, W, H>::mCellSize * 2);
        for (int xoffset = -1; xoffset <= 1; ++xoffset) {
            for (int yoffset = -1; yoffset <= 1; ++yoffset) {
                if (xoffset == 0 && yoffset == 0) continue;
                int cx = x + xoffset;
                int cy = x + yoffset;
                auto value = SectorMap<int, W, H>::mSector.getValue(x, y);
                if (SectorMap<int, W, H>::mSector.isValid(cx, cy) and value != SectorMap<int, W, H>::mDefaultValue) {
                    double path = (xoffset == 0 || yoffset == 0) ? value : z * value;
                    neighbours.push_back({SectorMap<int, W, H>::mOrigin + SectorMap<int, W, H>::mOffset + Point(cx, cy) * SectorMap<int, W, H>::mCellSize, path});
                }
            }
        }
        return neighbours;
    }

    Point getNearestVertex(const Point& point) const override {
        Point scaled(point);
        scaled /= SectorMap<int, W, H>::mCellSize;
        return this->getGlobalPosition((int) scaled.getX(), (int) scaled.getY());
    }
};


