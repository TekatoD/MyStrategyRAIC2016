/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


class Ranked {
public:
    virtual void setProbability(float value) = 0;

    virtual float getProbability() const = 0;

    virtual void setFactor(float value) = 0;

    virtual float getFactor() const = 0;

    virtual ~Ranked() {}
};