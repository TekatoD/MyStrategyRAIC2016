/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <memory>

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T, class... Args>
Ptr<T> share(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
};