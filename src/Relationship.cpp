/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Relationship.h"

Relationship::Relationship(const std::string& name, Ptr<Situation> situation,
                           Ptr<Behavior> behavior)
        : Named(name), mSituation(std::move(situation)), mBehavior(std::move(behavior)) {}


Ptr<Situation> Relationship::getSituation() const {
    return mSituation;
}


Ptr<Behavior> Relationship::getBehavior() const {
    return mBehavior;
}

double Relationship::getFactor() const {
    if (mSituation->isDisabled() || mBehavior->isDisabled()) return  0.0;
    return   mSituation->getProbability() * mSituation->getFactor()
           * mBehavior->getProbability() * mBehavior->getFactor();
}
