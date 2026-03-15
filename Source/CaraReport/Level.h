#pragma once

#include <string>

namespace CaraReport
{
    enum class Level
    {
        Error,
        Warning,
        Info
    };

    [[nodiscard]] std::string stringify(Level level);
}
