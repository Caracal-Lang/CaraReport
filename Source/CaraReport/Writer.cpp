#include <CaraReport/Writer.h>
#include <algorithm>
#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace CaraReport
{
    Writer::Writer() 
        : m_theme(std::make_unique<Theme>())
        , m_contextLines(2)
        , m_tabWidth(4)
        , m_colorsEnabled(colorsEnabled())
        , m_ansiSupported(enableUtf8AndAnsi())
    {
    }

    Writer::Writer(const Writer& other)
        : m_theme(std::make_unique<Theme>(*other.m_theme))
        , m_contextLines(other.m_contextLines)
        , m_tabWidth(other.m_tabWidth)
        , m_colorsEnabled(other.m_colorsEnabled)
        , m_ansiSupported(other.m_ansiSupported)
    {
    }

    Writer& Writer::operator=(const Writer& other)
    {
        if (this == &other)
            return *this;

        m_theme = std::make_unique<Theme>(*other.m_theme);
        m_contextLines = other.m_contextLines;
        m_tabWidth = other.m_tabWidth;
        m_colorsEnabled = other.m_colorsEnabled;
        m_ansiSupported = other.m_ansiSupported;

        return *this;
    }

    Writer Writer::create()
    {
        return Writer();
    }

    Writer& Writer::withTheme(std::unique_ptr<Theme>&& theme)
    {
        m_theme = std::move(theme);
        return *this;
    }

    Writer& Writer::withContextLines(int contextLines)
    {
        m_contextLines = contextLines;
        return *this;
    }

    Writer& Writer::withTabWidth(int tabWidth)
    {
        m_tabWidth = tabWidth;
        return *this;
    }

    Writer& Writer::withUnicode(bool enable)
    {
        if (!enable && m_theme->isUnicode())
            m_theme = Theme::ascii();
        return *this;
    }

    Writer& Writer::withColors(bool enable)
    {
        m_colorsEnabled = enable;
        return *this;
    }

    std::string Writer::writeReport(const Report& report) const
    {
        std::ostringstream out;
        writeReport(out, report, false);
        return out.str();
    }

    bool Writer::shouldUseColors() const
    {
        return m_colorsEnabled && m_ansiSupported;
    }

    std::string Writer::levelColor(Level level) const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return m_theme->levelColor(level);
    }

    std::string Writer::titleColor() const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return m_theme->titleColor();
    }

    std::string Writer::frameColor() const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return m_theme->frameColor();
    }

    std::string Writer::fixColor() const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return m_theme->fixColor();
    }

    std::string Writer::bold() const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return Colors::bold();
    }

    std::string Writer::reset() const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        return m_theme->reset();
    }

    std::string Writer::colorFor(const Label* label, const LabelColorMap& colorMap) const
    {
        if (!shouldUseColors())
        {
            return "";
        }

        if (m_theme->labelPalette().empty())
        {
            return "";
        }

        auto index = 0;
        const auto it = colorMap.find(label);
        if (it != colorMap.end())
            index = it->second;

        const auto& palette = m_theme->labelPalette();
        return palette[index % palette.size()];
    }

    LabelColorMap Writer::buildColorMap(const std::vector<Label>& labels, const Label* primaryLabel) const
    {
        // build color map: primary gets 0, rest get 1, 2, 3...
        LabelColorMap colorMap;
        colorMap[primaryLabel] = 0;
        int nextColor = 1;
        for (const auto& label : labels)
        {
            if (&label != primaryLabel && colorMap.find(&label) == colorMap.end())
            {
                colorMap[&label] = nextColor++;
            }
        }

        return colorMap;
    }

    std::vector<std::string> Writer::buildColumnColors(
        const std::vector<Highlight>& highlights, 
        const std::string& expandedText, 
        const LabelColorMap& colorMap) const
    {
        const auto lineDisplayWidth = expandedText.size();
        std::vector<std::string> columnColors(lineDisplayWidth, "");
        for (const auto& highlight : highlights)
        {
            const auto lineColor = colorFor(highlight.label, colorMap);
            const auto start = std::min(highlight.startColumn, static_cast<int>(lineDisplayWidth));
            const auto end = std::min(highlight.endColumn, static_cast<int>(lineDisplayWidth));
            for (auto column = start; column < end; ++column)
            {
                columnColors[column] = lineColor;
            }
        }

        return columnColors;
    }

    int Writer::lineNumberWidth(const std::vector<LineInfo>& lines) const
    {
        if (lines.empty())
        {
            return 1;
        }

        int maxLine = lines.back().lineNumber;
        int width = 0;
        while (maxLine > 0)
        {
            ++width;
            maxLine /= 10;
        }

        return std::max(width, 1);
    }

    std::string Writer::padLeft(const std::string& text, int width) const
    {
        int contentSize = text.size();
        int padding = width - contentSize;
        if (padding <= 0)
        {
            return text;
        }

        return std::string(padding, ' ') + text;
    }

    std::string Writer::expandTabs(const std::string& text) const
    {
        std::string result;
        for (char c : text)
        {
            if (c == '\t')
            {
                int spaces = m_tabWidth - (result.size() % m_tabWidth);
                result.append(spaces, ' ');
            }
            else
            {
                result += c;
            }
        }

        return result;
    }

    int Writer::offsetToDisplayColumn(
        const std::string& text,
        int offset) const
    {
        int column = 0;
        for (int i = 0; i < offset && i < text.size(); ++i)
        {
            if (text[i] == '\t')
            {
                column += m_tabWidth - (column % m_tabWidth);
            }
            else
            {
                ++column;
            }
        }

        return column;
    }

    int Writer::calculateFrameWidth(const std::vector<ContextBlock>& blocks) const
    {
        int frameWidth = 1;
        for (const auto& block : blocks)
        {
            frameWidth = std::max(frameWidth, lineNumberWidth(block.contents.lines));
        }

        return frameWidth;
    }

    int Writer::calculateMaxColumn(const std::vector<Highlight>& highlights) const
    {
        int maxColumn = 0;
        for (const auto& highlight : highlights)
        {
            maxColumn = std::max(maxColumn, highlight.endColumn);
        }

        return maxColumn;
    }

    const Label* Writer::findPrimaryLabel(const std::vector<Label>& labels) const
    {
        if(labels.empty())
        {
            return nullptr;
        }

        for (const auto& label : labels)
        {
            if (label.isPrimary())
            {
                return &label;
            }
        }

        return &labels[0];
    }

    std::vector<ContextBlock> Writer::buildBlocksForLabels(
        const Source* source, 
        const std::vector<Label>& labels) const
    {
        // merge overlapping contexts into one block
        std::vector<ContextBlock> blocks;
        for (const auto& label : labels)
        {
            const auto optionalContent = source->readSpan(label.span(), m_contextLines, m_contextLines);
            if (!optionalContent.has_value())
            {
                continue;
            }

            const auto& content = optionalContent.value();
            if (!blocks.empty())
            {
                auto& lastBlock = blocks.back();
                auto& lastBlockLines = lastBlock.contents.lines;
                auto& newBlockLines = content.lines;

                // check for overlap and adjacency
                if (!lastBlockLines.empty()
                    && !newBlockLines.empty()
                    && newBlockLines.front().lineNumber <= lastBlockLines.back().lineNumber + 1)
                {
                    for (const auto& newBlockLine : newBlockLines)
                    {
                        bool found = false;
                        for (const auto& lastBlockLine : lastBlockLines)
                        {
                            if (lastBlockLine.lineNumber == newBlockLine.lineNumber)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            lastBlockLines.push_back(newBlockLine);
                        }
                    }

                    std::sort(lastBlockLines.begin(), lastBlockLines.end(),
                        [](const LineInfo& lhs, const LineInfo& rhs)
                        {
                            return lhs.lineNumber < rhs.lineNumber;
                        });

                    lastBlock.blockLabels.push_back(&label);
                    continue;
                }
            }

            ContextBlock block;
            block.contents = content;
            block.blockLabels.push_back(&label);
            blocks.push_back(std::move(block));
        }

        return blocks;
    }

    std::vector<Highlight> Writer::buildHighlights(
        const LineInfo& line, 
        const std::vector<const Label*>& labels) const
    {
        std::vector<Highlight> highlights;
        for (const auto label : labels)
        {
            const auto highlight = buildHighlight(line, *label);
            if (highlight.kind != HighlightKind::None)
                highlights.push_back(highlight);
        }

        return highlights;
    }

    Highlight Writer::buildHighlight(const LineInfo& line, const Label& label) const
    {
        Highlight result{};
        result.label = &label;
        result.kind = HighlightKind::None;
        result.startColumn = 0;
        result.endColumn = 0;

        const auto labelStart = label.start();
        const auto labelEnd = label.end();
        const auto lineStart = line.start;
        const auto lineEnd = line.start + line.length;

        if (labelEnd <= lineStart || labelStart >= lineEnd + 1)
        {
            if (label.length() == 0 && labelStart == lineEnd)
            {
                result.kind = HighlightKind::Full;
                result.startColumn = offsetToDisplayColumn(line.text, line.length);
                result.endColumn = result.startColumn + 1;
                return result;
            }
            return result;
        }

        const auto startsHere = labelStart >= lineStart && labelStart <= lineEnd;
        const auto endsHere = labelEnd > lineStart && labelEnd <= lineEnd + 1;

        if (startsHere && endsHere)
        {
            result.kind = HighlightKind::Full;
            const auto localStart = labelStart - lineStart;
            const auto localEnd = labelEnd - lineStart;
            result.startColumn = offsetToDisplayColumn(line.text, localStart);
            result.endColumn = offsetToDisplayColumn(line.text, std::min(localEnd, line.length));

            if (result.endColumn <= result.startColumn)
            {
                result.endColumn = result.startColumn + 1;
            }
        }
        else if (startsHere)
        {
            result.kind = HighlightKind::Start;
            const auto localStart = labelStart - lineStart;
            result.startColumn = offsetToDisplayColumn(line.text, localStart);
            result.endColumn = expandTabs(line.text).size();
        }
        else if (endsHere)
        {
            result.kind = HighlightKind::End;
            result.startColumn = 0;
            const auto localEnd = labelEnd - lineStart;
            result.endColumn = offsetToDisplayColumn(line.text, std::min(localEnd, line.length));
        }
        else
        {
            result.kind = HighlightKind::Middle;
            result.startColumn = 0;
            result.endColumn = expandTabs(line.text).size();
        }

        return result;
    }

    std::vector<Highlight> Writer::filterSingleLineHighlights(
        const std::vector<Highlight>& highlights) const
    {
        std::vector<Highlight> singleLineHighlights;
        for (const auto& highlight : highlights)
        {
            if (highlight.kind == HighlightKind::Full)
            {
                singleLineHighlights.push_back(highlight);
            }
        }

        return singleLineHighlights;
    }

    void Writer::writeReport(
        std::ostringstream& outStream, 
        const Report& report, 
        bool isRelated) const
    {
        writeHeader(outStream, report);
        writeSource(outStream, report);
        writeFooter(outStream, report);
        writeRelated(outStream, report);
    }

    void Writer::writeHeader(
        std::ostringstream& outStream, 
        const Report& report) const
    {
        // title line
        const auto optionalTitle = report.title();
        if (optionalTitle.has_value())
        {
            const auto& title = optionalTitle.value();
            const auto level = report.level();
            const auto levelString = stringify(level);
            if (shouldUseColors())
            {
                outStream << " " << levelColor(level) << levelString << reset() << " ";
            }
            else
            {
                outStream << " " << levelString << " ";
            }

            outStream << titleColor() << title << reset();
            const auto optionalUrl = report.url();
            if (optionalUrl)
            {
                const auto& url = optionalUrl.value();
                outStream << " (" << url << ")";
            }
            outStream << "\n";
        }

        // message line
        outStream << "  " << bold() << report.message() << reset() << "\n";
    }

    void Writer::writeSource(std::ostringstream& outStream, const Report& report) const
    {
        const auto source = report.source();
        if (source == nullptr)
        {
            return;
        }

        auto labelList = report.labels();
        if (labelList.empty())
        {
            return;
        }

        std::sort(labelList.begin(), labelList.end(),
            [](const Label& lhs, const Label& rhs)
            { 
                return lhs.start() < rhs.start(); 
            });

        const auto primaryLabel = findPrimaryLabel(labelList);
        const auto colorMap = buildColorMap(labelList, primaryLabel);
        const auto blocks = buildBlocksForLabels(source, labelList);
        
        if (blocks.empty())
            return;

        const auto frameWidth = calculateFrameWidth(blocks);

        writeTopLine(outStream, source, primaryLabel, frameWidth);
        writeBlocks(outStream, blocks, frameWidth, report.level(), colorMap);
        writeBottomLine(outStream, frameWidth);
    }

    void Writer::writeTopLine(
        std::ostringstream& outStream, 
        const Source* source, 
        const Label* primaryLabel, 
        int frameWidth) const
    {
        // top line: ╭─[filename:line:col]
        
        const auto frameColorString = frameColor();
        const auto resetString = reset();
        const auto optionalSource = source->readSpan(primaryLabel->span(), 0, 0);
        std::string framePadding(frameWidth + 1, ' ');

        outStream << "  " << framePadding << frameColorString << m_theme->cornerTopLeft() << m_theme->horizontalBar() << resetString;
        if (optionalSource.has_value() && !optionalSource->name.empty())
        {
            const auto& sourceContent = optionalSource.value();
            outStream 
                << frameColorString 
                << "[" 
                << resetString 
                << sourceContent.name 
                << ":"
                << (sourceContent.startLine + 1) 
                << ":"
                << (sourceContent.startColumn + 1) 
                << frameColorString 
                << "]" 
                << resetString;
        }
        outStream << "\n";
    }

    void Writer::writeBlocks(
        std::ostringstream& outStream, 
        const std::vector<ContextBlock>& blocks,
        int frameWidth, 
        Level level, 
        const LabelColorMap& colorMap) const
    {
        for (const auto& block : blocks)
        {
            writeBlock(outStream, block.contents, block.blockLabels, frameWidth, level, colorMap);
        }
    }

    void Writer::writeBottomLine(std::ostringstream& outStream, int frameWidth) const
    {
        // bottom line: ╰────

        std::string framePadding(frameWidth + 1, ' ');
        outStream 
            << "  " 
            << framePadding 
            << frameColor() 
            << m_theme->cornerBottomLeft()
            << m_theme->horizontalBar() 
            << m_theme->horizontalBar()
            << m_theme->horizontalBar() 
            << m_theme->horizontalBar() 
            << reset()
            << "\n";
    }

    void Writer::writeBlock(
        std::ostringstream& outStream, 
        const SpanContents& contents,
        const std::vector<const Label*>& labels, 
        int frameWidth,
        Level level, 
        const LabelColorMap& colorMap) const
    {
        const auto frameColorString = frameColor();
        const auto resetString = reset();

        for (const auto& line : contents.lines)
        {
            const auto highlights = buildHighlights(line, labels);
            const auto expandedText = expandTabs(line.text);
            const auto lineDisplayWidth = expandedText.size();

            const auto columnColors = buildColumnColors(highlights, expandedText, colorMap);

            const auto lineNumberString = std::to_string(line.lineNumber);
            const auto paddedLineNumber = padLeft(lineNumberString, frameWidth);
            outStream << "  " << frameColorString << paddedLineNumber << " " << m_theme->verticalBar() << resetString << " ";

            // write the colored code
            for (int i = 0; i < lineDisplayWidth; ++i)
            {
                if (!columnColors[i].empty())
                {
                    outStream << columnColors[i] << expandedText[i] << resetString;
                }
                else
                {
                    outStream << expandedText[i];
                }
            }
            outStream << "\n";

            writeLineHighlights(outStream, line, labels, frameWidth, level, colorMap);
        }
    }

    void Writer::writeLineHighlights(
        std::ostringstream& outStream,
        const LineInfo& line,
        const std::vector<const Label*>& labels,
        int frameWidth,
        Level level,
        const LabelColorMap& colorMap) const
    {
        auto highlights = buildHighlights(line, labels);
        if (highlights.empty())
            return;

        const auto singleLineHighlights = filterSingleLineHighlights(highlights);
        if (!singleLineHighlights.empty())
        {
            writeSingleLineHighlights(outStream, line, singleLineHighlights, frameWidth, colorMap);
        }

        writeMultiLineHighlights(outStream, highlights, frameWidth, colorMap);
    }

    void Writer::writeSingleLineHighlights(
        std::ostringstream& outStream, 
        const LineInfo& line, 
        const std::vector<Highlight>& highlights, 
        int frameWidth,
        const LabelColorMap& colorMap) const
    {
        const auto frameColorString = frameColor();
        const auto resetString = reset();
        const auto expandedText = expandTabs(line.text);
        const auto lineDisplayWidth = static_cast<int>(expandedText.size());

        writeUnderlineWithMidpoints(outStream, highlights, frameWidth, colorMap);
        writeLineConnectorWithLabel(outStream, highlights, frameWidth, colorMap);
    }

    void Writer::writeMultiLineHighlights(
        std::ostringstream& outStream,
        const std::vector<Highlight>& highlights,
        int frameWidth,
        const LabelColorMap& colorMap) const
    {
        const auto frameColorString = frameColor();
        const auto resetString = reset();

        for (const auto& highlight : highlights)
        {
            if (highlight.kind == HighlightKind::Start || highlight.kind == HighlightKind::End)
            {
                const auto lineColor = colorFor(highlight.label, colorMap);
                std::string framePadding(frameWidth, ' ');

                if (highlight.kind == HighlightKind::Start)
                {
                    // the dot in the frame
                    outStream << "  " << framePadding << " " << frameColorString << m_theme->verticalBreak() << resetString << " ";

                    for (int column = 0; column < highlight.startColumn; ++column)
                    {
                        outStream << " ";
                    }
                    
                    // the arrow pointing to the code
                    outStream << lineColor << m_theme->cornerTopLeft() << m_theme->horizontalBar() << m_theme->upArrow() << resetString << "\n";
                }
                else if (highlight.kind == HighlightKind::End)
                {
                    // the vertical line in the frame
                    outStream << "  " << framePadding << " " << frameColorString << m_theme->verticalBreak() << resetString << " ";

                    for (int column = 0; column < highlight.startColumn; ++column)
                    {
                        outStream << " ";
                    }

                    // the arrow pointing to the code
                    outStream 
                        << lineColor 
                        << m_theme->cornerBottomLeft() << m_theme->horizontalBar() << m_theme->horizontalBar()
                        << m_theme->horizontalBar();
                    
                    if (highlight.label->text().has_value())
                    {
                        outStream << " " << highlight.label->text().value();
                    }
                    outStream << resetString << "\n";
                }
            }
        }
    }

    void Writer::writeUnderlineWithMidpoints(
        std::ostringstream& outStream, 
        const std::vector<Highlight>& highlights,
        int frameWidth, 
        const LabelColorMap& colorMap) const
    {
        // first line: underlines with ┬ at midpoints

        const auto frameColorString = frameColor();
        const auto resetString = reset();
        std::string framePadding(frameWidth, ' ');
        
        // the vertical line in the frame
        outStream << "  " << framePadding << " " << frameColorString << m_theme->verticalBreak() << resetString << " ";

        const auto maxColumn = calculateMaxColumn(highlights);
        for (int column = 0; column < maxColumn; ++column)
        {
            const Highlight* covering = nullptr;
            auto isMidPoint = false;
            for (const auto& highlight : highlights)
            {
                if (column >= highlight.startColumn && column < highlight.endColumn)
                {
                    covering = &highlight;
                    const auto midPoint = highlight.startColumn + (highlight.endColumn - highlight.startColumn) / 2;

                    isMidPoint = (column == midPoint);
                    break;
                }
            }

            if (covering != nullptr)
            {
                const auto lineColor = colorFor(covering->label, colorMap);
                if (isMidPoint)
                {
                    outStream << lineColor << m_theme->underConnector() << resetString;
                }
                else
                {
                    outStream << lineColor << m_theme->horizontalBar() << resetString;
                }
            }
            else
            {
                outStream << " ";
            }
        }
        outStream << "\n";
    }

    void Writer::writeLineConnectorWithLabel(
        std::ostringstream& outStream,
        const std::vector<Highlight>& highlights,
        int frameWidth, 
        const LabelColorMap& colorMap) const
    {
        const auto frameColorString = frameColor();
        const auto resetString = reset();

        for (int i = highlights.size() - 1; i >= 0; --i)
        {
            const auto& highlight = highlights[i];
            if (!highlight.label->text().has_value())
            {
                continue;
            }

            const auto lineColor = colorFor(highlight.label, colorMap);
            std::string framePadding(frameWidth, ' ');

            // the vertical line in the frame
            outStream << "  " << framePadding << " " << frameColorString << m_theme->verticalBreak() << resetString << " ";

            const auto midPoint = highlight.startColumn + (highlight.endColumn - highlight.startColumn) / 2;
            for (auto column = 0; column < midPoint; ++column)
            {
                auto needsConnector = false;
                for (int j = i - 1; j >= 0; --j)
                {
                    const auto otherMidPoint = highlights[j].startColumn + (highlights[j].endColumn - highlights[j].startColumn) / 2;
                    if (column == otherMidPoint && highlights[j].label->text().has_value())
                    {
                        needsConnector = true;
                        const auto otherLineColor = colorFor(highlights[j].label, colorMap);

                        // the connector from the other highlight
                        outStream << otherLineColor << m_theme->verticalBar() << resetString;
                        break;
                    }
                }
                if (!needsConnector)
                {
                    outStream << " ";
                }
            }

            // the arrow pointing to the code
            outStream
                << lineColor
                << m_theme->cornerBottomLeft()
                << m_theme->horizontalBar()
                << m_theme->horizontalBar()
                << " "
                << highlight.label->text().value()
                << resetString
                << "\n";
        }
    }

    void Writer::writeFooter(
        std::ostringstream& outStream,
        const Report& report) const
    {
        const auto optionalFix = report.fix();
        if (optionalFix.has_value())
        {
            outStream << "  " << fixColor() << bold() << "Fix" << reset() << ": " << optionalFix.value() << "\n";
        }
    }

    void Writer::writeRelated(
        std::ostringstream& outStream,
        const Report& report) const
    {
        const auto relatedReports = report.related();
        if (!relatedReports.empty())
        {
            outStream << "\n";
            for (const auto report : relatedReports)
            {
                if (report != nullptr)
                {
                    writeReport(outStream, *report, true);
                }
            }
        }
    }

    bool colorsEnabled()
    {
        static bool enabled = []()
            {
#ifdef _MSC_VER
                char* noColor = nullptr;
                //int length = 0;
                _dupenv_s(&noColor, nullptr, "NO_COLOR");
                bool hasNoColor = (noColor && noColor[0] != '\0');
                if (hasNoColor)
                {
                    free(noColor);
                    return false;
                }

                free(noColor);
                return true;
#else
                const char* noColor = std::getenv("NO_COLOR");
                if (noColor && noColor[0] != '\0')
                    return false;
                return true;
#endif
            }();
        return enabled;
    }

    bool enableUtf8AndAnsi()
    {
#if defined(_WIN32) || defined(_WIN64)
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        const auto outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (outputHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD mode = 0;
        if (!GetConsoleMode(outputHandle, &mode))
        {
            return false;
        }

        if (!(mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        {
            if (!SetConsoleMode(outputHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
            {
                return false;
            }
        }

        return true;
#else
        // linux terminals handle this natively
        return true; 
#endif
    }

    void printReport(const Report& report)
    {
        Writer handler;
        std::cout << handler.writeReport(report);
    }
}
