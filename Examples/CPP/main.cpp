#include <CaraReport.h>
#include <iostream>
#include <memory>

using namespace CaraReport;

static void example_1()
{
    std::cout << " Example 1: Simple parse error \n\n";

    // Error oops::my::bad
    //  oops!
    //    ╭─[bad_file.rs:3:3]
    //  1 │ source
    //  2 │   text
    //    ·   ──┬─
    //    ·     ╰── This bit here
    //  3 │     here
    //    ╰────
    //  Fix: try doing it better next time?

    auto source = std::make_unique<Source>("bad_file.rs", "source\n  text\n    here");
    auto report = Report("oops!")
        .withTitle("oops::my::bad")
        .withLevel(Level::Error)
        .withFix("try doing it better next time?")
        .withSource(std::move(source))
        .withLabel(Label(9, 4, "This bit here", true));

    printReport(report);
}

static void example_2()
{
    std::cout << "\n Example 2: Ascii theme \n\n";

    // Error oops::my::bad
    //  oops!
    //    ,-[bad_file.rs:3:3]
    //  1 | source
    //  2 |   text
    //    .   ----
    //    .     `-- This bit here
    //  3 |     here
    //    `----
    //  Fix: try doing it better next time?

    auto source = std::make_unique<Source>("bad_file.rs", "source\n  text\n    here");
    auto report = Report("oops!")
        .withTitle("oops::my::bad")
        .withLevel(Level::Error)
        .withFix("try doing it better next time?")
        .withSource(std::move(source))
        .withLabel(Label(Span(9, 4), "This bit here", true));

    auto writer = Writer::create().withUnicode(false);
    std::cerr << writer.writeReport(report);
}

static void example_3()
{
    std::cout << "\n Example 3: Multiple labels \n\n";

    // Error ts(2322)
    //  Type 'string' is not assignable to type 'number'
    //    ╭─[example.ts:2:17]
    //  1 │ let x: number = "hello";
    //    ·        ───┬──   ───┬───
    //    ·           │        ╰── got 'string'
    //    ·           ╰── expected 'number'
    //  2 │ console.log(x + 1);
    //    ╰────
    //  Fix: Change the type annotation or the assigned value

    auto source = std::make_unique<Source>(
        "example.ts", "let x: number = \"hello\";\nconsole.log(x + 1);");
    auto report = Report("Type 'string' is not assignable to type 'number'")
        .withTitle("ts(2322)")
        .withLevel(Level::Error)
        .withFix("Change the type annotation or the assigned value")
        .withSource(std::move(source))
        .withLabel(Label(Span(7, 6), "expected 'number'", false))
        .withLabel(Label(Span(16, 7), "got 'string'", true));

    printReport(report);
}

static void example_4()
{
    std::cout << "\n Example 4: Warning with URL \n\n";

        // Warning W0602 (https://docs.python.org/3/library/subprocess.html)
        //  os.popen() is deprecated since Python 3.0
        //    ╭─[app.py:3:13]
        //  1 │ import os
        //  2 │ result = os.popen('ls')
        //    ·             ──┬──
        //    ·               ╰── deprecated call
        //  3 │ print(result.read())
        //    ╰────
        //  Fix: Use subprocess.run() or subprocess.Popen() instead

    auto source = std::make_unique<Source>(
        "app.py", "import os\nresult = os.popen('ls')\nprint(result.read())");
    auto report = Report("os.popen() is deprecated since Python 3.0")
        .withTitle("W0602")
        .withLevel(Level::Warning)
        .withFix("Use subprocess.run() or subprocess.Popen() instead")
        .withUrl("https://docs.python.org/3/library/subprocess.html")
        .withSource(std::move(source))
        .withLabel(Label(Span(22, 5), "deprecated call", true));

    printReport(report);
}

static void example_5()
{
    std::cout << "\n Example 5: Info note\n\n";

        // Info note::feature_x
        //  Feature X is available
        //    ╭─[config.toml:2:1]
        //  1 │ enable_feature = false
        //    · ───────┬──────
        //    ·        ╰── toggle here
        //  2 │ # set to true to opt-in
        //    ╰────
        //  Fix: Consider enabling the feature in config.toml if desired

    auto source = std::make_unique<Source>(
        "config.toml", "enable_feature = false\n# set to true to opt-in");

    auto report = Report("Feature X is available")
        .withTitle("note::feature_x")
        .withLevel(Level::Info)
        .withFix("Consider enabling the feature in config.toml if desired")
        .withSource(std::move(source))
        .withLabel(Label(Span(0, 14), "toggle here", true));

    printReport(report);
}

