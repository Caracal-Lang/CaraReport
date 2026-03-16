#include <CaraReport/Report.h>

#include <utility>

namespace CaraReport
{
    Report::Report(std::string message)
        : m_message(std::move(message))
    {
    }

    Report::Report(const Report& other)
        : m_level(other.m_level)
        , m_title(other.m_title)
        , m_url(other.m_url)
        , m_message(other.m_message)
        , m_source(other.m_source ? std::make_unique<Source>(*other.m_source) : nullptr)
        , m_labels(other.m_labels)
        , m_fix(other.m_fix)
        , m_related(other.m_related)
    {
    }

    Report& Report::operator=(const Report& other)
    {
        if (this == &other) 
        {
            return *this;
        }

        m_level = other.m_level;
        m_title = other.m_title;
        m_url = other.m_url;
        m_message = other.m_message;
        m_source = (other.m_source ? std::make_unique<Source>(*other.m_source) : nullptr);
        m_labels = other.m_labels;
        m_fix = other.m_fix;
        m_related = other.m_related;

        return *this;
    }

    Report& Report::withLevel(Level level)
    {
        m_level = level;
        return *this;
    }

    Report& Report::withTitle(const std::string& title)
    {
        m_title = title;
        return *this;
    }

    Report& Report::withUrl(const std::string& url)
    {
        m_url = url;
        return *this;
    }

    Report& Report::withSource(std::unique_ptr<Source>&& source)
    {
        m_source = std::move(source);
        return *this;
    }

    Report& Report::withLabel(const Label& label)
    {
        m_labels.push_back(label);
        return *this;
    }

    Report& Report::withLabels(const std::vector<Label>& labels)
    {
        m_labels = labels;
        return *this;
    }

    Report& Report::withFix(const std::string& fix)
    {
        m_fix = fix;
        return *this;
    }

    Report& Report::withRelated(const Report* report)
    {
        m_related.push_back(report);
        return *this;
    }

    Level Report::level() const
    {
        return m_level;
    }

    const std::optional<std::string>& Report::title() const
    {
        return m_title;
    }

    const std::optional<std::string>& Report::url() const
    {
        return m_url;
    }

    const std::string& Report::message() const
    {
        return m_message;
    }

    const Source* Report::source() const
    {
        return m_source.get();
    }

    const std::vector<Label>& Report::labels() const
    {
        return m_labels;
    }

    const std::optional<std::string>& Report::fix() const
    {
        return m_fix;
    }

    const std::vector<const Report*>& Report::related() const
    {
        return m_related;
    }
}
