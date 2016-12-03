/**
 * @author arssivka
 * @date 12/3/16
 */

#pragma once


#include "model/Wizard.h"
#include "model/Projectile.h"
#include "model/Bonus.h"
#include "model/Building.h"
#include "model/Minion.h"
#include "model/Tree.h"
#include "Mechanism.h"
#include "VectorProxy.h"
#include "Point.h"

class WorldFilter : public Mechanism {
public:
    void update(Ptr<State> state) override;

    WorldFilter(double radius);

    void setRadius(double radius);

    double getRadius() const;

    const VectorProxy<model::Tree>& getTrees() const;

    const VectorProxy<model::Wizard>& getWizards() const;

    const VectorProxy<model::Building>& getBuildings() const;

    const VectorProxy<model::Minion>& getMinions() const;

    const VectorProxy<model::Bonus>& getBonuses() const;

    const VectorProxy<model::Projectile>& getProjectiles() const;

private:
    void updateProxies() const;

    template <class T>
    void updateProxy(const std::vector<T>& container, VectorProxy<T>& proxy) const {
        proxy.setContainer(container);
        size_t size = container.size();
        auto me = Point(mState->self);
        for (int i = 0; i < size; ++i)
            if (me.inCircle(container[i], mRadius + container[i].getRadius()))
                mBonuses.addIndex(i);
    }

private:
    Ptr<State> mState;
    double mRadius;
    mutable bool mUpdated;
    mutable VectorProxy<model::Tree> mTrees;
    mutable VectorProxy<model::Wizard> mWizards;
    mutable VectorProxy<model::Building> mBuildings;
    mutable VectorProxy<model::Minion> mMinions;
    mutable VectorProxy<model::Bonus> mBonuses;
    mutable VectorProxy<model::Projectile> mProjectiles;
};


