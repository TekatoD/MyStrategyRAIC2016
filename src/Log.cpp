/**
 * @author Arseniy Ivin <arssivka@yandex.ru>
 * @date 11/26/16
 */

#include "Log.h"


const char* Log::LABELS[4] =  {"DEBUG", "INFO", "WARN", "ERROR"};

TypeLog Log::sGlobalLevel = WARN;


Log::Log(TypeLog level) : mLevel(level), mPrompt(false) {
    if (level >= sGlobalLevel) {
        this->prompt();
    }
}

void Log::prompt() {
    if (!mPrompt) {
        std::cerr << '[' << Log::LABELS[mLevel] << ']';
        mPrompt = true;
    }
}


Log::~Log() {
    if (mPrompt) std::cerr << std::endl;
}


void Log::setGlobalLevel(TypeLog level) noexcept {
    sGlobalLevel = level;
}
