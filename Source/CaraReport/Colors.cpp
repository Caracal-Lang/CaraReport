#include <CaraReport/Colors.h>

namespace CaraReport::Colors
{
    std::string rgb(int red, int green, int blue)
    {
        return "\033[38;2;" 
            + std::to_string(red) 
            + ";" 
            + std::to_string(green) 
            + ";" 
            + std::to_string(blue) 
            + "m";
    }

    std::string reset()
    {
        return "\033[0m";
    }

    std::string bold()
    {
        return "\033[1m";
    }

    std::string dim()
    {
        return "\033[2m";
    }

    std::string italic()
    {
        return "\033[3m";
    }

    std::string underline()
    {
        return "\033[4m";
    }

    std::string red()
    {
        return "\033[31m";
    }

    std::string green()
    {
        return "\033[32m";
    }

    std::string yellow()
    {
        return "\033[33m";
    }

    std::string blue()
    {
        return "\033[34m";
    }

    std::string magenta()
    {
        return "\033[35m";
    }

    std::string cyan()
    {
        return "\033[36m";
    }

    std::string white()
    {
        return "\033[37m";
    }

    std::string brightRed()
    {
        return "\033[91m";
    }

    std::string brightYellow()
    {
        return "\033[93m";
    }

    std::string brightBlue()
    {
        return "\033[94m";
    }

    std::string brightCyan()
    {
        return "\033[96m";
    }

    std::string pastelRed()
    {
        return rgb(244, 75, 86);
    }

    std::string pastelOrange()
    {
        return rgb(255, 150, 100);
    }

    std::string pastelYellow()
    {
        return rgb(255, 205, 120);
    }

    std::string pastelGreen()
    {
        return rgb(138, 226, 138);
    }

    std::string pastelCyan()
    {
        return rgb(120, 200, 230);
    }

    std::string pastelBlue()
    {
        return rgb(130, 150, 255);
    }

    std::string pastelPurple()
    {
        return rgb(190, 140, 255);
    }

    std::string pastelPink()
    {
        return rgb(255, 145, 195);
    }

    std::string pastelGray()
    {
        return rgb(200, 200, 210);
    }
}
