#pragma once

#include <CaraReport/API.h>
#include <CaraReport/Level.h>
#include <CaraReport/Label.h>
#include <CaraReport/Colors.h>
#include <array>
#include <string>
#include <vector>
#include <memory>

namespace CaraReport
{
    class CARAREPORT_API Theme
    {
      public:
        Theme() = default;

        [[nodiscard]] std::string levelColor(Level level) const;
        [[nodiscard]] std::string titleColor() const;
        [[nodiscard]] std::vector<std::string>& labelPalette();
        [[nodiscard]] std::string frameColor() const;
        [[nodiscard]] std::string fixColor() const;
        [[nodiscard]] std::string reset() const;

        [[nodiscard]] std::string horizontalBar() const;
        [[nodiscard]] std::string verticalBar() const;
        [[nodiscard]] std::string intersection() const;
        [[nodiscard]] std::string verticalBreak() const;
        [[nodiscard]] std::string upArrow() const;
        [[nodiscard]] std::string rightArrow() const;
        [[nodiscard]] std::string cornerTopLeft() const;
        [[nodiscard]] std::string cornerTop() const;
        [[nodiscard]] std::string cornerTopRight() const;
        [[nodiscard]] std::string cornerBottomLeft() const;
        [[nodiscard]] std::string cornerBottomRight() const;
        [[nodiscard]] std::string cornerBottom() const;
        [[nodiscard]] std::string boxLeft() const;
        [[nodiscard]] std::string boxRight() const;
        [[nodiscard]] std::string crossLeft() const;
        [[nodiscard]] std::string crossRight() const;
        [[nodiscard]] std::string underConnector() const;
        [[nodiscard]] std::string underline() const;
        [[nodiscard]] std::string pointerRight() const;
        [[nodiscard]] bool isUnicode() const;

        void setLevelColor(Level level, const std::string& color);
        void setTitleColor(const std::string& color);
        void setLabelPalette(const std::vector<std::string>& palette);
        void setFrameColor(const std::string& color);
        void setFixColor(const std::string& color);
        void setReset(const std::string& resetCode);

        void setHorizontalBar(const std::string& glyph);
        void setVerticalBar(const std::string& glyph);
        void setIntersection(const std::string& glyph);
        void setVerticalBreak(const std::string& glyph);
        void setUpArrow(const std::string& glyph);
        void setRightArrow(const std::string& glyph);
        void setCornerTopLeft(const std::string& glyph);
        void setCornerTop(const std::string& glyph);
        void setCornerTopRight(const std::string& glyph);
        void setCornerBottomLeft(const std::string& glyph);
        void setCornerBottomRight(const std::string& glyph);
        void setCornerBottom(const std::string& glyph);
        void setBoxLeft(const std::string& glyph);
        void setBoxRight(const std::string& glyph);
        void setCrossLeft(const std::string& glyph);
        void setCrossRight(const std::string& glyph);
        void setUnderConnector(const std::string& glyph);
        void setUnderline(const std::string& glyph);
        void setPointerRight(const std::string& glyph);
        void setUnicode(bool enable);

        static std::unique_ptr<Theme> ascii();

      private:
        bool m_unicode = true;

        std::array<std::string, 3> m_levelColors = {
            Colors::pastelRed(),    // Error
            Colors::pastelYellow(), // Warning
            Colors::pastelCyan()    // Info
        };

        std::vector<std::string> m_labelPaletteVec = {
            Colors::pastelRed(),    Colors::pastelBlue(),
            Colors::pastelGreen(),  Colors::pastelPurple(),
            Colors::pastelOrange(), Colors::pastelPink(),
            Colors::pastelCyan(),
        };

        std::string m_titleColor = Colors::pastelYellow();
        std::string m_frameColor = Colors::pastelBlue();
        std::string m_fixColor = Colors::pastelCyan();
        std::string m_reset = Colors::reset();

        std::string m_horizontalBar = "\xe2\x94\x80"; // ─
        std::string m_verticalBar = "\xe2\x94\x82";   // │
        std::string m_intersection = "\xe2\x94\xbc";  // ┼
        std::string m_verticalBreak = "\xc2\xb7";     // ·

        std::string m_upArrow = "^";    // ^
        std::string m_rightArrow = ">"; // >

        std::string m_cornerTopLeft = "\xe2\x95\xad";     // ╭
        std::string m_cornerTop = "\xe2\x94\xac";         // ┬
        std::string m_cornerTopRight = "\xe2\x95\xae";    // ╮
        std::string m_cornerBottomLeft = "\xe2\x95\xb0";  // ╰
        std::string m_cornerBottomRight = "\xe2\x95\xaf"; // ╯
        std::string m_cornerBottom = "\xe2\x94\xb4";      // ┴

        std::string m_boxLeft = "\xe2\x94\x9c";  // ├
        std::string m_boxRight = "\xe2\x94\xa4"; // ┤

        std::string m_crossLeft = "\xe2\x94\x9c";  // ├
        std::string m_crossRight = "\xe2\x94\xa4"; // ┤

        std::string m_underConnector = "\xe2\x94\xac"; // ┬
        std::string m_underline = "\xe2\x94\x80";      // ─

        std::string m_pointerRight = "\xe2\x95\xb0\xe2\x94\x80\xe2\x94\x80"; // ╰──
    };
}