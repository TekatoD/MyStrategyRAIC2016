/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#include "Named.h"

Named::Named(const std::string& name) : mName(name) {}

const std::string& Named::getName() const {
    return mName;
}

std::ostream& operator<<(std::ostream& os, const Named& named) {
    return os << "mName: " << named.mName;
}
