/**
 * @author arssivka
 * @date 12/5/16
 */

#include "SelfDisabler.h"


SelfDisabler::SelfDisabler()
        : mDisabled(false) {}


bool SelfDisabler::isDisabled() const {
    return mDisabled;
}


void SelfDisabler::disable() {
    mDisabled = true;
}
