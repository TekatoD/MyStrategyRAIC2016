/**
 *  @autor arssivka
 *  @date 12/7/16
 */

#include "MagicSensors.h"
#include "Log.h"


MagicSensors::MagicSensors(Ptr<WorldFilter> filter, size_t count, double radius)
        : mRadius(radius), mFilter(std::move(filter)), mPositions(count), mSensors(count, false) {
    const double step = PI * 2 / count;
    for (int i = 0; i < count; ++i) {
        mPositions[i] = {radius * std::cos(i * step), radius * std::sin(i * step)};
    }
}


void MagicSensors::update(Ptr<State> state) {
    //////////////////////////////////////////////////////
    //               !!!GOTO BEWARE!!!                  //
    //////////////////////////////////////////////////////
    mFilter->update(state);
    auto& self = state->self;
    size_t size = mPositions.size();
    std::fill(mSensors.begin(), mSensors.end(), false);
    auto& buildings = mFilter->getBuildings();
    auto& minions = mFilter->getMinions();
    auto& wizards = mFilter->getWizards();
    auto& trees = mFilter->getTrees();

    for (int i = 0; i < size; ++i) {
        auto position = Point{self};
        position += mPositions[i];
        for (auto&& building : buildings) {
            if (position.inCircle({*building}, building->getRadius())) {
                mTriggered = true;
                mSensors[i] = true;
                goto end;
            }
        }
        for (auto&& minion : minions) {
            if (position.inCircle({*minion}, minion->getRadius())) {
                mTriggered = true;
                mSensors[i] = true;
                goto end;
            }
        }
        for (auto&& tree : trees) {
            if (position.inCircle({*tree}, tree->getRadius())) {
                mTriggered = true;
                mSensors[i] = true;
                goto end;
            }
        }
        for (auto&& wizard: wizards) {
            if (position.inCircle({*wizard}, wizard->getRadius())) {
                mTriggered = true;
                mSensors[i] = true;
                goto end;
            }
        }
        end:
        continue;
    }

}


double MagicSensors::correctDirection(double direction, double segmentSize) const {
    if (!mTriggered) return direction;
    size_t count = mPositions.size();
    const double step = PI * 2 / count;
    size_t countInSegment = std::abs(std::floor(segmentSize / step));
    size_t center = (size_t) std::round(direction / step) % count;
    size_t min;

    min = 0;
    for (size_t i = 1; i < countInSegment; ++i) {
        size_t index = (center + i) % count;
        if (mSensors[index]) {
            min = i;
            break;
        }
    }
    if (min != 0) direction += (countInSegment  - min) * step;

    double temp = min;
    min = 0;
    for (size_t i = 1; i < countInSegment; ++i) {
        size_t index = (center - i) % count;
        if (mSensors[index]) {
            min = i;
            break;
        }
    }
    if (min != temp && min != 0) direction -= (countInSegment  - min) * step;
    return direction;
}


double MagicSensors::getRadius() const {
    return mRadius;
}


bool MagicSensors::isTriggered() const {
    return mTriggered;
}


std::ostream& operator<<(std::ostream& os, const MagicSensors& sensors) {
    os << "Sensors: ";
    for (auto&& s : sensors.mSensors)
        os << s << ' ';
    return os;
}
