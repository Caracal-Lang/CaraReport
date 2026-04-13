using System.Runtime.InteropServices;

namespace CaraReport;

public enum Level
{
    Error = 0,
    Warning = 1,
    Info = 2
}

public sealed class Report : IDisposable
{
    private nint _handle;

    public Report(string message)
    {
        _handle = CaraReportNative.ReportCreate(message);
        if (_handle == nint.Zero)
        {
            throw new InvalidOperationException("Failed to create report.");
        }
    }

    public nint Handle
    {
        get
        {
            EnsureNotDisposed();
            return _handle;
        }
    }

    public Report SetLevel(Level level)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportSetLevel(_handle, (CaraReportNative.CRLevel)level);
        return this;
    }

    public Report SetTitle(string title)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportSetTitle(_handle, title);
        return this;
    }

    public Report SetUrl(string url)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportSetUrl(_handle, url);
        return this;
    }

    public Report SetFix(string fix)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportSetFix(_handle, fix);
        return this;
    }

    public Report SetSourceText(string name, string sourceText)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportSetSourceText(_handle, name, sourceText);
        return this;
    }

    public Report AddLabel(int start, int length, string label, bool primary = false)
    {
        EnsureNotDisposed();
        CaraReportNative.ReportAddLabel(_handle, start, length, label, primary);
        return this;
    }

    public Report AddRelated(Report related)
    {
        EnsureNotDisposed();
        ArgumentNullException.ThrowIfNull(related);
        CaraReportNative.ReportAddRelated(_handle, related.Handle);
        return this;
    }

    public string Write()
    {
        EnsureNotDisposed();
        return CaraReportNative.ReportWrite(_handle);
    }

    public void Dispose()
    {
        if (_handle != nint.Zero)
        {
            CaraReportNative.ReportDestroy(_handle);
            _handle = nint.Zero;
        }
    }

    private void EnsureNotDisposed()
    {
        ObjectDisposedException.ThrowIf(_handle == nint.Zero, this);
    }
}

public sealed class Theme : IDisposable
{
    private nint _handle;

    private Theme(nint handle)
    {
        this._handle = handle;
        if (handle == nint.Zero)
        {
            throw new InvalidOperationException("Failed to create theme.");
        }
    }

    public nint Handle
    {
        get
        {
            EnsureNotDisposed();
            return _handle;
        }
    }

    public static Theme Create()
    {
        return new Theme(CaraReportNative.ThemeCreate());
    }

    public static Theme Ascii()
    {
        return new Theme(CaraReportNative.ThemeAscii());
    }

    public Theme SetLevelColor(Level level, string color)
    {
        EnsureNotDisposed();
        CaraReportNative.ThemeSetLevelColor(_handle, (CaraReportNative.CRLevel)level, color);
        return this;
    }

    public Theme SetFrameColor(string color)
    {
        EnsureNotDisposed();
        CaraReportNative.ThemeSetFrameColor(_handle, color);
        return this;
    }

    public Theme SetLabelPaletteColor(int index, string color)
    {
        EnsureNotDisposed();
        CaraReportNative.ThemeSetLabelPaletteColor(_handle, index, color);
        return this;
    }

    public void Dispose()
    {
        if (_handle != nint.Zero)
        {
            CaraReportNative.ThemeDestroy(_handle);
            _handle = nint.Zero;
        }
    }

    private void EnsureNotDisposed()
    {
        ObjectDisposedException.ThrowIf(_handle == nint.Zero, this);
    }
}

public sealed class Writer : IDisposable
{
    private nint _handle;

    public Writer()
    {
        _handle = CaraReportNative.WriterCreate();
        if (_handle == nint.Zero)
        {
            throw new InvalidOperationException("Failed to create writer.");
        }
    }

    public Writer SetTheme(Theme theme)
    {
        EnsureNotDisposed();
        ArgumentNullException.ThrowIfNull(theme);
        CaraReportNative.WriterSetTheme(_handle, theme.Handle);
        return this;
    }

    public Writer SetContextLines(int contextLines)
    {
        EnsureNotDisposed();
        CaraReportNative.WriterSetContextLines(_handle, contextLines);
        return this;
    }

    public Writer SetTabWidth(int tabWidth)
    {
        EnsureNotDisposed();
        CaraReportNative.WriterSetTabWidth(_handle, tabWidth);
        return this;
    }

