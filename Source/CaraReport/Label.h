#pragma once

#include <CaraReport/API.h>
#include <CaraReport/Span.h>
#include <string>

namespace CaraReport
{
    class CARAREPORT_API Label
    {
    public:
        Label(Span span, const std::string& label, bool primary = false);
        Label(int start, int length, const std::string& label, bool primary = false);

        [[nodiscard]] const Span& span() const;
        [[nodiscard]] const std::string& text() const;
        [[nodiscard]] bool isPrimary() const;

        [[nodiscard]] int start() const;
        [[nodiscard]] int end() const;
        [[nodiscard]] int length() const;

    private:
        Span m_span;
        std::string m_text;
        bool m_primary = false;
    };
}
