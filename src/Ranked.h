/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


class Ranked {
public:
    void setProbability(double value);

    double getProbability() const;

    void setFactor(double value);

    double getFactor() const;

    bool isInverseProbability() const;

    void setInverseProbability(bool inverseProbability);


protected:
    Ranked();

    Ranked(double probability, double factor = 1.0, bool inverseProbability = false);

private:
    double mProbability;
    double mFactor;
    bool mInverseProbability;
};