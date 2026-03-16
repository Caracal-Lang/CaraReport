#pragma once

#include <string>

namespace CaraReport
{
    enum class Level : std::uint8_t
    {
        Error,
        Warning,
        Info
    };

    [[nodiscard]] std::string stringify(Level level);
}
