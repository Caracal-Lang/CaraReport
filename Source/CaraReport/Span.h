#pragma once

namespace CaraReport
{
    class Span
    {
      public:
        Span();
        Span(int start, int length);

        [[nodiscard]] static Span fromRange(int start, int end);

        [[nodiscard]] int start() const;
        [[nodiscard]] int end() const;
        [[nodiscard]] int length() const;
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] bool contains(int position) const;
        [[nodiscard]] bool overlaps(const Span& other) const;

      private:
        int m_start;
        int m_length;
    };
}
