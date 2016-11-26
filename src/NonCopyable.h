/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


class NonCopyable {
public:
    NonCopyable() = default;

private:
    NonCopyable(const NonCopyable& other) = delete;

    NonCopyable& operator=(const NonCopyable& other) = delete;

};
