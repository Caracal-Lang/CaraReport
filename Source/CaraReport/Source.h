#pragma once

#include <CaraReport/Span.h>
#include <algorithm>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace CaraReport
{
    struct LineInfo
    {
        int lineNumber;
        int start;
        int length;
        std::string text;
    };

    struct SpanContents
    {
        std::string name;
        std::string data;
        Span span;
        int startLine = 0;
        int startColumn = 0;
        int lineCount = 0;
        std::vector<LineInfo> lines;
    };

    class Source
    {
    public:
        Source(const std::string& name, const std::string& source);

        void setName(const std::string& name);
        void setSource(const std::string& source);

        [[nodiscard]] const std::string& name() const;
        [[nodiscard]] const std::string& source() const;
        [[nodiscard]] std::optional<SpanContents> readSpan(
            const Span& span, 
            int contextLinesBefore,
            int contextLinesAfter) const;

    private:
        std::string m_name;
        std::string m_source;
        std::vector<int> m_lineOffsets;

        void buildLineIndex();
        [[nodiscard]] int lineAtOffset(int offset) const;
    };
}
