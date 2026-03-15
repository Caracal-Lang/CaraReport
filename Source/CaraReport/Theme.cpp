#include <CaraReport/Theme.h>

namespace CaraReport
{
    std::string Theme::levelColor(Level level) const
    {
        return m_levelColors[static_cast<int>(level)];
    }

    std::vector<std::string>& Theme::labelPalette()
    {
        return m_labelPaletteVec;
    }

    std::string Theme::frameColor() const
    {
        return m_frameColor;
    }

    std::string Theme::fixColor() const
    {
        return m_fixColor;
    }

    std::string Theme::titleColor() const
    {
        return m_titleColor;
    }

    std::string Theme::reset() const
    {
        return m_reset;
    }

    std::string Theme::horizontalBar() const
    {
        return m_horizontalBar;
    }

    std::string Theme::verticalBar() const
    {
        return m_verticalBar;
    }

    std::string Theme::intersection() const
    {
        return m_intersection;
    }

    std::string Theme::verticalBreak() const
    {
        return m_verticalBreak;
    }

    std::string Theme::upArrow() const
    {
        return m_upArrow;
    }

    std::string Theme::rightArrow() const
    {
        return m_rightArrow;
    }

    std::string Theme::cornerTopLeft() const
    {
        return m_cornerTopLeft;
    }

    std::string Theme::cornerTop() const
    {
        return m_cornerTop;
    }

    std::string Theme::cornerTopRight() const
    {
        return m_cornerTopRight;
    }

    std::string Theme::cornerBottomLeft() const
    {
        return m_cornerBottomLeft;
    }

    std::string Theme::cornerBottomRight() const
    {
        return m_cornerBottomRight;
    }

    std::string Theme::cornerBottom() const
    {
        return m_cornerBottom;
    }

    std::string Theme::boxLeft() const
    {
        return m_boxLeft;
    }

    std::string Theme::boxRight() const
    {
        return m_boxRight;
    }

    std::string Theme::crossLeft() const
    {
        return m_crossLeft;
    }

    std::string Theme::crossRight() const
    {
        return m_crossRight;
    }

    std::string Theme::underConnector() const
    {
        return m_underConnector;
    }

    std::string Theme::underline() const
    {
        return m_underline;
    }

    std::string Theme::pointerRight() const
    {
        return m_pointerRight;
    }

    bool Theme::isUnicode() const
    {
        return m_unicode;
    }

    void Theme::setLevelColor(Level level, const std::string& color)
    {
        m_levelColors[static_cast<int>(level)] = color;
    }

    void Theme::setTitleColor(const std::string& color)
    {
        m_titleColor = color;
    }

    void Theme::setLabelPalette(const std::vector<std::string>& palette)
    {
        m_labelPaletteVec = palette;
    }

    void Theme::setFrameColor(const std::string& color)
    {
        m_frameColor = color;
    }

    void Theme::setFixColor(const std::string& color)
    {
        m_fixColor = color;
    }

    void Theme::setReset(const std::string& resetCode)
    {
        m_reset = resetCode;
    }

    void Theme::setHorizontalBar(const std::string& glyph)
    {
        m_horizontalBar = glyph;
    }

    void Theme::setVerticalBar(const std::string& glyph)
    {
        m_verticalBar = glyph;
    }

    void Theme::setIntersection(const std::string& glyph)
    {
        m_intersection = glyph;
    }

    void Theme::setVerticalBreak(const std::string& glyph)
    {
        m_verticalBreak = glyph;
    }

    void Theme::setUpArrow(const std::string& glyph)
    {
        m_upArrow = glyph;
    }

    void Theme::setRightArrow(const std::string& glyph)
    {
        m_rightArrow = glyph;
    }

    void Theme::setCornerTopLeft(const std::string& glyph)
    {
        m_cornerTopLeft = glyph;
    }

    void Theme::setCornerTop(const std::string& glyph)
    {
        m_cornerTop = glyph;
    }

    void Theme::setCornerTopRight(const std::string& glyph)
    {
        m_cornerTopRight = glyph;
    }

    void Theme::setCornerBottomLeft(const std::string& glyph)
    {
        m_cornerBottomLeft = glyph;
    }

    void Theme::setCornerBottomRight(const std::string& glyph)
    {
        m_cornerBottomRight = glyph;
    }

    void Theme::setCornerBottom(const std::string& glyph)
    {
        m_cornerBottom = glyph;
    }

    void Theme::setBoxLeft(const std::string& glyph)
    {
        m_boxLeft = glyph;
    }

    void Theme::setBoxRight(const std::string& glyph)
    {
        m_boxRight = glyph;
    }

    void Theme::setCrossLeft(const std::string& glyph)
    {
        m_crossLeft = glyph;
    }

    void Theme::setCrossRight(const std::string& glyph)
    {
        m_crossRight = glyph;
    }

    void Theme::setUnderConnector(const std::string& glyph)
    {
        m_underConnector = glyph;
    }

    void Theme::setUnderline(const std::string& glyph)
    {
        m_underline = glyph;
    }

    void Theme::setPointerRight(const std::string& glyph)
    {
        m_pointerRight = glyph;
    }

    void Theme::setUnicode(bool enable)
    {
        m_unicode = enable;
    }

    std::unique_ptr<Theme> Theme::ascii()
    {
        auto theme = std::make_unique<Theme>();
        theme->setUnicode(false);
        theme->setHorizontalBar("-");
        theme->setVerticalBar("|");
        theme->setIntersection("+");
        theme->setVerticalBreak(".");
        theme->setUpArrow("^");
        theme->setRightArrow(">");
        theme->setCornerTopLeft(",");
        theme->setCornerTop("v");
        theme->setCornerTopRight(".");
        theme->setCornerBottomLeft("`");
        theme->setCornerBottomRight("'");
        theme->setCornerBottom("^");
        theme->setBoxLeft("[");
        theme->setBoxRight("]");
        theme->setCrossLeft("|");
        theme->setCrossRight("|");
        theme->setUnderConnector("-");
        theme->setUnderline("^");
        theme->setPointerRight("`--");
        return theme;
    }
}
