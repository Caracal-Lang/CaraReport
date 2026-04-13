#include <CaraReport/C.h>

#include <stdio.h>
#include <stdlib.h>

static const char* PASTEL_PINK = "\x1b[38;2;244;143;177m";
static const char* PASTEL_ORANGE = "\x1b[38;2;255;183;77m";
static const char* PASTEL_PURPLE = "\x1b[38;2;179;157;219m";

static void print_and_free(char* text)
{
    if (text != NULL)
    {
        fputs(text, stdout);
        CRString_free(text);
    }
}

static int print_report(CRReport* report)
{
    char* rendered = CRReport_write(report);
    if (rendered == NULL)
    {
        return 0;
    }

    print_and_free(rendered);
    return 1;
}

static int print_writer_report(CRWriter* writer, CRReport* report)
{
    char* rendered;

    if (writer == NULL || report == NULL)
    {
        return 0;
    }

    rendered = CRWriter_writeReport(writer, report);
    if (rendered == NULL)
    {
        return 0;
    }

    print_and_free(rendered);
    return 1;
}

static int example_1(void)
{
    CRReport* report;

    puts(" Example 1: Simple parse error \n");

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

    report = CRReport_create("oops!");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "oops::my::bad");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "try doing it better next time?");
    CRReport_setSourceText(report, "bad_file.rs", "source\n  text\n    here");
    CRReport_addLabel(report, 9, 4, "This bit here", 1);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_2(void)
{
    CRReport* report;
    CRWriter* writer;
    char* rendered;

    puts("\n Example 2: Ascii theme \n");

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

    report = CRReport_create("oops!");
    writer = CRWriter_create();
    if (report == NULL || writer == NULL)
    {
        CRReport_destroy(report);
        CRWriter_destroy(writer);
        return 0;
    }

    CRReport_setTitle(report, "oops::my::bad");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "try doing it better next time?");
    CRReport_setSourceText(report, "bad_file.rs", "source\n  text\n    here");
    CRReport_addLabel(report, 9, 4, "This bit here", 1);

    CRWriter_setUnicode(writer, 0);

    rendered = CRWriter_writeReport(writer, report);
    if (rendered == NULL)
    {
        CRReport_destroy(report);
        CRWriter_destroy(writer);
        return 0;
    }

    fputs(rendered, stderr);
    CRString_free(rendered);
    CRReport_destroy(report);
    CRWriter_destroy(writer);
    return 1;
}