    public Writer SetUnicode(bool enable)
    {
        EnsureNotDisposed();
        CaraReportNative.WriterSetUnicode(_handle, enable);
        return this;
    }

    public Writer SetColors(bool enable)
    {
        EnsureNotDisposed();
        CaraReportNative.WriterSetColors(_handle, enable);
        return this;
    }

    public string WriteReport(Report report)
    {
        EnsureNotDisposed();
        ArgumentNullException.ThrowIfNull(report);
        return CaraReportNative.WriterWriteReport(_handle, report.Handle);
    }

    public void Dispose()
    {
        if (_handle != nint.Zero)
        {
            CaraReportNative.WriterDestroy(_handle);
            _handle = nint.Zero;
        }
    }

    private void EnsureNotDisposed()
    {
        ObjectDisposedException.ThrowIf(_handle == nint.Zero, this);
    }
}

internal static class CaraReportNative
{
    private static readonly nint _libraryHandle = LoadLibrary();

    private static readonly CRReportCreateDelegate _reportCreateExport = LoadExport<CRReportCreateDelegate>("CRReport_create");
    private static readonly CRReportDestroyDelegate _reportDestroyExport = LoadExport<CRReportDestroyDelegate>("CRReport_destroy");
    private static readonly CRReportSetLevelDelegate _reportSetLevelExport = LoadExport<CRReportSetLevelDelegate>("CRReport_setLevel");
    private static readonly CRReportSetTitleDelegate _reportSetTitleExport = LoadExport<CRReportSetTitleDelegate>("CRReport_setTitle");
    private static readonly CRReportSetUrlDelegate _reportSetUrlExport = LoadExport<CRReportSetUrlDelegate>("CRReport_setUrl");
    private static readonly CRReportSetFixDelegate _reportSetFixExport = LoadExport<CRReportSetFixDelegate>("CRReport_setFix");
    private static readonly CRReportSetSourceTextDelegate _reportSetSourceTextExport = LoadExport<CRReportSetSourceTextDelegate>("CRReport_setSourceText");
    private static readonly CRReportAddRelatedDelegate _reportAddRelatedExport = LoadExport<CRReportAddRelatedDelegate>("CRReport_addRelated");
    private static readonly CRReportAddLabelDelegate _reportAddLabelExport = LoadExport<CRReportAddLabelDelegate>("CRReport_addLabel");
    private static readonly CRReportWriteDelegate _reportWriteExport = LoadExport<CRReportWriteDelegate>("CRReport_write");
    
    private static readonly CRThemeCreateDelegate _themeCreateExport = LoadExport<CRThemeCreateDelegate>("CRTheme_create");
    private static readonly CRThemeAsciiDelegate _themeAsciiExport = LoadExport<CRThemeAsciiDelegate>("CRTheme_ascii");
    private static readonly CRThemeDestroyDelegate _themeDestroyExport = LoadExport<CRThemeDestroyDelegate>("CRTheme_destroy");
    private static readonly CRThemeSetLevelColorDelegate _themeSetLevelColorExport = LoadExport<CRThemeSetLevelColorDelegate>("CRTheme_setLevelColor");
    private static readonly CRThemeSetFrameColorDelegate _themeSetFrameColorExport = LoadExport<CRThemeSetFrameColorDelegate>("CRTheme_setFrameColor");
    private static readonly CRThemeSetLabelPaletteColorDelegate _themeSetLabelPaletteColorExport = LoadExport<CRThemeSetLabelPaletteColorDelegate>("CRTheme_setLabelPaletteColor");
    
