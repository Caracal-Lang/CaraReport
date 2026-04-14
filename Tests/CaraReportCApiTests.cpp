#include <CaraReport/C.h>
#include <CaraTest.h>

#include <string_view>

static bool Contains(std::string_view haystack, std::string_view needle)
{
    return haystack.find(needle) != std::string::npos;
}

static void Span()
{
    const auto span = CRSpan_create(10, 5);

    CaraTest::areEqual(span.start, 10);
    CaraTest::areEqual(span.length, 5);
    CaraTest::areEqual(CRSpan_end(span), 15);
    CaraTest::isTrue(CRSpan_isEmpty(span) == 0);
    CaraTest::isTrue(CRSpan_contains(span, 12) != 0);
    CaraTest::isTrue(CRSpan_contains(span, 15) == 0);
}

static void SpanFromRange()
{
    const auto span = CRSpan_fromRange(5, 10);

    CaraTest::areEqual(span.start, 5);
    CaraTest::areEqual(span.length, 5);
}

static void SpanOverlap()
{
    const auto first = CRSpan_create(5, 5);
    const auto second = CRSpan_create(8, 5);
    const auto third = CRSpan_create(10, 5);
    const auto fourth = CRSpan_create(15, 5);

    CaraTest::isTrue(CRSpan_overlaps(first, second) != 0);
    CaraTest::isTrue(CRSpan_overlaps(first, third) == 0);
    CaraTest::isTrue(CRSpan_overlaps(first, fourth) == 0);
}

static void Label()
{
    auto* label = CRLabel_create(10, 5, "test label", 0);
    CaraTest::isTrue(label != nullptr);
    const auto* labelText = CRLabel_text(label);

    CaraTest::areEqual(CRLabel_start(label), 10);
    CaraTest::areEqual(CRLabel_length(label), 5);
    CaraTest::isTrue(labelText != nullptr);
    CaraTest::areEqual(labelText, std::string_view("test label"));

    CRLabel_destroy(label);
}

static void SourceReadSpan()
{
    auto* source = CRSource_create("", "hello\nworld\nfoo\nbar\n");
    CRSpanContents* contents = nullptr;

    CaraTest::isTrue(source != nullptr);
    CRSource_readSpan(
        source,
        CRSpan_create(6, 5),
        0,
        0,
        &contents);

    CaraTest::isTrue(contents != nullptr);
    CaraTest::areEqual(CRSpanContents_lineCount(contents), 1);
    const auto* lineText = CRSpanContents_lineText(contents, 0);
    CaraTest::isTrue(lineText != nullptr);
    CaraTest::areEqual(lineText, std::string_view("world"));
    CaraTest::areEqual(CRSpanContents_lineNumber(contents, 0), 2);

    CRSpanContents_destroy(contents);
    CRSource_destroy(source);
}

static void SourceContextLines()
{
    auto* source = CRSource_create("", "line1\nline2\nline3\nline4\nline5\n");
    CRSpanContents* contents = nullptr;

    CaraTest::isTrue(source != nullptr);
    CRSource_readSpan(
        source,
        CRSpan_create(12, 5),
        1,
        1,
        &contents);

    CaraTest::isTrue(contents != nullptr);
    CaraTest::isTrue(CRSpanContents_lineCount(contents) >= 3);
    const auto* firstLine = CRSpanContents_lineText(contents, 0);
    const auto* secondLine = CRSpanContents_lineText(contents, 1);
    const auto* thirdLine = CRSpanContents_lineText(contents, 2);
    CaraTest::isTrue(firstLine != nullptr);
    CaraTest::isTrue(secondLine != nullptr);
    CaraTest::isTrue(thirdLine != nullptr);
    CaraTest::areEqual(firstLine, std::string_view("line2"));
    CaraTest::areEqual(secondLine, std::string_view("line3"));
    CaraTest::areEqual(thirdLine, std::string_view("line4"));

    CRSpanContents_destroy(contents);
    CRSource_destroy(source);
}

