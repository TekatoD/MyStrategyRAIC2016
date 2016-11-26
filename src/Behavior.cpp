/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Behavior.h"


void Behavior::turn() {
    mMessagingAction.perform();
    mLearningAction.perform();
    mWalkingAction.perform();
    mCastAction.perform();
}