    private static readonly CRWriterCreateDelegate _writerCreateExport = LoadExport<CRWriterCreateDelegate>("CRWriter_create");
    private static readonly CRWriterDestroyDelegate _writerDestroyExport = LoadExport<CRWriterDestroyDelegate>("CRWriter_destroy");
    private static readonly CRWriterSetThemeDelegate _writerSetThemeExport = LoadExport<CRWriterSetThemeDelegate>("CRWriter_setTheme");
    private static readonly CRWriterSetContextLinesDelegate _writerSetContextLinesExport = LoadExport<CRWriterSetContextLinesDelegate>("CRWriter_setContextLines");
    private static readonly CRWriterSetTabWidthDelegate _writerSetTabWidthExport = LoadExport<CRWriterSetTabWidthDelegate>("CRWriter_setTabWidth");
    private static readonly CRWriterSetUnicodeDelegate _writerSetUnicodeExport = LoadExport<CRWriterSetUnicodeDelegate>("CRWriter_setUnicode");
    private static readonly CRWriterSetColorsDelegate _writerSetColorsExport = LoadExport<CRWriterSetColorsDelegate>("CRWriter_setColors");
    private static readonly CRWriterWriteReportDelegate _writerWriteReportExport = LoadExport<CRWriterWriteReportDelegate>("CRWriter_writeReport");
    
    private static readonly CRStringFreeDelegate _stringFreeExport = LoadExport<CRStringFreeDelegate>("CRString_free");

    internal static nint ReportCreate(string message)
    {
        using var messageUtf8 = Utf8String.FromString(message);
        return _reportCreateExport(messageUtf8.Pointer);
    }

    internal static void ReportDestroy(nint report)
    {
        _reportDestroyExport(report);
    }

    internal static void ReportSetLevel(nint report, CRLevel level)
    {
        _reportSetLevelExport(report, level);
    }

    internal static void ReportSetTitle(nint report, string title)
    {
        using var titleUtf8 = Utf8String.FromString(title);
        _reportSetTitleExport(report, titleUtf8.Pointer);
    }

    internal static void ReportSetUrl(nint report, string url)
    {
        using var urlUtf8 = Utf8String.FromString(url);
        _reportSetUrlExport(report, urlUtf8.Pointer);
    }

    internal static void ReportSetFix(nint report, string fix)
    {
        using var fixUtf8 = Utf8String.FromString(fix);
        _reportSetFixExport(report, fixUtf8.Pointer);
    }

    internal static void ReportSetSourceText(nint report, string name, string sourceText)
    {
        using var nameUtf8 = Utf8String.FromString(name);
        using var sourceTextUtf8 = Utf8String.FromString(sourceText);
        _reportSetSourceTextExport(report, nameUtf8.Pointer, sourceTextUtf8.Pointer);
    }

    internal static void ReportAddRelated(nint report, nint related)
    {
        _reportAddRelatedExport(report, related);
    }

    internal static void ReportAddLabel(nint report, int start, int length, string label, bool primary)
    {
        using var labelUtf8 = Utf8String.FromString(label);
        _reportAddLabelExport(report, start, length, labelUtf8.Pointer, primary ? 1 : 0);
    }

    internal static string ReportWrite(nint report)
    {
        var text = _reportWriteExport(report);
        return TakeOwnedUtf8String(text);
    }

    internal static nint ThemeCreate()
    {
        return _themeCreateExport();
    }

    internal static nint ThemeAscii()
    {
        return _themeAsciiExport();
    }

    internal static void ThemeDestroy(nint theme)
    {
        _themeDestroyExport(theme);
    }

    internal static void ThemeSetLevelColor(nint theme, CRLevel level, string color)
    {
        using var colorUtf8 = Utf8String.FromString(color);
        _themeSetLevelColorExport(theme, level, colorUtf8.Pointer);
    }

    internal static void ThemeSetFrameColor(nint theme, string color)
    {
        using var colorUtf8 = Utf8String.FromString(color);
        _themeSetFrameColorExport(theme, colorUtf8.Pointer);
    }

    internal static void ThemeSetLabelPaletteColor(nint theme, int index, string color)
    {
        using var colorUtf8 = Utf8String.FromString(color);
        _themeSetLabelPaletteColorExport(theme, index, colorUtf8.Pointer);
    }

    internal static nint WriterCreate()
    {
        return _writerCreateExport();
    }

    internal static void WriterDestroy(nint writer)
    {
        _writerDestroyExport(writer);
    }

    internal static void WriterSetTheme(nint writer, nint theme)
    {
        _writerSetThemeExport(writer, theme);
    }

    internal static void WriterSetContextLines(nint writer, int contextLines)
    {
        _writerSetContextLinesExport(writer, contextLines);
    }

    internal static void WriterSetTabWidth(nint writer, int tabWidth)
    {
        _writerSetTabWidthExport(writer, tabWidth);
    }