static void SourceOutOfBounds()
{
    auto* source = CRSource_create("", "hello");
    CRSpanContents* contents = nullptr;

    CaraTest::isTrue(source != nullptr);
    CRSource_readSpan(
        source,
        CRSpan_create(100, 5),
        0,
        0,
        &contents);
    CaraTest::isTrue(contents == nullptr);

    CRSource_destroy(source);
}

static void SourceHasName()
{
    auto* source = CRSource_create("test.rs", "fn main() {}");
    CRSpanContents* contents = nullptr;

    CaraTest::isTrue(source != nullptr);
    CRSource_readSpan(
        source,
        CRSpan_create(0, 2),
        0,
        0,
        &contents);

    CaraTest::isTrue(contents != nullptr);
    const auto* name = CRSpanContents_name(contents);
    CaraTest::isTrue(name != nullptr);
    CaraTest::areEqual(name, std::string_view("test.rs"));

    CRSpanContents_destroy(contents);
    CRSource_destroy(source);
}

static void ReportBuilder()
{
    auto* source = CRSource_create("test.cpp", "int x = ;");
    auto* report = CRReport_create("unexpected token");
    auto* label = CRLabel_create(8, 1, "here", 0);

    CaraTest::isTrue(source != nullptr);
    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(label != nullptr);
    CRReport_setTitle(report, "parse::unexpected");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "did you forget a value?");
    CRReport_setSource(report, source);
    CRReport_addLabelHandle(report, label);

    const auto* message = CRReport_message(report);
    const auto* title = CRReport_title(report);
    const auto* fix = CRReport_fix(report);

    CaraTest::isTrue(message != nullptr);
    CaraTest::areEqual(message, std::string_view("unexpected token"));
    CaraTest::isTrue(title != nullptr);
    CaraTest::areEqual(title, std::string_view("parse::unexpected"));
    CaraTest::isTrue(CRReport_level(report) == CARAREPORT_LEVEL_ERROR);
    CaraTest::isTrue(fix != nullptr);
    CaraTest::areEqual(fix, std::string_view("did you forget a value?"));
    CaraTest::isTrue(CRReport_hasSource(report) != 0);
    CaraTest::areEqual(CRReport_labelCount(report), 1);

    CRLabel_destroy(label);
    CRReport_destroy(report);
    CRSource_destroy(source);
}

static void WriterGeneratesOutput()
{
    auto* report = CRReport_create("oops!");
    auto* writer = CRWriter_create();

    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(writer != nullptr);
    CRReport_setTitle(report, "oops::my::bad");
    CRReport_setFix(report, "try doing it better next time?");
    CRReport_setSourceText(report, "test.rs", "source\n  text\n    here");
    CRReport_addLabel(report, 9, 4, "This bit here", 1);
    CRWriter_setColors(writer, 0);

    auto* output = CRWriter_writeReport(writer, report);
    CaraTest::isTrue(output != nullptr);
    auto outputView = std::string_view(output);

    CaraTest::contains(outputView, std::string_view("oops::my::bad"));
    CaraTest::contains(outputView, std::string_view("oops!"));
    CaraTest::contains(outputView, std::string_view("This bit here"));
    CaraTest::contains(outputView, std::string_view("try doing it better next time?"));
    CaraTest::contains(outputView, std::string_view("test.rs"));
    CaraTest::contains(outputView, std::string_view("text"));

    CRString_free(output);
    CRWriter_destroy(writer);
    CRReport_destroy(report);
}

static void WriterGeneratesOutputAsciiMode()
{
    auto* report = CRReport_create("problem");
    auto* writer = CRWriter_create();

    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(writer != nullptr);
    CRReport_setSourceText(report, "test.rs", "hello world");
    CRReport_addLabel(report, 6, 5, "this word", 0);
    CRWriter_setUnicode(writer, 0);
    CRWriter_setColors(writer, 0);

    auto* output = CRWriter_writeReport(writer, report);
    CaraTest::isTrue(output != nullptr);
    auto outputView = std::string_view(output);

    CaraTest::contains(outputView, std::string_view("problem"));
    CaraTest::contains(outputView, std::string_view("this word"));
    CaraTest::contains(outputView, std::string_view("test.rs"));

    CRString_free(output);
    CRWriter_destroy(writer);
    CRReport_destroy(report);
}

