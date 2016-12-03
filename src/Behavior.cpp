/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Behavior.h"


void Behavior::turn() {
    mWalkingAction.perform();
    mCastAction.perform();
}