static void example_6()
{
    std::cout << "\n Example 6: Simple error\n\n";

        // Error math::precedence
        //  Wrong answer
        //    ╭─[math.txt:2:13]
        //  1 │ 2 + 2 * 2 = 8
        //    ·             ┬
        //    ·             ╰── this should be 6
        //    ╰────
        //  Fix: '*' has greater precedence than '+', so 2 + 2 * 2 = 6

    auto source = std::make_unique<Source>("math.txt", "2 + 2 * 2 = 8");

    auto report = Report("Wrong answer")
        .withTitle("math::precedence")
        .withFix("'*' has greater precedence than '+', so 2 + 2 * 2 = 6")
        .withSource(std::move(source))
        .withLabel(Label(12, 1, "this should be 6"));

    printReport(report);
}

static void example_7()
{
    std::cout << "\n Example 7: Related reports \n\n";

        // Error root::err
        //  Top-level error referencing a note
        //    ╭─[lib.rs:4:5]
        //  1 │ fn call_old() {
        //  2 │     // deprecated usage
        //  3 │     old_api();
        //    ·     ───┬───
        //    ·        ╰── deprecated call
        //  4 │ }
        //  5 │
        //    ╰────
        //  Fix: Replace call_old -> call_new or update usage
        //
        // Info note::related
        //  This is a related note
        //    ╭─[note.txt:3:1]
        //  1 │ This helper explains why this API is deprecated:
        //  2 │ Use new_api() instead.
        //    · ───────────┬──────────
        //    ·            ╰── explanation
        //  3 │
        //    ╰────
        //  Fix: No action needed

    auto relatedSource = std::make_unique<Source>(
        "note.txt",
        "This helper explains why this API is deprecated:\nUse new_api() instead.\n");

    auto relatedReport = Report("This is a related note")
        .withLevel(Level::Info)
        .withTitle("note::related")
        .withFix("No action needed")
        .withSource(std::move(relatedSource))
        .withLabel(Label(49, 22, "explanation"));

    auto source = std::make_unique<Source>(
        "lib.rs",
        "fn call_old() {\n    // deprecated usage\n    old_api();\n}\n");

    auto report = Report("Top-level error referencing a note")
        .withTitle("root::err")
        .withLevel(Level::Error)
        .withFix("Replace call_old -> call_new or update usage")
        .withSource(std::move(source))
        .withLabel(Label(44, 7, "deprecated call"))
        .withRelated(&relatedReport);

    printReport(report);
}

static void example_8()
{
    std::cout << "\n Example 8: Custom theme and colors \n\n";

        // Info theme::info
        //  Custom themed info
        //    ,-[theme.conf:3:1]
        //  1 | [settings]
        //  2 | enable_feature = true
        //    . --------------
        //    .        `-- setting enabled
        //  3 | log_level = info
        //  4 |
        //    `----

    auto source = std::make_unique<Source>(
        "theme.conf",
        "[settings]\nenable_feature = true\nlog_level = info\n");

    auto report = Report("Custom themed info")
        .withTitle("theme::info")
        .withLevel(Level::Info)
        .withSource(std::move(source))
        .withLabel(Label(11, 14, "setting enabled"));

    auto theme = Theme::ascii();
    theme->setLevelColor(Level::Info, Colors::pastelPink());
    theme->setFrameColor(Colors::pastelOrange());
    theme->labelPalette()[0] = Colors::pastelPurple();

    auto writer = Writer::create()
        .withTheme(std::move(theme))
        .withColors(true)
        .withUnicode(false);
    std::cout << writer.writeReport(report);
}

static void example_9()
{
    std::cout << "\n Example 9: Tab width and context lines \n\n";

        //  Tab-handling example
        //    ╭─[tabs.txt:2:2]
        //  1 │         fn main() {
        //    ·         ─┬
        //    ·          ╰── call site
        //  2 │                 println!("hi");
        //  3 │ }
        //    ╰────

    auto source = std::make_unique<Source>(
        "tabs.txt", "\tfn main() {\n\t\tprintln!(\"hi\");\n}\n");

    auto report = Report("Tab-handling example")
        .withLevel(Level::Warning)
        .withSource(std::move(source))
        .withLabel(Label(1, 2, "call site"));

    auto writer = Writer::create()
        .withTabWidth(8).withContextLines(2);
    std::cout << writer.writeReport(report);
}

