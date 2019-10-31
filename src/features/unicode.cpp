#include "unicode.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include <windows.h>

static wchar_t temp_wide_str[8092];
static char temp_utf8_str[8092];
BOOL WINAPI utf8_text_out(HDC hdc, int x, int y, char* string, int c) {
  auto len = MultiByteToWideChar(CP_UTF8, 0, string, c, temp_wide_str, sizeof(temp_wide_str));
  TextOutW(hdc, x, y, temp_wide_str, len);
  return 1;
}

BOOL WINAPI utf8_draw_text(HDC hdc, char* string, int c, RECT* rect, UINT format) {
  auto len = MultiByteToWideChar(CP_UTF8, 0, string, c, temp_wide_str, sizeof(temp_wide_str));
  DrawTextW(hdc, temp_wide_str, len, rect, format);
  return 1;
}

static wchar_t* temp_str_ptr = nullptr;
int WINAPI utf8_load_string(HINSTANCE hInstance, UINT uID, char* str_out, int cchBufferMax) {
  auto len = LoadStringW(hInstance, uID, reinterpret_cast<wchar_t*>(&temp_str_ptr), 0);

  auto utf8_len = WideCharToMultiByte(CP_UTF8, 0, temp_str_ptr, len, temp_utf8_str, sizeof(temp_utf8_str), 0, 0);
  if (cchBufferMax > 0) {
    strncpy(str_out, temp_utf8_str, std::min(utf8_len, cchBufferMax));
    if (cchBufferMax > utf8_len) str_out[utf8_len] = '\0';
  }

  return utf8_len;
}

BOOL WINAPI utf8_text_metrics(HDC hdc, LPTEXTMETRICA lptm) {
  TEXTMETRICW wide;
  auto result = GetTextMetricsW(hdc, &wide);

  lptm->tmHeight = wide.tmHeight;
  lptm->tmAscent = wide.tmAscent;
  lptm->tmDescent = wide.tmDescent;
  lptm->tmInternalLeading = wide.tmInternalLeading;
  lptm->tmExternalLeading = wide.tmExternalLeading;
  lptm->tmAveCharWidth = wide.tmAveCharWidth;
  lptm->tmMaxCharWidth = wide.tmMaxCharWidth;
  lptm->tmWeight = wide.tmWeight;
  lptm->tmOverhang = wide.tmOverhang;
  lptm->tmDigitizedAspectY = wide.tmDigitizedAspectY;
  lptm->tmDigitizedAspectX = wide.tmDigitizedAspectX;
  lptm->tmFirstChar = static_cast<char>(wide.tmFirstChar);
  lptm->tmLastChar = static_cast<char>(wide.tmLastChar);
  lptm->tmDefaultChar = static_cast<char>(wide.tmDefaultChar);
  lptm->tmBreakChar = static_cast<char>(wide.tmBreakChar);
  lptm->tmItalic = wide.tmItalic;
  lptm->tmUnderlined = wide.tmUnderlined;
  lptm->tmStruckOut = wide.tmStruckOut;
  lptm->tmPitchAndFamily = wide.tmPitchAndFamily;
  lptm->tmCharSet = wide.tmCharSet;

  return result;
}

BOOL WINAPI utf8_text_extent(HDC hdc, char* string, int c, LPSIZE psizl) {
  auto len = MultiByteToWideChar(CP_UTF8, 0, string, c, temp_wide_str, sizeof(temp_wide_str));
  return GetTextExtentPoint32W(hdc, temp_wide_str, len, psizl);
}

static BytesHook text_out_hook_;
static BytesHook draw_text_hook_;
static BytesHook load_string_hook_;
static BytesHook text_metrics_hook_;
static BytesHook text_extent_hook_;

static void* ptr_text_out_a;
static void* ptr_draw_text_a;
static void* ptr_load_string_a;
static void* ptr_text_metrics_a;
static void* ptr_text_extent_a;

void Unicode::install() {
  ptr_text_out_a = (void*)utf8_text_out;
  ptr_load_string_a = (void*)utf8_load_string;
  ptr_text_metrics_a = (void*)utf8_text_metrics;
  ptr_text_extent_a = (void*)utf8_text_extent;

  text_out_hook_.install((void*)0x635040, &ptr_text_out_a, sizeof(ptr_text_out_a));
  draw_text_hook_.install((void*)0x6352EC, &ptr_draw_text_a, sizeof(ptr_draw_text_a));
  load_string_hook_.install((void*)0x635398, &ptr_load_string_a, sizeof(ptr_load_string_a));
  text_metrics_hook_.install((void*)0x635070, &ptr_text_metrics_a, sizeof(ptr_text_metrics_a));
  text_extent_hook_.install((void*)0x635078, &ptr_text_extent_a, sizeof(ptr_text_extent_a));
}