    internal static void WriterSetUnicode(nint writer, bool enable)
    {
        _writerSetUnicodeExport(writer, enable ? 1 : 0);
    }

    internal static void WriterSetColors(nint writer, bool enable)
    {
        _writerSetColorsExport(writer, enable ? 1 : 0);
    }

    internal static string WriterWriteReport(nint writer, nint report)
    {
        var text = _writerWriteReportExport(writer, report);
        return TakeOwnedUtf8String(text);
    }

    private static string TakeOwnedUtf8String(nint text)
    {
        if (text == nint.Zero)
        {
            return string.Empty;
        }

        try
        {
            return Marshal.PtrToStringUTF8(text) ?? string.Empty;
        }
        finally
        {
            _stringFreeExport(text);
        }
    }

    private static T LoadExport<T>(string exportName) where T : Delegate
    {
        var symbol = NativeLibrary.GetExport(_libraryHandle, exportName);
        return Marshal.GetDelegateForFunctionPointer<T>(symbol);
    }

    private static nint LoadLibrary()
    {
        var configuredPath = Environment.GetEnvironmentVariable("CARAREPORT_PATH");
        if (!string.IsNullOrWhiteSpace(configuredPath) && NativeLibrary.TryLoad(configuredPath, out var configuredHandle))
        {
            return configuredHandle;
        }

        foreach (var candidate in GetLibraryCandidates())
        {
            if (NativeLibrary.TryLoad(candidate, out var handle))
            {
                return handle;
            }
        }

        throw new DllNotFoundException("Unable to load the CaraReport native library. Set CARAREPORT_PATH or place the native library on the process search path.");
    }

    private static IEnumerable<string> GetLibraryCandidates()
    {
        string[] fileNames;
        if (OperatingSystem.IsWindows())
        {
            fileNames = ["CaraReport.dll", "carareport.dll"];
        }
        else if (OperatingSystem.IsMacOS())
        {
            fileNames = ["libCaraReport.dylib", "libcarareport.dylib"];
        }
        else
        {
            fileNames = ["libCaraReport.so", "libcarareport.so"];
        }

        foreach (var fileName in fileNames)
        {
            yield return fileName;
            yield return Path.Combine(AppContext.BaseDirectory, fileName);
        }
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRReportCreateDelegate(nint message);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportDestroyDelegate(nint report);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportSetLevelDelegate(nint report, CRLevel level);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportSetTitleDelegate(nint report, nint title);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportSetUrlDelegate(nint report, nint url);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportSetFixDelegate(nint report, nint fix);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportSetSourceTextDelegate(nint report, nint name, nint sourceText);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportAddRelatedDelegate(nint report, nint related);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRReportAddLabelDelegate(nint report, int start, int length, nint label, int primary);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRReportWriteDelegate(nint report);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRThemeCreateDelegate();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRThemeAsciiDelegate();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRThemeDestroyDelegate(nint theme);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRThemeSetLevelColorDelegate(nint theme, CRLevel level, nint color);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRThemeSetFrameColorDelegate(nint theme, nint color);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRThemeSetLabelPaletteColorDelegate(nint theme, int index, nint color);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRWriterCreateDelegate();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterDestroyDelegate(nint writer);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterSetThemeDelegate(nint writer, nint theme);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterSetContextLinesDelegate(nint writer, int contextLines);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterSetTabWidthDelegate(nint writer, int tabWidth);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterSetUnicodeDelegate(nint writer, int enable);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRWriterSetColorsDelegate(nint writer, int enable);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate nint CRWriterWriteReportDelegate(nint writer, nint report);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void CRStringFreeDelegate(nint text);

    internal enum CRLevel
    {
        Error = 0,
        Warning = 1,
        Info = 2
    }

    private sealed class Utf8String : IDisposable
    {
        private Utf8String(nint pointer)
        {
            Pointer = pointer;
        }

        internal nint Pointer { get; private set; }

        internal static Utf8String FromString(string value)
        {
            return new Utf8String(Marshal.StringToCoTaskMemUTF8(value));
        }

        public void Dispose()
        {
            if (Pointer != nint.Zero)
            {
                Marshal.FreeCoTaskMem(Pointer);
                Pointer = nint.Zero;
            }
        }
    }
}
