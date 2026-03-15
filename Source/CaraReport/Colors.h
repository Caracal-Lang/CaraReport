#pragma once

#include <string>

namespace CaraReport::Colors
{
    [[nodiscard]] std::string rgb(int red, int green, int blue);

    [[nodiscard]] std::string reset();
    [[nodiscard]] std::string bold();
    [[nodiscard]] std::string dim();
    [[nodiscard]] std::string italic();
    [[nodiscard]] std::string underline();

    [[nodiscard]] std::string red();
    [[nodiscard]] std::string green();
    [[nodiscard]] std::string yellow();
    [[nodiscard]] std::string blue();
    [[nodiscard]] std::string magenta();
    [[nodiscard]] std::string cyan();
    [[nodiscard]] std::string white();

    [[nodiscard]] std::string brightRed();
    [[nodiscard]] std::string brightYellow();
    [[nodiscard]] std::string brightBlue();
    [[nodiscard]] std::string brightCyan();

    [[nodiscard]] std::string pastelRed();
    [[nodiscard]] std::string pastelOrange();
    [[nodiscard]] std::string pastelYellow();
    [[nodiscard]] std::string pastelGreen();
    [[nodiscard]] std::string pastelCyan();
    [[nodiscard]] std::string pastelBlue();
    [[nodiscard]] std::string pastelPurple();
    [[nodiscard]] std::string pastelPink();
    [[nodiscard]] std::string pastelGray();
}
