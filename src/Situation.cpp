/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Situation.h"

std::ostream& operator<<(std::ostream& os, const Situation& situation) {
    os << "Situation " << situation.getName();
    return os;
}

Situation::Situation(const std::string& name) : Named(name) {}
