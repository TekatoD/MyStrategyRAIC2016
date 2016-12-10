/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#pragma once


#include <unordered_map>
#include <list>
#include <set>
#include <random>
#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"
#include "Mechanism.h"
#include "Teacher.h"

class BigTeacher : public Teacher {
public:
    virtual void feedBehavior(Ptr<Behavior> behavior) override;

    virtual void feedSituation(Ptr<Situation> situation) override;

    virtual void feedRelationship(Ptr<Relationship> relationship) override;

    void update(Ptr<State> state) override;

private:

    struct Being {
        Being() = default;
        Being(Being&& other) = default;
        void updateScore(double score, int ticks, Ptr<State> state);
        Being makeChild(const Being& partner);
        Being& operator=(const Being& other) = default;
        Being&operator=(Being&& other) = default;
        std::unordered_map<std::string, float> mFactors;
        double mScore;
        bool mChanged;
    };

    struct Incubator {
        Incubator(size_t generationSize);
        void init();
        void update(double score, int ticks, Ptr<State> state, const std::set<Ptr<Behavior>>& behaviorSet);
        void setNeededFactors(const std::set<Ptr<Behavior>>& behaviorSet);
        std::vector<Being> mGeneration;
    };
    double mLastScore;
    std::string mLastSituation;
    int mLastTick;
    bool mSituationUpdated;
    std::unordered_map<std::string, Incubator> mIncubators;
    std::set<Ptr<Behavior>> mBehaviour;
};


