using System.Text;

namespace CaraReport;

internal static class Program
{
    private const string PastelPink = "\x1b[38;2;244;143;177m";
    private const string PastelOrange = "\x1b[38;2;255;183;77m";
    private const string PastelPurple = "\x1b[38;2;179;157;219m";

    private static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8;

        Example1();
        Example2();
        Example3();
        Example4();
        Example5();
        Example6();
        Example7();
        Example8();
        Example9();
        Example10();
        Example11();
        Example12();
        Example13();
    }

    private static void Example1()
    {
        Console.WriteLine(" Example 1: Simple parse error \n");

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

        using var report = new Report("oops!")
            .SetTitle("oops::my::bad")
            .SetFix("try doing it better next time?")
            .SetSourceText("bad_file.rs", "source\n  text\n    here")
            .AddLabel(9, 4, "This bit here", primary: true);

        Console.WriteLine(report.Write());
    }

    private static void Example2()
    {
        Console.WriteLine("\n Example 2: Ascii theme \n");

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

        using var report = new Report("oops!")
            .SetTitle("oops::my::bad")
            .SetFix("try doing it better next time?")
            .SetSourceText("bad_file.rs", "source\n  text\n    here")
            .AddLabel(9, 4, "This bit here", primary: true);

        using var writer = new Writer()
            .SetUnicode(false);

        Console.Error.Write(writer.WriteReport(report));
    }

    private static void Example3()
    {
        Console.WriteLine("\n Example 3: Multiple labels \n");

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

        using var report = new Report("Type 'string' is not assignable to type 'number'")
            .SetTitle("ts(2322)")
            .SetFix("Change the type annotation or the assigned value")
            .SetSourceText("example.ts", "let x: number = \"hello\";\nconsole.log(x + 1);")
            .AddLabel(7, 6, "expected 'number'")
            .AddLabel(16, 7, "got 'string'", primary: true);

        Console.WriteLine(report.Write());
    }

    private static void Example4()
    {
        Console.WriteLine("\n Example 4: Warning with URL \n");

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

        using var report = new Report("os.popen() is deprecated since Python 3.0")
            .SetTitle("W0602")
            .SetLevel(Level.Warning)
            .SetFix("Use subprocess.run() or subprocess.Popen() instead")
            .SetUrl("https://docs.python.org/3/library/subprocess.html")
            .SetSourceText("app.py", "import os\nresult = os.popen('ls')\nprint(result.read())")
            .AddLabel(22, 5, "deprecated call", primary: true);

        Console.WriteLine(report.Write());
    }

    private static void Example5()
    {
        Console.WriteLine("\n Example 5: Info note\n");

        // Info note::feature_x
        //  Feature X is available
        //    ╭─[config.toml:2:1]
        //  1 │ enable_feature = false
        //    · ───────┬──────
        //    ·        ╰── toggle here
        //  2 │ # set to true to opt-in
        //    ╰────
        //  Fix: Consider enabling the feature in config.toml if desired

        using var report = new Report("Feature X is available")
            .SetTitle("note::feature_x")
            .SetLevel(Level.Info)
            .SetFix("Consider enabling the feature in config.toml if desired")
            .SetSourceText("config.toml", "enable_feature = false\n# set to true to opt-in")
            .AddLabel(0, 14, "toggle here", primary: true);

        Console.WriteLine(report.Write());
    }

    private static void Example6()
    {
        Console.WriteLine("\n Example 6: Simple error\n");

        // Error math::precedence
        //  Wrong answer
        //    ╭─[math.txt:2:13]
        //  1 │ 2 + 2 * 2 = 8
        //    ·             ┬
        //    ·             ╰── this should be 6
        //    ╰────
        //  Fix: '*' has greater precedence than '+', so 2 + 2 * 2 = 6

        using var report = new Report("Wrong answer")
            .SetTitle("math::precedence")
            .SetFix("'*' has greater precedence than '+', so 2 + 2 * 2 = 6")
            .SetSourceText("math.txt", "2 + 2 * 2 = 8")
            .AddLabel(12, 1, "this should be 6");

        Console.WriteLine(report.Write());
    }

    private static void Example7()
    {
        Console.WriteLine("\n Example 7: Related reports \n");

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

        using var relatedReport = new Report("This is a related note")
            .SetLevel(Level.Info)
            .SetTitle("note::related")
            .SetFix("No action needed")
            .SetSourceText("note.txt", "This helper explains why this API is deprecated:\nUse new_api() instead.\n")
            .AddLabel(49, 22, "explanation");

        using var report = new Report("Top-level error referencing a note")
            .SetTitle("root::err")
            .SetLevel(Level.Error)
            .SetFix("Replace call_old -> call_new or update usage")
            .SetSourceText("lib.rs", "fn call_old() {\n    // deprecated usage\n    old_api();\n}\n")
            .AddLabel(44, 7, "deprecated call")
            .AddRelated(relatedReport);

        Console.WriteLine(report.Write());
    }

    private static void Example8()
    {
        Console.WriteLine("\n Example 8: Custom theme and colors \n");

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

        using var report = new Report("Custom themed info")
            .SetTitle("theme::info")
            .SetLevel(Level.Info)
            .SetSourceText("theme.conf", "[settings]\nenable_feature = true\nlog_level = info\n")
            .AddLabel(11, 14, "setting enabled");

        using var theme = Theme.Ascii()
            .SetLevelColor(Level.Info, PastelPink)
            .SetFrameColor(PastelOrange)
            .SetLabelPaletteColor(0, PastelPurple);

        using var writer = new Writer()
            .SetTheme(theme)
            .SetColors(true)
            .SetUnicode(false);

        Console.Write(writer.WriteReport(report));
    }

    private static void Example9()
    {
        Console.WriteLine("\n Example 9: Tab width and context lines \n");

        //  Tab-handling example
        //    ╭─[tabs.txt:2:2]
        //  1 │         fn main() {
        //    ·         ─┬
        //    ·          ╰── call site
        //  2 │                 println!("hi");
        //  3 │ }
        //    ╰────

        using var report = new Report("Tab-handling example")
            .SetLevel(Level.Warning)
            .SetSourceText("tabs.txt", "\tfn main() {\n\t\tprintln!(\"hi\");\n}\n")
            .AddLabel(1, 2, "call site");

        using var writer = new Writer()
            .SetTabWidth(8)
            .SetContextLines(2);

        Console.Write(writer.WriteReport(report));
    }

    private static void Example10()
    {
        Console.WriteLine("\n Example 10: Multiline span \n");

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

        using var report = new Report("mismatched types")
            .SetTitle("E0308")
            .SetFix("Consider converting the vec to the expected type")
            .SetSourceText("script.rs", "fn main() {\n    let x = vec![\n        1, 2, 3,\n        4, 5, 6,\n    ];\n}\n")
            .AddLabel(24, 46, "expected Vec<String>, found Vec<i32>", primary: true);

        Console.WriteLine(report.Write());
    }

    private static void Example11()
    {
        Console.WriteLine("\n Example 11: Three labels (long) \n");

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

        using var report = new Report("3 type errors found in query")
            .SetTitle("sql::type_check")
            .SetFix("Use integer literals for numeric columns, strings for text columns, and a list for IN clauses")
            .SetSourceText("query.sql", "SELECT name, age, email FROM users WHERE age > 'thirty' AND active = 7.5 AND role IN 42")
            .AddLabel(47, 8, "expected integer, got string", primary: true)
            .AddLabel(69, 3, "expected boolean, got float")
            .AddLabel(85, 2, "expected list, got integer");

        Console.WriteLine(report.Write());
    }

    private static void Example12()
    {
        Console.WriteLine("\n Example 12: Three labels (short) \n");

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

        using var report = new Report("Type mismatch in query")
            .SetTitle("sql::type")
            .SetLevel(Level.Error)
            .SetFix("Use a numeric literal for the comparison (e.g. 30)")
            .SetSourceText("short.sql", "SELECT age FROM users WHERE age > 'thirty'")
            .AddLabel(34, 8, "expected integer", primary: true)
            .AddLabel(28, 3, "column 'age'")
            .AddLabel(22, 5, "WHERE clause");

        Console.WriteLine(report.Write());
    }

    private static void Example13()
    {
        Console.WriteLine("\n Example 13: Longer function With multiple labels \n");

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

        using var report = new Report("Multiple issues in function")
            .SetTitle("cpp::fn_issue")
            .SetLevel(Level.Error)
            .SetFix("Fix the assignment in the condition and remove unused variables")
            .SetSourceText("bigfn.cpp", "int add(int a, int b) {\n    int unused = 42;\n    if (a = b) {\n        return a;\n    }\n    int sum = a + b;\n    return sum;\n}\n")
            .AddLabel(53, 5, "assignment used as condition", primary: true)
            .AddLabel(32, 6, "unused variable")
            .AddLabel(100, 5, "sum expression");

        Console.WriteLine(report.Write());
    }
}
