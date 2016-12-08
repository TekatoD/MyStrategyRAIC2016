/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "Situation.h"
#include "Point.h"

class BonusExistsSituation : public Situation {
public:
    BonusExistsSituation(const std::string& name, const Point& position);

private:
    void update(Ptr<State> state) override;

public:
    Point mPosition;
    bool mExists;
};


