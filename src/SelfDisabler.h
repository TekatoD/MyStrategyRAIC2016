/**
 * @author arssivka
 * @date 12/5/16
 */

#pragma once


class SelfDisabler {
public:
    SelfDisabler() noexcept;

    bool isDisabled() const noexcept;


protected:
    void disable() noexcept;

private:
    bool mDisabled;
};


