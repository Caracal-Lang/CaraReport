#pragma once

#include <CaraReport/API.h>
#include <string>

namespace CaraReport::Colors
{
    [[nodiscard]] CARAREPORT_API std::string rgb(int red, int green, int blue);

    [[nodiscard]] CARAREPORT_API std::string reset();
    [[nodiscard]] CARAREPORT_API std::string bold();
    [[nodiscard]] CARAREPORT_API std::string dim();
    [[nodiscard]] CARAREPORT_API std::string italic();
    [[nodiscard]] CARAREPORT_API std::string underline();

    [[nodiscard]] CARAREPORT_API std::string red();
    [[nodiscard]] CARAREPORT_API std::string green();
    [[nodiscard]] CARAREPORT_API std::string yellow();
    [[nodiscard]] CARAREPORT_API std::string blue();
    [[nodiscard]] CARAREPORT_API std::string magenta();
    [[nodiscard]] CARAREPORT_API std::string cyan();
    [[nodiscard]] CARAREPORT_API std::string white();

    [[nodiscard]] CARAREPORT_API std::string brightRed();
    [[nodiscard]] CARAREPORT_API std::string brightYellow();
    [[nodiscard]] CARAREPORT_API std::string brightBlue();
    [[nodiscard]] CARAREPORT_API std::string brightCyan();

    [[nodiscard]] CARAREPORT_API std::string pastelRed();
    [[nodiscard]] CARAREPORT_API std::string pastelOrange();
    [[nodiscard]] CARAREPORT_API std::string pastelYellow();
    [[nodiscard]] CARAREPORT_API std::string pastelGreen();
    [[nodiscard]] CARAREPORT_API std::string pastelCyan();
    [[nodiscard]] CARAREPORT_API std::string pastelBlue();
    [[nodiscard]] CARAREPORT_API std::string pastelPurple();
    [[nodiscard]] CARAREPORT_API std::string pastelPink();
    [[nodiscard]] CARAREPORT_API std::string pastelGray();
}
