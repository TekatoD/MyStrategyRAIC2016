#include "Message.h"

using namespace model;
using namespace std;

Message::Message()
    : line(_LINE_UNKNOWN_), skillToLearn(_SKILL_UNKNOWN_), rawMessage(vector<signed char>()) { }

Message::Message(LineType line, SkillType skillToLearn, const vector<signed char>& rawMessage)
    : line(line), skillToLearn(skillToLearn), rawMessage(rawMessage) { }

LineType Message::getLine() const {
    return line;
}

SkillType Message::getSkillToLearn() const {
    return skillToLearn;
}

const vector<signed char>& Message::getRawMessage() const {
    return rawMessage;
}