static int example_3(void)
{
    CRReport* report;

    puts("\n Example 3: Multiple labels \n");

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

    report = CRReport_create("Type 'string' is not assignable to type 'number'");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "ts(2322)");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "Change the type annotation or the assigned value");
    CRReport_setSourceText(report, "example.ts", "let x: number = \"hello\";\nconsole.log(x + 1);");
    CRReport_addLabel(report, 7, 6, "expected 'number'", 0);
    CRReport_addLabel(report, 16, 7, "got 'string'", 1);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_4(void)
{
    CRReport* report;

    puts("\n Example 4: Warning with URL \n");

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

    report = CRReport_create("os.popen() is deprecated since Python 3.0");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "W0602");
    CRReport_setLevel(report, CARAREPORT_LEVEL_WARNING);
    CRReport_setFix(report, "Use subprocess.run() or subprocess.Popen() instead");
    CRReport_setUrl(report, "https://docs.python.org/3/library/subprocess.html");
    CRReport_setSourceText(report, "app.py", "import os\nresult = os.popen('ls')\nprint(result.read())");
    CRReport_addLabel(report, 22, 5, "deprecated call", 1);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_5(void)
{
    CRReport* report;

    puts("\n Example 5: Info note\n");

    // Info note::feature_x
    //  Feature X is available
    //    ╭─[config.toml:2:1]
    //  1 │ enable_feature = false
    //    · ───────┬──────
    //    ·        ╰── toggle here
    //  2 │ # set to true to opt-in
    //    ╰────
    //  Fix: Consider enabling the feature in config.toml if desired

    report = CRReport_create("Feature X is available");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "note::feature_x");
    CRReport_setLevel(report, CARAREPORT_LEVEL_INFO);
    CRReport_setFix(report, "Consider enabling the feature in config.toml if desired");
    CRReport_setSourceText(report, "config.toml", "enable_feature = false\n# set to true to opt-in");
    CRReport_addLabel(report, 0, 14, "toggle here", 1);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_6(void)
{
    CRReport* report;

    puts("\n Example 6: Simple error\n");

    // Error math::precedence
    //  Wrong answer
    //    ╭─[math.txt:2:13]
    //  1 │ 2 + 2 * 2 = 8
    //    ·             ┬
    //    ·             ╰── this should be 6
    //    ╰────
    //  Fix: '*' has greater precedence than '+', so 2 + 2 * 2 = 6

    report = CRReport_create("Wrong answer");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "math::precedence");
    CRReport_setFix(report, "'*' has greater precedence than '+', so 2 + 2 * 2 = 6");
    CRReport_setSourceText(report, "math.txt", "2 + 2 * 2 = 8");
    CRReport_addLabel(report, 12, 1, "this should be 6", 0);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_7(void)
{
    CRReport* related;
    CRReport* report;

    puts("\n Example 7: Related reports \n");

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

    related = CRReport_create("This is a related note");
    report = CRReport_create("Top-level error referencing a note");
    if (related == NULL || report == NULL)
    {
        CRReport_destroy(report);
        CRReport_destroy(related);
        return 0;
    }

    CRReport_setLevel(related, CARAREPORT_LEVEL_INFO);
    CRReport_setTitle(related, "note::related");
    CRReport_setFix(related, "No action needed");
    CRReport_setSourceText(related, "note.txt", "This helper explains why this API is deprecated:\nUse new_api() instead.\n");
    CRReport_addLabel(related, 49, 22, "explanation", 0);

    CRReport_setTitle(report, "root::err");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "Replace call_old -> call_new or update usage");
    CRReport_setSourceText(report, "lib.rs", "fn call_old() {\n    // deprecated usage\n    old_api();\n}\n");
    CRReport_addLabel(report, 44, 7, "deprecated call", 0);
    CRReport_addRelated(report, related);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        CRReport_destroy(related);
        return 0;
    }

    CRReport_destroy(report);
    CRReport_destroy(related);
    return 1;
}

static int example_8(void)
{
    CRReport* report;
    CRTheme* theme;
    CRWriter* writer;

    puts("\n Example 8: Custom theme and colors \n");

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

    report = CRReport_create("Custom themed info");
    theme = CRTheme_ascii();
    writer = CRWriter_create();
    if (report == NULL || theme == NULL || writer == NULL)
    {
        CRReport_destroy(report);
        CRTheme_destroy(theme);
        CRWriter_destroy(writer);
        return 0;
    }

    CRReport_setTitle(report, "theme::info");
    CRReport_setLevel(report, CARAREPORT_LEVEL_INFO);
    CRReport_setSourceText(report, "theme.conf", "[settings]\nenable_feature = true\nlog_level = info\n");
    CRReport_addLabel(report, 11, 14, "setting enabled", 0);

    CRTheme_setLevelColor(theme, CARAREPORT_LEVEL_INFO, PASTEL_PINK);
    CRTheme_setFrameColor(theme, PASTEL_ORANGE);
    CRTheme_setLabelPaletteColor(theme, 0, PASTEL_PURPLE);

    CRWriter_setTheme(writer, theme);
    CRWriter_setColors(writer, 1);
    CRWriter_setUnicode(writer, 0);

    if (!print_writer_report(writer, report))
    {
        CRReport_destroy(report);
        CRTheme_destroy(theme);
        CRWriter_destroy(writer);
        return 0;
    }

    CRReport_destroy(report);
    CRTheme_destroy(theme);
    CRWriter_destroy(writer);
    return 1;
}

