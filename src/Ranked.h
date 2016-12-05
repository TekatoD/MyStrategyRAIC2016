/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


class Ranked {
public:
    void setProbability(double value) {
        if (value < 0.0) {
            value = 0.0;
        } else if (value >= 1.0) {
            value = 1.0;
        }
        mProbability = value;
    }

    double getProbability() const {

        return (!mInverseProbability) ? mProbability : 1.0 - mProbability;
    }

    void setFactor(float value) {
        mFactor = value;
    }

    double getFactor() const {
        return mFactor;
    }

    bool isInverseProbability() const {
        return mInverseProbability;
    }

    void setInverseProbability(bool inverseProbability) {
        mInverseProbability = inverseProbability;
    }


protected:
    Ranked()
            : mProbability(0.0), mFactor(1.0), mInverseProbability(false) {}

    Ranked(double probability, double factor = 1.0, bool inverseProbability = false)
            : mProbability(probability), mFactor(factor), mInverseProbability(inverseProbability) {}

private:
    double mProbability;
    double mFactor;
    bool mInverseProbability;
};