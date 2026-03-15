#pragma once

#include <CaraReport/Label.h>
#include <CaraReport/Level.h>
#include <CaraReport/Source.h>
#include <CaraReport/Span.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace CaraReport
{
    class Report
    {
    public:
        explicit Report(const std::string& message);
        Report(const Report& other);
        Report(Report&& other) noexcept = default;
        Report& operator=(const Report& other);
        Report& operator=(Report&& other) noexcept = default;

        [[nodiscard]] Report& withLevel(Level level);
        [[nodiscard]] Report& withTitle(const std::string& title);
        [[nodiscard]] Report& withUrl(const std::string& url);
        [[nodiscard]] Report& withSource(std::unique_ptr<Source>&& source);
        [[nodiscard]] Report& withLabel(const Label& label);
        [[nodiscard]] Report& withLabels(const std::vector<Label>& labels);
        [[nodiscard]] Report& withFix(const std::string& fix);
        [[nodiscard]] Report& withRelated(const Report* report);

        [[nodiscard]] Level level() const;
        [[nodiscard]] std::optional<std::string> title() const;
        [[nodiscard]] std::optional<std::string> url() const;
        [[nodiscard]] std::string message() const;
        [[nodiscard]] const Source* source() const;
        [[nodiscard]] std::vector<Label> labels() const;
        [[nodiscard]] std::optional<std::string> fix() const;
        [[nodiscard]] std::vector<const Report*> related() const;

    private:
        Level m_level = Level::Error;
        std::optional<std::string> m_title;
        std::optional<std::string> m_url;
        std::string m_message;
        std::unique_ptr<Source> m_source;
        std::vector<Label> m_labels;
        std::optional<std::string> m_fix;
        std::vector<const Report*> m_related;
    };
}
