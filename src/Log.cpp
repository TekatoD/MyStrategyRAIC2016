/**
 * @author arssivka
 * @date 11/26/16
 */

#include "Log.h"

static const std::string LABELS[] = {"DEBUG", "INFO", "WARN", "ERROR"};
const std::string* LOG::LABELS = LABELS;


LOG::LOG(TypeLog type) : mLevel(type), mStream((mLevel == WARN || mLevel== ERROR) ? std::cerr : std::cout) {
    mStream << '[' << LABELS[type] << ']';
}


LOG::~LOG() {
    mStream << endl;
}
