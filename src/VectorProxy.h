/**
 * @author arssivka
 * @date 12/3/16
 */

#pragma once


#include <vector>
#include <cstddef>
#include <iterator>
#include <algorithm>

template <class T>
class VectorProxy {
public:
    VectorProxy() : mContainer(nullptr) {}

    VectorProxy(const std::vector<T>& container)
            : mContainer(&container) {}

    VectorProxy(const VectorProxy& other) : mIndexes(mIndexes), mContainer(other.mContainer) {}

    VectorProxy(VectorProxy&& other) : mIndexes(std::move(mIndexes)), mContainer(other.mContainer) {}

    void addIndex(size_t index) {
        mIndexes.push_back(index);
    }

    void removeIndex(size_t index) {
        mIndexes.erase(std::remove(mIndexes.begin(), mIndexes.end(), index), mIndexes.end());
    }

    const T& operator[](size_t index) const {
        return mContainer[index];
    }


    size_t getSize() const { return mIndexes.size(); }

    bool isEmpty() const { return mIndexes.empty(); }

    void clear() { mIndexes.clear(); }

    void setContainer(const std::vector<T>& container) {
        mContainer = &container;
        mIndexes.clear();
    }


private:
    std::vector<size_t> mIndexes;
    const std::vector<T>* mContainer;
};