static int example_9(void)
{
    CRReport* report;
    CRWriter* writer;

    puts("\n Example 9: Tab width and context lines \n");

    //  Tab-handling example
    //    ╭─[tabs.txt:2:2]
    //  1 │         fn main() {
    //    ·         ─┬
    //    ·          ╰── call site
    //  2 │                 println!("hi");
    //  3 │ }
    //    ╰────

    report = CRReport_create("Tab-handling example");
    writer = CRWriter_create();
    if (report == NULL || writer == NULL)
    {
        CRReport_destroy(report);
        CRWriter_destroy(writer);
        return 0;
    }

    CRReport_setLevel(report, CARAREPORT_LEVEL_WARNING);
    CRReport_setSourceText(report, "tabs.txt", "\tfn main() {\n\t\tprintln!(\"hi\");\n}\n");
    CRReport_addLabel(report, 1, 2, "call site", 0);

    CRWriter_setTabWidth(writer, 8);
    CRWriter_setContextLines(writer, 2);

    if (!print_writer_report(writer, report))
    {
        CRReport_destroy(report);
        CRWriter_destroy(writer);
        return 0;
    }

    CRReport_destroy(report);
    CRWriter_destroy(writer);
    return 1;
}

static int example_10(void)
{
    CRReport* report;

    puts("\n Example 10: Multiline span \n");

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

    report = CRReport_create("mismatched types");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "E0308");
    CRReport_setFix(report, "Consider converting the vec to the expected type");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setSourceText(report, "script.rs", "fn main() {\n    let x = vec![\n        1, 2, 3,\n        4, 5, 6,\n    ];\n}\n");
    CRReport_addLabel(report, 24, 46, "expected Vec<String>, found Vec<i32>", 1);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_11(void)
{
    CRReport* report;

    puts("\n Example 11: Three labels (long) \n");

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

    report = CRReport_create("3 type errors found in query");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "sql::type_check");
    CRReport_setFix(report, "Use integer literals for numeric columns, strings for text columns, and a list for IN clauses");
    CRReport_setSourceText(report, "query.sql", "SELECT name, age, email FROM users WHERE age > 'thirty' AND active = 7.5 AND role IN 42");
    CRReport_addLabel(report, 47, 8, "expected integer, got string", 1);
    CRReport_addLabel(report, 69, 3, "expected boolean, got float", 0);
    CRReport_addLabel(report, 85, 2, "expected list, got integer", 0);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_12(void)
{
    CRReport* report;

    puts("\n Example 12: Three labels (short) \n");

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

    report = CRReport_create("Type mismatch in query");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "sql::type");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "Use a numeric literal for the comparison (e.g. 30)");
    CRReport_setSourceText(report, "short.sql", "SELECT age FROM users WHERE age > 'thirty'");
    CRReport_addLabel(report, 34, 8, "expected integer", 1);
    CRReport_addLabel(report, 28, 3, "column 'age'", 0);
    CRReport_addLabel(report, 22, 5, "WHERE clause", 0);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

static int example_13(void)
{
    CRReport* report;

    puts("\n Example 13: Longer function With multiple labels \n");

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

    report = CRReport_create("Multiple issues in function");
    if (report == NULL)
    {
        return 0;
    }

    CRReport_setTitle(report, "cpp::fn_issue");
    CRReport_setLevel(report, CARAREPORT_LEVEL_ERROR);
    CRReport_setFix(report, "Fix the assignment in the condition and remove unused variables");
    CRReport_setSourceText(report, "bigfn.cpp", "int add(int a, int b) {\n    int unused = 42;\n    if (a = b) {\n        return a;\n    }\n    int sum = a + b;\n    return sum;\n}\n");
    CRReport_addLabel(report, 53, 5, "assignment used as condition", 1);
    CRReport_addLabel(report, 32, 6, "unused variable", 0);
    CRReport_addLabel(report, 100, 5, "sum expression", 0);

    if (!print_report(report))
    {
        CRReport_destroy(report);
        return 0;
    }

    CRReport_destroy(report);
    return 1;
}

int main(void)
{
    if (!example_1()) return EXIT_FAILURE;
    if (!example_2()) return EXIT_FAILURE;
    if (!example_3()) return EXIT_FAILURE;
    if (!example_4()) return EXIT_FAILURE;
    if (!example_5()) return EXIT_FAILURE;
    if (!example_6()) return EXIT_FAILURE;
    if (!example_7()) return EXIT_FAILURE;
    if (!example_8()) return EXIT_FAILURE;
    if (!example_9()) return EXIT_FAILURE;
    if (!example_10()) return EXIT_FAILURE;
    if (!example_11()) return EXIT_FAILURE;
    if (!example_12()) return EXIT_FAILURE;
    if (!example_13()) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}