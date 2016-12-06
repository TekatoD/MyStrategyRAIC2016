/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include "Point.h"
#include "Situation.h"

class DamagedBuildingSituation : public Situation {
public:
    DamagedBuildingSituation(const std::string& name, const Point& position, int threshold);

    void update(Ptr<State> state) override;

    const Point& getPosition() const;

    void setPosition(const Point& position);

    int getThreshold() const;

    void setThreshold(int threshold);

private:
    Point mPosition;
    int mThreshold;
};


