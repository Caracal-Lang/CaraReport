#include <CaraReport/Source.h>

namespace CaraReport
{
    Source::Source(const std::string& name, const std::string& source)
        : m_name(name)
        , m_source(source)
    {
        buildLineIndex();
    }

    void Source::setName(const std::string& name)
    {
        m_name = name;
    }

    void Source::setSource(const std::string& source)
    {
        m_source = source;
        buildLineIndex();
    }

    const std::string& Source::name() const
    {
        return m_name;
    }

    const std::string& Source::source() const
    {
        return m_source;
    }

    void Source::buildLineIndex()
    {
        m_lineOffsets.clear();
        m_lineOffsets.push_back(0);

        for (auto i = 0; i < m_source.size(); ++i)
        {
            if (m_source[i] == '\n')
            {
                m_lineOffsets.push_back(i + 1);
            }
        }
    }

    int Source::lineAtOffset(int offset) const
    {
        if (m_lineOffsets.empty())
        {
            return 0;
        }

        int lastOffset = 0;
        for (auto i = 0; i < m_lineOffsets.size(); ++i)
        {
            if (m_lineOffsets[i] > offset)
            {
                break;
            }
            lastOffset = i;
        }

        // line numbers are 1-based
        return lastOffset + 1;
    }

    std::optional<SpanContents> Source::readSpan(
        const Span& span,
        int contextLinesBefore,
        int contextLinesAfter) const
    {
        if (span.start() < 0
            || span.length() < 0
            || span.end() > m_source.size())
        {
            return std::nullopt;
        }

        SpanContents contents;
        contents.name = m_name;
        contents.data = m_source;
        contents.span = span;

        int startLine = lineAtOffset(span.start());
        int endLine = lineAtOffset(span.end());
        int fromLine = std::max(1, startLine - contextLinesBefore);
        int toLine = endLine + contextLinesAfter;

        if (toLine > m_lineOffsets.size())
        {
            toLine = m_lineOffsets.size();
        }

        contents.startLine = fromLine;
        contents.startColumn = span.start() - m_lineOffsets[fromLine - 1];

        for (int lineNumber = fromLine; lineNumber <= toLine; ++lineNumber)
        {
            int offset = m_lineOffsets[lineNumber - 1];
            int length = 0;
            if (lineNumber < m_lineOffsets.size())
            {
                length = m_lineOffsets[lineNumber] - offset;
                if (length > 0 && m_source[offset + length - 1] == '\n')
                {
                    --length;
                }
            }
            else
            {
                length = m_source.size() - offset;
            }

            LineInfo info;
            info.lineNumber = lineNumber;
            info.start = offset;
            info.length = length;
            info.text = m_source.substr(offset, length);
            
            contents.lines.push_back(std::move(info));
        }

        return contents;
    }
}
