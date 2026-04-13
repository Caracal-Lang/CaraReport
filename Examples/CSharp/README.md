
The example shows how a C# application could use the CaraReport C API to generate a report.
The wrapper checks for the shared lib under ``CARAREPORT_PATH`` and falls back to the default search paths if not found:
- ``CaraReport.dll`` or ``carareport.dll`` on Windows
- ``libCaraReport.dylib`` or ``libcarareport.dylib`` on macOS
- ``libCaraReport.so`` or ``libcarareport.so`` on Linux

The CaraReport.cs file can be used as a drop-in wrapper for the C API, since i'm not providing a Nuget package for it.
