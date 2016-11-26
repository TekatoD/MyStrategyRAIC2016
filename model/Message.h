#pragma once

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <vector>

#include "LineType.h"
#include "SkillType.h"

namespace model {
    class Message {
    private:
        LineType line;
        SkillType skillToLearn;
        std::vector<signed char> rawMessage;
    public:
        Message();
        Message(LineType line, SkillType skillToLearn, const std::vector<signed char>& rawMessage);

        LineType getLine() const;
        SkillType getSkillToLearn() const;
        const std::vector<signed char>& getRawMessage() const;
    };
}

#endif