static void example_10()
{
    std::cout << "\n Example 10: Multiline span \n\n";

        // Error E0308
        //  mismatched types
        //    ╭─[script.rs:3:13]
        //  1 │ fn main() {
        //  2 │     let x = vec![
        //    ·             ╭─^
        //  3 │         1, 2, 3,
        //  4 │         4, 5, 6,
        //  5 │     ];
        //    · ╰─── expected Vec<String>, found Vec<i32>
        //  6 │ }
        //  7 │
        //    ╰────
        //  Fix: Consider converting the vec to the expected type

    auto source = std::make_unique<Source>(
        "script.rs",
        "fn main() {\n    let x = vec![\n        1, 2, 3,\n        4, 5, 6,\n    ];\n}\n");

    auto report = Report("mismatched types")
        .withTitle("E0308")
        .withFix("Consider converting the vec to the expected type")
        .withLevel(Level::Error)
        .withSource(std::move(source))
        .withLabel(Label(Span(24, 46), "expected Vec<String>, found Vec<i32>", true));

    printReport(report);
}

static void example_11()
{
    std::cout << "\n Example 11: Three labels (long) \n\n";

        // Error sql::type_check
        //  3 type errors found in query
        //    ╭─[query.sql:2:48]
        //  1 │ SELECT name, age, email FROM users WHERE age > 'thirty' AND active = 7.5 AND role IN 42
        //    ·                                                ────┬───              ─┬─             ─┬
        //    ·                                                    │                  │               ╰── expected list, got integer
        //    ·                                                    │                  ╰── expected boolean, got float
        //    ·                                                    ╰── expected integer, got string
        //    ╰────
        //  Fix: Use integer literals for numeric columns, strings for text columns, and a list for IN clauses

    auto source = std::make_unique<Source>(
        "query.sql", "SELECT name, age, email FROM users WHERE age > "
        "'thirty' AND active = 7.5 AND role IN 42");

    auto report = Report("3 type errors found in query")
        .withTitle("sql::type_check")
        .withFix("Use integer literals for numeric columns, strings for text columns, and a list for IN clauses")
        .withSource(std::move(source))
        .withLabel(Label(47, 8, "expected integer, got string", true))
        .withLabel(Label(69, 3, "expected boolean, got float", false))
        .withLabel(Label(85, 2, "expected list, got integer", false));

    printReport(report);
}

static void example_12()
{
    std::cout << "\n Example 12: Three labels (short) \n\n";

        // Error sql::type
        //  Type mismatch in query
        //    ╭─[short.sql:2:35]
        //  1 │ SELECT age FROM users WHERE age > 'thirty'
        //    ·                       ──┬── ─┬─   ────┬───
        //    ·                         │    │        ╰── expected integer
        //    ·                         │    ╰── column 'age'
        //    ·                         ╰── WHERE clause
        //    ╰────
        //  Fix: Use a numeric literal for the comparison (e.g. 30)

    auto source = std::make_unique<Source>(
        "short.sql", "SELECT age FROM users WHERE age > 'thirty'");

    auto report = Report("Type mismatch in query")
        .withTitle("sql::type")
        .withLevel(Level::Error)
        .withFix("Use a numeric literal for the comparison (e.g. 30)")
        .withSource(std::move(source))
        .withLabel(Label(34, 8, "expected integer", true))
        .withLabel(Label(28, 3, "column 'age'", false))
        .withLabel(Label(22, 5, "WHERE clause", false));

    printReport(report);
}

static void example_13()
{
    std::cout << "\n Example 13: Longer function With multiple labels \n\n";

        // Error cpp::fn_issue
        //  Multiple issues in function
        //    ╭─[bigfn.cpp:4:9]
        //  1 │ int add(int a, int b) {
        //  2 │     int unused = 42;
        //    ·         ───┬──
        //    ·            ╰── unused variable
        //  3 │     if (a = b) {
        //    ·         ──┬──
        //    ·           ╰── assignment used as condition
        //  4 │         return a;
        //  5 │     }
        //  6 │     int sum = a + b;
        //    ·               ──┬──
        //    ·                 ╰── sum expression
        //  7 │     return sum;
        //  8 │ }
        //    ╰────
        //  Fix: Fix the assignment in the condition and remove unused variables

    auto source = std::make_unique<Source>(
        "bigfn.cpp", "int add(int a, int b) {\n    int unused = 42;\n    if (a = b) {\n        return a;\n    }\n    int sum = a + b;\n    return sum;\n}\n");

    auto report = Report("Multiple issues in function")
        .withTitle("cpp::fn_issue")
        .withLevel(Level::Error)
        .withFix("Fix the assignment in the condition and remove unused variables")
        .withSource(std::move(source))
        .withLabel(Label(53, 5, "assignment used as condition", true))
        .withLabel(Label(32, 6, "unused variable", false))
        .withLabel(Label(100, 5, "sum expression", false));

    printReport(report);
}

int main()
{
    example_1();
    example_2();
    example_3();
    example_4();
    example_5();
    example_6();
    example_7();
    example_8();
    example_9();
    example_10();
    example_11();
    example_12();
    example_13();

    return 0;
}