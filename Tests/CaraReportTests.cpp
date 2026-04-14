#include <CaraReport.h>
#include <CaraTest.h>
#include <iostream>

static void Span()
{
    CaraReport::Span span(10, 5);
    
    CaraTest::areEqual(span.start(), 10);
    CaraTest::areEqual(span.length(), 5);
    CaraTest::areEqual(span.end(), 15);
    CaraTest::isTrue(!span.isEmpty());
    CaraTest::isTrue(span.contains(12));
    CaraTest::isTrue(!span.contains(15));
}

static void SpanFromRange()
{
    auto span = CaraReport::Span::fromRange(5, 10);

    CaraTest::areEqual(span.start(), 5);
    CaraTest::areEqual(span.length(), 5);
}

static void SpanOverlap()
{
    CaraReport::Span a(5, 5);  // 5..10
    CaraReport::Span b(8, 5);  // 8..13
    CaraReport::Span c(10, 5); // 10..15
    CaraReport::Span d(15, 5); // 15..20

    CaraTest::isTrue(a.overlaps(b));
    CaraTest::isTrue(!a.overlaps(c)); // adjacent, not overlapping
    CaraTest::isTrue(!a.overlaps(d));
}

static void Label()
{
    auto label = CaraReport::Label(10, 5, "test label");

    CaraTest::areEqual(label.start(), 10u);
    CaraTest::areEqual(label.length(), 5u);
    CaraTest::isTrue(!label.text().empty());
    CaraTest::areEqual(label.text(), "test label");
}

static void SourceReadSpan()
{
    CaraReport::Source source("", "hello\nworld\nfoo\nbar\n");
    auto contents = source.readSpan(CaraReport::Span(6, 5), 0, 0);
    
    CaraTest::isTrue(contents.has_value());
    CaraTest::areEqual(contents->lines.size(), 1u);
    CaraTest::areEqual(contents->lines[0].text, "world");
    CaraTest::areEqual(contents->lines[0].lineNumber, 2u);
}

static void SourceContextLines()
{
    CaraReport::Source source("", "line1\nline2\nline3\nline4\nline5\n");
    auto contents = source.readSpan(CaraReport::Span(12, 5), 1, 1);

    CaraTest::isTrue(contents.has_value());
    // Should include line2, line3, line4 (1 context line before and after)
    CaraTest::isTrue(contents->lines.size() >= 3);
    CaraTest::areEqual(contents->lines[0].text, "line2");
    CaraTest::areEqual(contents->lines[1].text, "line3");
    CaraTest::areEqual(contents->lines[2].text, "line4");
}

static void SourceOutOfBounds()
{
    CaraReport::Source source("", "hello");
    auto contents = source.readSpan(CaraReport::Span(100, 5), 0, 0);
    
    CaraTest::isTrue(!contents.has_value());
}

static void SourceHasName()
{
    CaraReport::Source source("test.rs", "fn main() {}");
    auto contents = source.readSpan(CaraReport::Span(0, 2), 0, 0);
    
    CaraTest::isTrue(contents.has_value());
    CaraTest::areEqual(contents->name, "test.rs");
}

static void ReportBuilder()
{
    auto source = std::make_unique<CaraReport::Source>("test.cpp", "int x = ;");
    auto report = CaraReport::Report("unexpected token")
        .withTitle("parse::unexpected")
        .withLevel(CaraReport::Level::Error)
        .withFix("did you forget a value?")
        .withSource(std::move(source))
        .withLabel(CaraReport::Label(8, 1, "here"));

    CaraTest::areEqual(report.message(), "unexpected token");
    CaraTest::isTrue(!report.title().empty());
    CaraTest::areEqual(report.title(), "parse::unexpected");
    CaraTest::isTrue(report.level() == CaraReport::Level::Error);
    CaraTest::isTrue(!report.fix().empty());
    CaraTest::areEqual(report.fix(), "did you forget a value?");
    CaraTest::isTrue(report.source() != nullptr);
    CaraTest::areEqual(report.labels().size(), 1u);
}

static void WriterGeneratesOutput()
{
    auto source = std::make_unique<CaraReport::Source>("test.rs", "source\n  text\n    here");
    auto report = CaraReport::Report("oops!")
        .withTitle("oops::my::bad")
        .withFix("try doing it better next time?")
        .withSource(std::move(source))
        .withLabel(CaraReport::Label(9, 4, "This bit here"));

    // Disable colors for string comparison
    auto writer = CaraReport::Writer::create()
        .withColors(false);
    auto output = writer.writeReport(report);

    CaraTest::contains(output, "oops::my::bad");
    CaraTest::contains(output, "oops!");
    CaraTest::contains(output, "This bit here");
    CaraTest::contains(output, "try doing it better next time?");
    CaraTest::contains(output, "test.rs");
    CaraTest::contains(output, "text");
}

static void WriterGeneratesOutputAsciiMode()
{
    auto source = std::make_unique<CaraReport::Source>("test.rs", "hello world");
    auto report = CaraReport::Report("problem")
        .withSource(std::move(source))
        .withLabel(CaraReport::Label(6, 5, "this word"));

    auto writer = CaraReport::Writer::create()
        .withUnicode(false)
        .withColors(false);
    auto output = writer.writeReport(report);

    CaraTest::contains(output, "problem");
    CaraTest::contains(output, "this word");
    CaraTest::contains(output, "test.rs");
}

static void WriterGeneratesOutputMultipleLabels()
{
    auto source = std::make_unique<CaraReport::Source>(
        "test.ts", "let x: number = \"hello\";\nconsole.log(x + 1);");
    auto report = CaraReport::Report("type mismatch")
        .withSource(std::move(source))
        .withLabel(CaraReport::Label(CaraReport::Span(7, 6),"expected 'number'", false))
        .withLabel(CaraReport::Label(CaraReport::Span(16, 7),"got 'string'", true));

    auto writer = CaraReport::Writer::create()
        .withColors(false);
    auto output = writer.writeReport(report);

    CaraTest::contains(output, "type mismatch");
    CaraTest::contains(output, "expected 'number'");
    CaraTest::contains(output, "got 'string'");
}

static void WriterGeneratesOutputWarningLevel()
{
    auto report = CaraReport::Report("this is a warning")
        .withLevel(CaraReport::Level::Warning);

    auto writer = CaraReport::Writer::create()
        .withColors(false);
    auto output = writer.writeReport(report);

    CaraTest::contains(output, "this is a warning");
}

static void WriterGeneratesOutputNoSource()
{
    auto report = CaraReport::Report("no source code")
        .withTitle("simple::error")
        .withFix("this is just a message");

    auto writer = CaraReport::Writer::create()
        .withColors(false);
    auto output = writer.writeReport(report);

    CaraTest::contains(output, "simple::error");
    CaraTest::contains(output, "no source code");
    CaraTest::contains(output, "this is just a message");
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
