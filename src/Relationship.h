/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "NonCopyable.h"
#include "Situation.h"
#include "Behavior.h"

class Relationship: public Pointer<Relationship>, NonCopyable {
public:
    Relationship(Situation::Ptr situation,
                 Behavior::Ptr behavior,
                 std::function<float(Situation::Ptr, Behavior::Ptr)> evaluator);

    float getFactor() const {
        return mEvaluator(mSituation, mBehavior);
    }

    Situation::Ptr getSituation() const;

    Behavior::Ptr getBehavior() const;

private:
    Situation::Ptr mSituation;
    Behavior::Ptr mBehavior;
    std::function<float(std::shared_ptr<Situation>, std::shared_ptr<Behavior>)> mEvaluator;
};


