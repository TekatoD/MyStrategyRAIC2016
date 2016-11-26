/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Relationship.h"

Relationship::Relationship(std::shared_ptr<Situation> situation,
                           std::shared_ptr<Behavior> behavior,
                           std::function<float(std::shared_ptr<Situation>, std::shared_ptr<Behavior>)> evaluator)
        : mSituation(std::move(situation)), mBehavior(std::move(behavior)), mEvaluator(evaluator) {}


Situation::Ptr Relationship::getSituation() const {
    return mSituation;
}


Behavior::Ptr Relationship::getBehavior() const {
    return mBehavior;
}
