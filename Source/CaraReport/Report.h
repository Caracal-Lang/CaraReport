#pragma once

#include <CaraReport/API.h>
#include <CaraReport/Label.h>
#include <CaraReport/Level.h>
#include <CaraReport/Source.h>
#include <CaraReport/Span.h>
#include <memory>
#include <string>
#include <vector>

namespace CaraReport
{
    class CARAREPORT_API Report
    {
    public:
        explicit Report(std::string message);
        Report(const Report& other);
        Report(Report&& other) noexcept = default;
        Report& operator=(const Report& other);
        Report& operator=(Report&& other) noexcept = default;
        ~Report() = default;

        [[nodiscard]] Report& withLevel(Level level);
        [[nodiscard]] Report& withTitle(const std::string& title);
        [[nodiscard]] Report& withUrl(const std::string& url);
        [[nodiscard]] Report& withSource(std::unique_ptr<Source>&& source);
        [[nodiscard]] Report& withLabel(const Label& label);
        [[nodiscard]] Report& withLabels(const std::vector<Label>& labels);
        [[nodiscard]] Report& withFix(const std::string& fix);
        [[nodiscard]] Report& withRelated(const Report* report);

        [[nodiscard]] Level level() const;
        [[nodiscard]] const std::string& title() const;
        [[nodiscard]] const std::string& url() const;
        [[nodiscard]] const std::string& message() const;
        [[nodiscard]] const Source* source() const;
        [[nodiscard]] const std::vector<Label>& labels() const;
        [[nodiscard]] const std::string& fix() const;
        [[nodiscard]] const std::vector<const Report*>& related() const;

    private:
        Level m_level = Level::Error;
        std::string m_title;
        std::string m_url;
        std::string m_message;
        std::unique_ptr<Source> m_source;
        std::vector<Label> m_labels;
        std::string m_fix;
        std::vector<const Report*> m_related;
    };
}
