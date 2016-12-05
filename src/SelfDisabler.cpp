/**
 * @author arssivka
 * @date 12/5/16
 */

#include "SelfDisabler.h"


SelfDisabler::SelfDisabler() noexcept
        : mDisabled(false) {}


bool SelfDisabler::isDisabled() const noexcept {
    return mDisabled;
}


void SelfDisabler::disable() noexcept {
    mDisabled = true;
}
