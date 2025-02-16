#ifndef SHIFTJIS_H
#define SHIFTJIS_H

#include <windows.h>
#include <iostream>
#include <string>

std::string ShiftJisToUtf8(const std::string& shift_jis_str) {
    if (shift_jis_str.empty()) return "";

    // MultiByteToWideCharの変換サイズ取得（NULL終端なし）
    int wide_size = MultiByteToWideChar(932, 0, shift_jis_str.data(), (int)shift_jis_str.size(), nullptr, 0);
    if (wide_size <= 0) return "";

    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(932, 0, shift_jis_str.data(), (int)shift_jis_str.size(), &wide_str[0], wide_size);

    // WideCharToMultiByteの変換サイズ取得（NULL終端なし）
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_str.data(), (int)wide_str.size(), nullptr, 0, nullptr, nullptr);
    if (utf8_size <= 0) return "";

    std::string utf8_str(utf8_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_str.data(), (int)wide_str.size(), &utf8_str[0], utf8_size, nullptr, nullptr);

    return utf8_str;
}

std::string Utf8ToShiftJis(const std::string& utf8_str) {
    // UTF-8 から wstring に変換
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_size);

    // wstring から Shift_JIS に変換
    int sjis_size = WideCharToMultiByte(932, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string sjis_str(sjis_size, 0);
    WideCharToMultiByte(932, 0, wide_str.c_str(), -1, &sjis_str[0], sjis_size, nullptr, nullptr);

    return sjis_str;
}

#endif SHIFTJIS_H