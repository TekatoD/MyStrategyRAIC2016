/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <iostream>
#include "NonCopyable.h"

using namespace std;

enum TypeLog {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
};


class LOG : NonCopyable {
public:
    LOG(TypeLog type);

    ~LOG();

    template<class T>
    LOG& operator<<(const T& msg) {
        mStream << ' ' << msg;
        return *this;
    }

private:
    TypeLog mLevel;
    std::ostream& mStream;
    static const std::string* LABELS;
};

