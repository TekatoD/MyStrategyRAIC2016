/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "Situation.h"

class CallbackSituation : public Situation {
public:
    CallbackSituation(const std::string& name, const std::function<double(Ptr<State>)>& probabilityGenerator);

    void update(Ptr<State> state) override;

private:
    std::function<double(Ptr<State>)> mProbabilityGenerator;
};


