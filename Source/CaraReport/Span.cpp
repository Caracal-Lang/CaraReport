#include <CaraReport/Span.h>

namespace CaraReport
{
    Span::Span() 
        : m_start(0)
        , m_length(0) 
    {
    }

    Span::Span(int start, int length) 
        : m_start(start)
        , m_length(length) 
    {
    }

    Span Span::fromRange(int start, int end)
    {
        return {start, end - start};
    }

    int Span::start() const
    {
        return m_start;
    }

    int Span::end() const
    {
        return m_start + m_length;
    }

    int Span::length() const
    {
        return m_length;
    }

    bool Span::isEmpty() const
    {
        return m_length == 0;
    }

    bool Span::contains(int position) const
    {
        return (position >= m_start) && (position < end());
    }

    bool Span::overlaps(const Span& other) const
    {
        return (m_start < other.end()) && (other.m_start < end());
    }
}
