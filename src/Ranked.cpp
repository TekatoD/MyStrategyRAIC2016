/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#include "Ranked.h"

void Ranked::setProbability(double value) {
    if (value < 0.0) {
        value = 0.0;
    } else if (value >= 1.0) {
        value = 1.0;
    }
    mProbability = value;
}

double Ranked::getProbability() const {

    return (!mInverseProbability) ? mProbability : 1.0 - mProbability;
}

void Ranked::setFactor(double value) {
    mFactor = value;
}

double Ranked::getFactor() const {
    return mFactor;
}

bool Ranked::isInverseProbability() const {
    return mInverseProbability;
}

void Ranked::setInverseProbability(bool inverseProbability) {
    mInverseProbability = inverseProbability;
}

Ranked::Ranked()
        : mProbability(0.0), mFactor(1.0), mInverseProbability(false) {}

Ranked::Ranked(double probability, double factor, bool inverseProbability)
        : mProbability(probability), mFactor(factor), mInverseProbability(inverseProbability) {}
