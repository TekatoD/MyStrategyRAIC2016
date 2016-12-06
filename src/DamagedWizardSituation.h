/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include "Situation.h"

class DamagedWizardSituation : public Situation {
public:
    DamagedWizardSituation(const std::string& name, long long id, double threshold, double heals, double maxHeals);

    void update(Ptr<State> state) override;

    void setThreshold(double threshold);

    void setId(long long id);

    double getThreshold() const;

    long long getId() const;

    double getMaxHeals() const;

    void setMaxHeals(double maxHP);

    double getHeals() const;

    void setHeals(double HP);

private:
    double mThreshold;
    double mHP;
    double mMaxHP;
    int mLevel;
    long long mId;
};


