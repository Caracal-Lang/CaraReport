#pragma once

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

    enum class HighlightKind
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

    class Writer
    {
    public:
        Writer();
        Writer(const Writer& other);
        Writer(Writer&& other) noexcept = default;
        Writer& operator=(const Writer& other);
        Writer& operator=(Writer&& other) noexcept = default;

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
        [[nodiscard]] LabelColorMap buildColorMap(
            const std::vector<Label>& labels,
            const Label* primaryLabel) const;
        [[nodiscard]] std::vector<std::string> buildColumnColors(
            const std::vector<Highlight>& highlights, 
            const std::string& expandedText, 
            const LabelColorMap& colorMap) const;

        [[nodiscard]] int lineNumberWidth(const std::vector<LineInfo>& lines) const;
        [[nodiscard]] std::string padLeft(const std::string& text, int width) const;
        [[nodiscard]] std::string expandTabs(const std::string& text) const;
        [[nodiscard]] int offsetToDisplayColumn(const std::string& text, int offset) const;
        [[nodiscard]] int calculateFrameWidth(const std::vector<ContextBlock>& blocks) const;
        [[nodiscard]] int calculateMaxColumn(const std::vector<Highlight>& highlights) const;

        [[nodiscard]] const Label* findPrimaryLabel(
            const std::vector<Label>& labels) const;
        [[nodiscard]] std::vector<ContextBlock> buildBlocksForLabels(
            const Source* source, 
            const std::vector<Label>& labels) const;
        [[nodiscard]] std::vector<Highlight> buildHighlights(
            const LineInfo& line, 
            const std::vector<const Label*>& labels) const;
        [[nodiscard]] Highlight buildHighlight(
            const LineInfo& line, 
            const Label& label) const;
        [[nodiscard]] std::vector<Highlight> filterSingleLineHighlights(
            const std::vector<Highlight>& highlights) const;

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
        int m_contextLines;
        int m_tabWidth;
        bool m_colorsEnabled;
        bool m_ansiSupported;
    };

    [[nodiscard]] bool colorsEnabled();
    [[nodiscard]] bool enableUtf8AndAnsi();

    void printReport(const Report& report);
}
