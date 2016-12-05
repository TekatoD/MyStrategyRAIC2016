/**
 * @author arssivka
 * @date 12/4/16
 */

#include "CallbackSituation.h"


CallbackSituation::CallbackSituation(const std::string& name,
                                     const std::function<double(Ptr<State>)>& probabilityGenerator)
        : Situation(name), mProbabilityGenerator(probabilityGenerator) {}


void CallbackSituation::update(Ptr<State> state) {
    this->setProbability(mProbabilityGenerator(state));
}
