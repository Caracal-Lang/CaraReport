#pragma once

#include <CaraReport/API.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CRReport CRReport;
typedef struct CRLabel CRLabel;
typedef struct CRSource CRSource;
typedef struct CRSpanContents CRSpanContents;
typedef struct CRTheme CRTheme;
typedef struct CRWriter CRWriter;

typedef struct CRSpan
{
    int start;
    int length;
} CRSpan;

typedef enum CRLevel
{
    CARAREPORT_LEVEL_ERROR = 0,
    CARAREPORT_LEVEL_WARNING = 1,
    CARAREPORT_LEVEL_INFO = 2
} CRLevel;

CARAREPORT_API CRSpan CRSpan_create(int start, int length);
CARAREPORT_API CRSpan CRSpan_fromRange(int start, int end);
CARAREPORT_API int CRSpan_end(CRSpan span);
CARAREPORT_API int CRSpan_isEmpty(CRSpan span);
CARAREPORT_API int CRSpan_contains(CRSpan span, int position);
CARAREPORT_API int CRSpan_overlaps(CRSpan left, CRSpan right);

CARAREPORT_API CRLabel* CRLabel_create(int start, int length, const char* label, int primary);
CARAREPORT_API void CRLabel_destroy(CRLabel* label);
CARAREPORT_API CRSpan CRLabel_span(const CRLabel* label);
CARAREPORT_API const char* CRLabel_text(const CRLabel* label);
CARAREPORT_API int CRLabel_isPrimary(const CRLabel* label);
CARAREPORT_API int CRLabel_start(const CRLabel* label);
CARAREPORT_API int CRLabel_end(const CRLabel* label);
CARAREPORT_API int CRLabel_length(const CRLabel* label);

CARAREPORT_API CRReport* CRReport_create(const char* message);
CARAREPORT_API void CRReport_destroy(CRReport* report);
CARAREPORT_API void CRReport_setLevel(CRReport* report, CRLevel level);
CARAREPORT_API void CRReport_setTitle(CRReport* report, const char* title);
CARAREPORT_API void CRReport_setUrl(CRReport* report, const char* url);
CARAREPORT_API void CRReport_setFix(CRReport* report, const char* fix);
CARAREPORT_API void CRReport_setSource(CRReport* report, const CRSource* source);
CARAREPORT_API void CRReport_setSourceText(CRReport* report, const char* name, const char* sourceText);
CARAREPORT_API void CRReport_addLabelHandle(CRReport* report, const CRLabel* label);
CARAREPORT_API void CRReport_addRelated(CRReport* report, const CRReport* related);
CARAREPORT_API void CRReport_addLabel(CRReport* report, int start, int length, const char* label, int primary);
CARAREPORT_API CRLevel CRReport_level(const CRReport* report);
CARAREPORT_API const char* CRReport_title(const CRReport* report);
CARAREPORT_API const char* CRReport_url(const CRReport* report);
CARAREPORT_API const char* CRReport_message(const CRReport* report);
CARAREPORT_API const char* CRReport_fix(const CRReport* report);
CARAREPORT_API int CRReport_hasSource(const CRReport* report);
CARAREPORT_API int CRReport_labelCount(const CRReport* report);

CARAREPORT_API CRSource* CRSource_create(const char* name, const char* sourceText);
CARAREPORT_API void CRSource_destroy(CRSource* source);
CARAREPORT_API void CRSource_setName(CRSource* source, const char* name);
CARAREPORT_API void CRSource_setText(CRSource* source, const char* sourceText);
CARAREPORT_API void CRSource_readSpan(const CRSource* source, CRSpan span, int contextLinesBefore, int contextLinesAfter, CRSpanContents** contents);

CARAREPORT_API void CRSpanContents_destroy(CRSpanContents* contents);
CARAREPORT_API const char* CRSpanContents_name(const CRSpanContents* contents);
CARAREPORT_API int CRSpanContents_startLine(const CRSpanContents* contents);
CARAREPORT_API int CRSpanContents_startColumn(const CRSpanContents* contents);
CARAREPORT_API int CRSpanContents_lineCount(const CRSpanContents* contents);
CARAREPORT_API int CRSpanContents_lineNumber(const CRSpanContents* contents, int index);
CARAREPORT_API const char* CRSpanContents_lineText(const CRSpanContents* contents, int index);

CARAREPORT_API CRTheme* CRTheme_create(void);
CARAREPORT_API CRTheme* CRTheme_ascii(void);
CARAREPORT_API void CRTheme_destroy(CRTheme* theme);
CARAREPORT_API void CRTheme_setLevelColor(CRTheme* theme, CRLevel level, const char* color);
CARAREPORT_API void CRTheme_setFrameColor(CRTheme* theme, const char* color);
CARAREPORT_API void CRTheme_setLabelPaletteColor(CRTheme* theme, int index, const char* color);

CARAREPORT_API CRWriter* CRWriter_create(void);
CARAREPORT_API void CRWriter_destroy(CRWriter* writer);
CARAREPORT_API void CRWriter_setTheme(CRWriter* writer, const CRTheme* theme);
CARAREPORT_API void CRWriter_setContextLines(CRWriter* writer, int contextLines);
CARAREPORT_API void CRWriter_setTabWidth(CRWriter* writer, int tabWidth);
CARAREPORT_API void CRWriter_setUnicode(CRWriter* writer, int enable);
CARAREPORT_API void CRWriter_setColors(CRWriter* writer, int enable);
CARAREPORT_API char* CRWriter_writeReport(const CRWriter* writer, const CRReport* report);

CARAREPORT_API char* CRReport_write(const CRReport* report);
CARAREPORT_API void CRString_free(char* text);

#ifdef __cplusplus
}
#endif