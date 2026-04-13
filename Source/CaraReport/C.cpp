#include <CaraReport/C.h>

#include <CaraReport/Report.h>
#include <CaraReport/Source.h>
#include <CaraReport/Theme.h>
#include <CaraReport/Writer.h>

#include <optional>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

struct CRLabel
{
    CRLabel(int start, int length, const char* label, int primary)
        : value(start, length, label != nullptr ? label : "", primary != 0)
    {
    }

    CaraReport::Label value;
};

struct CRReport
{
    explicit CRReport(const char* message)
        : value(message != nullptr ? message : "")
    {
    }

    CaraReport::Report value;
};

struct CRSource
{
    CRSource(const char* name, const char* sourceText)
        : value(name != nullptr ? name : "", sourceText != nullptr ? sourceText : "")
    {
    }

    CaraReport::Source value;
};

struct CRSpanContents
{
    explicit CRSpanContents(CaraReport::SpanContents spanContents)
        : value(std::move(spanContents))
    {
    }

    CaraReport::SpanContents value;
};

struct CRTheme
{
    explicit CRTheme(CaraReport::Theme themeValue)
        : value(std::move(themeValue))
    {
    }

    CaraReport::Theme value;
};

struct CRWriter
{
    CaraReport::Writer value;
};

namespace
{    
    [[nodiscard]] const char* emptyIfNull(const char* value)
    {
        return value != nullptr ? value : "";
    }

    [[nodiscard]] CaraReport::Span toCppSpan(CRSpan span)
    {
        return {span.start, span.length};
    }

    [[nodiscard]] CRSpan toCSpan(const CaraReport::Span& span)
    {
        return {span.start(), span.length()};
    }

    [[nodiscard]] CaraReport::Level toCppLevel(CRLevel level)
    {
        switch (level)
        {
        case CARAREPORT_LEVEL_WARNING:
            return CaraReport::Level::Warning;
        case CARAREPORT_LEVEL_INFO:
            return CaraReport::Level::Info;
        case CARAREPORT_LEVEL_ERROR:
            return CaraReport::Level::Error;
        }
        return CaraReport::Level::Error;
    }

    [[nodiscard]] CRLevel toCLevel(CaraReport::Level level)
    {
        switch (level)
        {
        case CaraReport::Level::Warning:
            return CARAREPORT_LEVEL_WARNING;
        case CaraReport::Level::Info:
            return CARAREPORT_LEVEL_INFO;
        case CaraReport::Level::Error:
            return CARAREPORT_LEVEL_ERROR;
        }
        return CARAREPORT_LEVEL_ERROR;
    }

    [[nodiscard]] char* duplicateCString(const std::string& value)
    {
        auto* buffer = static_cast<char*>(std::malloc(value.size() + 1));
        if (buffer == nullptr)
        {
            return nullptr;
        }

        std::memcpy(buffer, value.c_str(), value.size() + 1);
        return buffer;
    }

    [[nodiscard]] char* duplicateOptionalString(const std::optional<std::string>& value)
    {
        if (!value.has_value())
        {
            return nullptr;
        }

        return duplicateCString(value.value());
    }
}

