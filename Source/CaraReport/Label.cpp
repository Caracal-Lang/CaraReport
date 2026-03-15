#include <CaraReport/Label.h>

namespace CaraReport
{
    Label::Label(Span span, const std::string& label, bool primary)
        : m_span(span)
        , m_text(label)
        , m_primary(primary)
    {
    }

    Label::Label(int start, int length, const std::string& label, bool primary)
        : m_span(start, length)
        , m_text(label)
        , m_primary(primary)
    {
    }

	const Span& Label::span() const
    {
        return m_span;
    }

    std::optional<std::string> Label::text() const
    {
        return m_text;
    }

    bool Label::isPrimary() const
    {
        return m_primary;
    }

	int Label::start() const
    {
        return m_span.start();
    }

	int Label::end() const 
    {
        return m_span.end(); 
    }

	int Label::length() const 
    {
        return m_span.length(); 
    }
}