static void WriterGeneratesOutputMultipleLabels()
{
    auto* report = CRReport_create("type mismatch");
    auto* writer = CRWriter_create();

    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(writer != nullptr);
    CRReport_setSourceText(
        report,
        "test.ts",
        "let x: number = \"hello\";\nconsole.log(x + 1);");
    CRReport_addLabel(report, 7, 6, "expected 'number'", 0);
    CRReport_addLabel(report, 16, 7, "got 'string'", 1);
    CRWriter_setColors(writer, 0);

    auto* output = CRWriter_writeReport(writer, report);
    CaraTest::isTrue(output != nullptr);
    auto outputView = std::string_view(output);

    CaraTest::contains(outputView, std::string_view("type mismatch"));
    CaraTest::contains(outputView, std::string_view("expected 'number'"));
    CaraTest::contains(outputView, std::string_view("got 'string'"));

    CRString_free(output);
    CRWriter_destroy(writer);
    CRReport_destroy(report);
}

static void WriterGeneratesOutputWarningLevel()
{
    auto* report = CRReport_create("this is a warning");
    auto* writer = CRWriter_create();

    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(writer != nullptr);
    CRReport_setLevel(report, CARAREPORT_LEVEL_WARNING);
    CRWriter_setColors(writer, 0);

    auto* output = CRWriter_writeReport(writer, report);
    CaraTest::isTrue(output != nullptr);
    auto outputView = std::string_view(output);

    CaraTest::contains(outputView, std::string_view("this is a warning"));

    CRString_free(output);
    CRWriter_destroy(writer);
    CRReport_destroy(report);
}

static void WriterGeneratesOutputNoSource()
{
    auto* report = CRReport_create("no source code");
    auto* writer = CRWriter_create();

    CaraTest::isTrue(report != nullptr);
    CaraTest::isTrue(writer != nullptr);
    CRReport_setTitle(report, "simple::error");
    CRReport_setFix(report, "this is just a message");
    CRWriter_setColors(writer, 0);

    auto* output = CRWriter_writeReport(writer, report);
    CaraTest::isTrue(output != nullptr);

    auto outputView = std::string_view(output);
    CaraTest::contains(outputView, std::string_view("simple::error"));
    CaraTest::contains(outputView, std::string_view("no source code"));
    CaraTest::contains(outputView, std::string_view("this is just a message"));

    CRString_free(output);
    CRWriter_destroy(writer);
    CRReport_destroy(report);
}

static const auto tests =
{
    CaraTest::addTest("Span", Span),
    CaraTest::addTest("SpanFromRange", SpanFromRange),
    CaraTest::addTest("SpanOverlap", SpanOverlap),
    CaraTest::addTest("Label", Label),
    CaraTest::addTest("SourceReadSpan", SourceReadSpan),
    CaraTest::addTest("SourceContextLines", SourceContextLines),
    CaraTest::addTest("SourceOutOfBounds", SourceOutOfBounds),
    CaraTest::addTest("SourceHasName", SourceHasName),
    CaraTest::addTest("ReportBuilder", ReportBuilder),
    CaraTest::addTest("WriterGeneratesOutput", WriterGeneratesOutput),
    CaraTest::addTest("WriterGeneratesOutputAsciiMode", WriterGeneratesOutputAsciiMode),
    CaraTest::addTest("WriterGeneratesOutputMultipleLabels", WriterGeneratesOutputMultipleLabels),
    CaraTest::addTest("WriterGeneratesOutputWarningLevel", WriterGeneratesOutputWarningLevel),
    CaraTest::addTest("WriterGeneratesOutputNoSource", WriterGeneratesOutputNoSource)
};
