#ifndef SHIFTJIS_H
#define SHIFTJIS_H

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>

inline std::string ShiftJisToUtf8(const std::string& shift_jis_str) {
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

inline std::string Utf8ToShiftJis(const std::string& utf8_str) {
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_size);

    int sjis_size = WideCharToMultiByte(932, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string sjis_str(sjis_size, 0);
    WideCharToMultiByte(932, 0, wide_str.c_str(), -1, &sjis_str[0], sjis_size, nullptr, nullptr);

    return sjis_str;
}

#else // Apple (macOS, Linux)

#include <iconv.h>
#include <stdexcept>

inline std::string ConvertEncoding(const std::string& input, const char* from_enc, const char* to_enc) {
    iconv_t cd = iconv_open(to_enc, from_enc);
    if (cd == (iconv_t)-1) {
        throw std::runtime_error("iconv_open failed");
    }

    size_t in_size = input.size();
    size_t out_size = in_size * 4 + 4; // 余裕を持ったバッファサイズ
    std::string output(out_size, '\0');

    char* in_buf = const_cast<char*>(input.data());
    char* out_buf = &output[0];

    size_t in_bytes_left = in_size;
    size_t out_bytes_left = out_size;

    if (iconv(cd, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) == (size_t)-1) {
        iconv_close(cd);
        throw std::runtime_error("iconv conversion failed");
    }

    iconv_close(cd);

    output.resize(out_size - out_bytes_left); // 実際の長さに調整
    return output;
}

inline std::string ShiftJisToUtf8(const std::string& shift_jis_str) {
    return ConvertEncoding(shift_jis_str, "SHIFT_JIS", "UTF-8");
}

inline std::string Utf8ToShiftJis(const std::string& utf8_str) {
    return ConvertEncoding(utf8_str, "UTF-8", "SHIFT_JIS");
}

#endif // _WIN32

#endif // SHIFTJIS_H