extern "C"
{
    CRSpan CRSpan_create(int start, int length)
    {
        return {start, length};
    }

    CRSpan CRSpan_fromRange(int start, int end)
    {
        return {start, end - start};
    }

    int CRSpan_end(CRSpan span)
    {
        return span.start + span.length;
    }

    int CRSpan_isEmpty(CRSpan span)
    {
        return span.length == 0;
    }

    int CRSpan_contains(CRSpan span, int position)
    {
        return position >= span.start && position < CRSpan_end(span);
    }

    int CRSpan_overlaps(CRSpan left, CRSpan right)
    {
        return left.start < CRSpan_end(right) && right.start < CRSpan_end(left);
    }

    CRLabel* CRLabel_create(int start, int length, const char* label, int primary)
    {
        return new CRLabel(start, length, label, primary);
    }

    void CRLabel_destroy(CRLabel* label)
    {
        delete label;
    }

    CRSpan CRLabel_span(const CRLabel* label)
    {
        if (label == nullptr)
        {
            return {0, 0};
        }

        return toCSpan(label->value.span());
    }

    char* CRLabel_text(const CRLabel* label)
    {
        if (label == nullptr)
        {
            return nullptr;
        }

        return duplicateOptionalString(label->value.text());
    }

    int CRLabel_isPrimary(const CRLabel* label)
    {
        return label != nullptr && label->value.isPrimary();
    }

    int CRLabel_start(const CRLabel* label)
    {
        return label != nullptr ? label->value.start() : 0;
    }

    int CRLabel_end(const CRLabel* label)
    {
        return label != nullptr ? label->value.end() : 0;
    }

    int CRLabel_length(const CRLabel* label)
    {
        return label != nullptr ? label->value.length() : 0;
    }

    CRReport* CRReport_create(const char* message)
    {
        return new CRReport(message);
    }

    void CRReport_destroy(CRReport* report)
    {
        delete report;
    }

    void CRReport_setLevel(CRReport* report, CRLevel level)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withLevel(toCppLevel(level)));
    }

    void CRReport_setTitle(CRReport* report, const char* title)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withTitle(emptyIfNull(title)));
    }

    void CRReport_setUrl(CRReport* report, const char* url)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withUrl(emptyIfNull(url)));
    }

    void CRReport_setFix(CRReport* report, const char* fix)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withFix(emptyIfNull(fix)));
    }

    void CRReport_setSource(CRReport* report, const CRSource* source)
    {
        if (report == nullptr || source == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withSource(
            std::make_unique<CaraReport::Source>(source->value)));
    }

    void CRReport_setSourceText(CRReport* report, const char* name, const char* sourceText)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withSource(
            std::make_unique<CaraReport::Source>(emptyIfNull(name), emptyIfNull(sourceText))));
    }

    void CRReport_addLabelHandle(CRReport* report, const CRLabel* label)
    {
        if (report == nullptr || label == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withLabel(label->value));
    }

    void CRReport_addRelated(CRReport* report, const CRReport* related)
    {
        if (report == nullptr || related == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withRelated(&related->value));
    }

    void CRReport_addLabel(CRReport* report, int start, int length, const char* label, int primary)
    {
        if (report == nullptr)
        {
            return;
        }

        static_cast<void>(report->value.withLabel(
            CaraReport::Label(start, length, emptyIfNull(label), primary != 0)));
    }

    CRLevel CRReport_level(const CRReport* report)
    {
        if (report == nullptr)
        {
            return CARAREPORT_LEVEL_ERROR;
        }

        return toCLevel(report->value.level());
    }

    char* CRReport_title(const CRReport* report)
    {
        if (report == nullptr)
        {
            return nullptr;
        }

        return duplicateOptionalString(report->value.title());
    }

    char* CRReport_url(const CRReport* report)
    {
        if (report == nullptr)
        {
            return nullptr;
        }

        return duplicateOptionalString(report->value.url());
    }

    char* CRReport_message(const CRReport* report)
    {
        if (report == nullptr)
        {
            return nullptr;
        }

        return duplicateCString(report->value.message());
    }

    char* CRReport_fix(const CRReport* report)
    {
        if (report == nullptr)
        {
            return nullptr;
        }

        return duplicateOptionalString(report->value.fix());
    }

    int CRReport_hasSource(const CRReport* report)
    {
        return report != nullptr && report->value.source() != nullptr;
    }

    int CRReport_labelCount(const CRReport* report)
    {
        return report != nullptr ? static_cast<int>(report->value.labels().size()) : 0;
    }

    CRSource* CRSource_create(const char* name, const char* sourceText)
    {
        return new CRSource(name, sourceText);
    }

    void CRSource_destroy(CRSource* source)
    {
        delete source;
    }

    void CRSource_setName(CRSource* source, const char* name)
    {
        if (source == nullptr)
        {
            return;
        }

        source->value.setName(emptyIfNull(name));
    }

    void CRSource_setText(CRSource* source, const char* sourceText)
    {
        if (source == nullptr)
        {
            return;
        }

        source->value.setSource(emptyIfNull(sourceText));
    }

    void CRSource_readSpan(const CRSource* source, CRSpan span, int contextLinesBefore, int contextLinesAfter, CRSpanContents** contents)
    {
        if (source == nullptr || contents == nullptr)
        {
            return;
        }

        *contents = nullptr;

        const auto spanContents = source->value.readSpan(
            toCppSpan(span),
            contextLinesBefore,
            contextLinesAfter);

        if (!spanContents.has_value())
        {
            return;
        }

        *contents = new CRSpanContents(spanContents.value());
    }

    void CRSpanContents_destroy(CRSpanContents* contents)
    {
        delete contents;
    }

    char* CRSpanContents_name(const CRSpanContents* contents)
    {
        if (contents == nullptr)
        {
            return nullptr;
        }

        return duplicateCString(contents->value.name);
    }

    int CRSpanContents_startLine(const CRSpanContents* contents)
    {
        return contents != nullptr ? contents->value.startLine : 0;
    }

    int CRSpanContents_startColumn(const CRSpanContents* contents)
    {
        return contents != nullptr ? contents->value.startColumn : 0;
    }

    int CRSpanContents_lineCount(const CRSpanContents* contents)
    {
        return contents != nullptr ? static_cast<int>(contents->value.lines.size()) : 0;
    }

    int CRSpanContents_lineNumber(const CRSpanContents* contents, int index)
    {
        if (contents == nullptr || index < 0)
        {
            return 0;
        }

        const auto lineIndex = static_cast<size_t>(index);
        if (lineIndex >= contents->value.lines.size())
        {
            return 0;
        }

        return contents->value.lines[lineIndex].lineNumber;
    }

    char* CRSpanContents_lineText(const CRSpanContents* contents, int index)
    {
        if (contents == nullptr || index < 0)
        {
            return nullptr;
        }

        const auto lineIndex = static_cast<size_t>(index);
        if (lineIndex >= contents->value.lines.size())
        {
            return nullptr;
        }

        return duplicateCString(contents->value.lines[lineIndex].text);
    }

    CRTheme* CRTheme_create(void)
    {
        return new CRTheme(CaraReport::Theme());
    }

    CRTheme* CRTheme_ascii(void)
    {
        return new CRTheme(*CaraReport::Theme::ascii());
    }

    void CRTheme_destroy(CRTheme* theme)
    {
        delete theme;
    }

    void CRTheme_setLevelColor(CRTheme* theme, CRLevel level, const char* color)
    {
        if (theme == nullptr)
        {
            return;
        }

        theme->value.setLevelColor(toCppLevel(level), emptyIfNull(color));
    }

    void CRTheme_setFrameColor(CRTheme* theme, const char* color)
    {
        if (theme == nullptr)
        {
            return;
        }

        theme->value.setFrameColor(emptyIfNull(color));
    }

    void CRTheme_setLabelPaletteColor(CRTheme* theme, int index, const char* color)
    {
        if (theme == nullptr || index < 0)
        {
            return;
        }

        auto& palette = theme->value.labelPalette();
        const auto paletteIndex = static_cast<size_t>(index);
        if (paletteIndex >= palette.size())
        {
            return;
        }

        palette[paletteIndex] = emptyIfNull(color);
    }

    CRWriter* CRWriter_create(void)
    {
        return new CRWriter();
    }

    void CRWriter_destroy(CRWriter* writer)
    {
        delete writer;
    }

    void CRWriter_setTheme(CRWriter* writer, const CRTheme* theme)
    {
        if (writer == nullptr || theme == nullptr)
        {
            return;
        }

        static_cast<void>(writer->value.withTheme(
            std::make_unique<CaraReport::Theme>(theme->value)));
    }

    void CRWriter_setContextLines(CRWriter* writer, int contextLines)
    {
        if (writer == nullptr)
        {
            return;
        }

        static_cast<void>(writer->value.withContextLines(contextLines));
    }

    void CRWriter_setTabWidth(CRWriter* writer, int tabWidth)
    {
        if (writer == nullptr)
        {
            return;
        }

        static_cast<void>(writer->value.withTabWidth(tabWidth));
    }

    void CRWriter_setUnicode(CRWriter* writer, int enable)
    {
        if (writer == nullptr)
        {
            return;
        }

        static_cast<void>(writer->value.withUnicode(enable != 0));
    }

    void CRWriter_setColors(CRWriter* writer, int enable)
    {
        if (writer == nullptr)
        {
            return;
        }

        static_cast<void>(writer->value.withColors(enable != 0));
    }

    char* CRWriter_writeReport(const CRWriter* writer, const CRReport* report)
    {
        if (writer == nullptr || report == nullptr)
        {
            return nullptr;
        }

        return duplicateCString(writer->value.writeReport(report->value));
    }

    char* CRReport_write(const CRReport* report)
    {
        if (report == nullptr)
        {
            return nullptr;
        }

        return duplicateCString(CaraReport::Writer::create().writeReport(report->value));
    }

    void CRString_free(char* text)
    {
        std::free(text);
    }
}
