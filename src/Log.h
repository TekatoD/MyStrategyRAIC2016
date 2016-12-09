/**
 * @author Arseniy Ivin <arssivka@yandex.ru>
 * @date 11/25/16
 */

#pragma once


#include <iostream>

enum TypeLog {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERR = 3
};



class Log {
public:
    Log(TypeLog level, const std::string& separator = " ");

    ~Log();


    template<class T>
    Log& operator<<(const T& msg) {
        if (mLevel >= sGlobalLevel) {
            this->prompt();
            if (!mFirst) {
                mFirst = true;
            } else {
                std::cerr << mSeparator;
            }
            std::cerr << msg;
        }
        return *this;
    }


    static void setGlobalLevel(TypeLog level) noexcept;

private:
    void prompt();

    TypeLog mLevel;
    bool mPrompt;
    bool mFirst;
    const std::string mSeparator;

    static TypeLog sGlobalLevel;
    static const char* LABELS[4];
};

