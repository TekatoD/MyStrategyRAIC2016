/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#pragma once


#include <string>
#include <ostream>

class Named {
public:
    Named(const std::string& name);

    const std::string& getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Named& named);

private:
    std::string mName;
};