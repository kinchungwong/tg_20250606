#pragma once
#include <iostream>
#include "tg/common/project_macros.hpp"

struct OStrm
{
    std::ostream* m_pstrm;
    
    INLINE_NEVER
    FLATTEN
    explicit OStrm(std::ostream& ostrm)
        : m_pstrm(&ostrm)
    {}

    INLINE_NEVER
    FLATTEN
    explicit OStrm()
        : m_pstrm(&std::cout)
    {}

    template <typename T>
    INLINE_NEVER
    FLATTEN
    OStrm& operator<<(const T& value)
    {
        (*m_pstrm) << value;
        return *this;
    }

    INLINE_NEVER
    FLATTEN
    OStrm& operator<<(std::ostream& (*f)(std::ostream&))
    {
        f(*m_pstrm);
        return *this;
    }
};
