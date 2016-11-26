/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <memory>

template <class T>
class Pointer {
public:
    using Ptr = typename std::shared_ptr<T>;
};

template <class T, class... Args>
typename T::Ptr share(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
};