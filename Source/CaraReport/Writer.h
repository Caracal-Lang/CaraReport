#pragma once

#include <CaraReport/API.h>
#include <CaraReport/Report.h>
#include <CaraReport/Theme.h>
#include <CaraReport/Label.h>
#include <CaraReport/Source.h>
#include <CaraReport/Colors.h>

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace CaraReport
{
    using LabelColorMap = std::unordered_map<const Label*, int>;

    struct ContextBlock
    {
        SpanContents contents;
        std::vector<const Label*> blockLabels;
    };

    enum class HighlightKind : std::uint8_t
    {
        Start,   // label starts on this line
        End,     // label ends on this line
        Full,    // label is entirely on this line
        Middle,  // label passes through this line
        None     // not attached
    };

    struct Highlight
    {
        HighlightKind kind;
        int startColumn;
        int endColumn;
        const Label* label;
    };

    class CARAREPORT_API Writer
    {
    public:
        Writer();
        Writer(const Writer& other);
        Writer(Writer&& other) noexcept = default;
        Writer& operator=(const Writer& other);
        Writer& operator=(Writer&& other) noexcept = default;
        ~Writer() = default;

        [[nodiscard]] static Writer create();
        [[nodiscard]] Writer& withTheme(std::unique_ptr<Theme>&& theme);
        [[nodiscard]] Writer& withContextLines(int contextLines);
        [[nodiscard]] Writer& withTabWidth(int tabWidth);
        [[nodiscard]] Writer& withUnicode(bool enable);
        [[nodiscard]] Writer& withColors(bool enable);

        [[nodiscard]] std::string writeReport(const Report& report) const;

      private:
        [[nodiscard]] bool shouldUseColors() const;
        [[nodiscard]] std::string levelColor(Level level) const;
        [[nodiscard]] std::string titleColor() const;
        [[nodiscard]] std::string frameColor() const;
        [[nodiscard]] std::string fixColor() const;
        [[nodiscard]] std::string bold() const;
        [[nodiscard]] std::string reset() const;
        [[nodiscard]] std::string colorFor(
            const Label* label, 
            const LabelColorMap& colorMap) const;
        [[nodiscard]] std::vector<std::string> buildColumnColors(
            const std::vector<Highlight>& highlights, 
            const std::string& expandedText, 
            const LabelColorMap& colorMap) const;

        void writeReport(
            std::ostringstream& outStream, 
            const Report& report, 
            bool isRelated) const;
        void writeHeader(
            std::ostringstream& outStream, 
            const Report& report) const;
        void writeSource(
            std::ostringstream& outStream, 
            const Report& report) const;
        void writeTopLine(
            std::ostringstream& outStream, 
            const Source* source, 
            const Label* primaryLabel, 
            int frameWidth) const;
        void writeBlocks(
            std::ostringstream& outStream, 
            const std::vector<ContextBlock>& blocks, 
            int frameWidth,
            Level level, 
            const LabelColorMap& colorMap) const;
        void writeBottomLine(
            std::ostringstream& outStream, 
            int frameWidth) const;
        void writeBlock(
            std::ostringstream& outStream,
            const SpanContents& contents,
            const std::vector<const Label*>& labels,
            int frameWidth, 
            Level level,
            const LabelColorMap& colorMap) const;
        void writeLineHighlights(
            std::ostringstream& out, 
            const LineInfo& line,
            const std::vector<const Label*>& labels,
            int frameWidth, 
            Level level,
            const LabelColorMap& colorMap) const;
        void writeSingleLineHighlights(
            std::ostringstream& outStream, 
            const LineInfo& line, 
            const std::vector<Highlight>& highlights, 
            int frameWidth, 
            const LabelColorMap& colorMap) const;
        void writeMultiLineHighlights(
            std::ostringstream& outStream, 
            const std::vector<Highlight>& highlights, 
            int frameWidth, 
            const LabelColorMap& colorMap) const;
        void writeUnderlineWithMidpoints(
            std::ostringstream& outStream, 
            const std::vector<Highlight>& highlights, 
            int frameWidth, 
            const LabelColorMap& colorMap) const;
        void writeLineConnectorWithLabel(
            std::ostringstream& outStream, 
            const std::vector<Highlight>& highlights, 
            int frameWidth, 
            const LabelColorMap& colorMap) const;
        void writeFooter(
            std::ostringstream& outStream, 
            const Report& report) const;
        void writeRelated(
            std::ostringstream& outStream, 
            const Report& report) const;

        std::unique_ptr<Theme> m_theme;
        int m_contextLines{2};
        int m_tabWidth{4};
        bool m_colorsEnabled;
        bool m_ansiSupported;
    };

    [[nodiscard]] CARAREPORT_API bool colorsEnabled();
    [[nodiscard]] CARAREPORT_API bool enableUtf8AndAnsi();

    CARAREPORT_API void printReport(const Report& report);
}
