#ifndef MEREMEMO_LOG_H
#define MEREMEMO_LOG_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace MereMemo
{

class Tag
{
public:
    virtual ~Tag () = default;

    std::string key () const
    {
        return mKey;
    }

    std::string text () const
    {
        return mText;
    }

    virtual std::unique_ptr<Tag> clone () const = 0;

    virtual bool match (Tag const & other) const = 0;

protected:
    Tag (std::string const & key, std::string const & value)
    : mKey(key), mText(key + "=\"" + value + "\"")
    { }

    Tag (std::string const & key, int value)
    : mKey(key), mText(key + "=" + std::to_string(value))
    { }

    Tag (std::string const & key, long long value)
    : mKey(key), mText(key + "=" + std::to_string(value))
    { }

    Tag (std::string const & key, double value)
    : mKey(key), mText(key + "=" + std::to_string(value))
    { }

    Tag (std::string const & key, bool value)
    : mKey(key), mText(key + "=" + (value?"true":"false"))
    { }

private:
    std::string mKey;
    std::string const mText;
};

inline std::string to_string (Tag const & tag)
{
    return tag.text();
}

inline std::ostream & operator << (std::ostream && stream, Tag const & tag)
{
    stream << to_string(tag);
    return stream;
}

enum class TagOperation
{
    None,
    Equal,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual
};

template <typename T, typename ValueT>
class TagType : public Tag
{
public:
    std::unique_ptr<Tag> clone () const override
    {
        return std::unique_ptr<Tag>(
            new T(*static_cast<T const *>(this)));
    }

    bool match (Tag const & other) const override
    {
        if (key() != other.key())
        {
            return false;
        }
        TagType const & otherCast = static_cast<TagType const &>(other);
        if (mOperation == TagOperation::None)
        {
            switch (otherCast.mOperation)
            {
            case TagOperation::None:
                return mValue == otherCast.mValue;

            default:
                return compareTagTypes(mValue,
                    otherCast.mOperation,
                    otherCast.mValue);
            }
        }
        switch (otherCast.mOperation)
        {
        case TagOperation::None:
            return compareTagTypes(otherCast.mValue,
                mOperation,
                mValue);

        default:
            return false;
        }
    }

    ValueT value () const
    {
        return mValue;
    }

protected:
    TagType (ValueT const & value,
        TagOperation operation)
    : Tag(T::key, value), mValue(value), mOperation(operation)
    { }

    virtual bool compareTagTypes (ValueT const & value,
        TagOperation operation,
        ValueT const & criteria) const
    {
        return false;
    }

    ValueT mValue;
    TagOperation mOperation;
};

template <typename T>
class StringTagType : public TagType<T, std::string>
{
protected:
    StringTagType (std::string const & value,
        TagOperation operation)
    : TagType<T, std::string>(value, operation)
    { }

    bool compareTagTypes (std::string const & value,
        TagOperation operation,
        std::string const & criteria) const override
    {
        int result = value.compare(criteria);
        switch (operation)
        {
        case TagOperation::Equal:
            return result == 0;

        case TagOperation::LessThan:
            return result == -1;

        case TagOperation::LessThanOrEqual:
            return result == 0 || result == -1;

        case TagOperation::GreaterThan:
            return result == 1;

        case TagOperation::GreaterThanOrEqual:
            return result == 0 || result == 1;

        default:
            return false;
        }
    }
};

template <typename T>
class IntTagType : public TagType<T, int>
{
protected:
    IntTagType (int const & value,
        TagOperation operation)
    : TagType<T, int>(value, operation)
    { }

    bool compareTagTypes (int const & value,
        TagOperation operation,
        int const & criteria) const override
    {
        switch (operation)
        {
        case TagOperation::Equal:
            return value == criteria;

        case TagOperation::LessThan:
            return value < criteria;

        case TagOperation::LessThanOrEqual:
            return value <= criteria;

        case TagOperation::GreaterThan:
            return value > criteria;

        case TagOperation::GreaterThanOrEqual:
            return value >= criteria;

        default:
            return false;
        }
    }
};

template <typename T>
class LongLongTagType : public TagType<T, long long>
{
protected:
    LongLongTagType (long long const & value,
        TagOperation operation)
    : TagType<T, long long>(value, operation)
    { }

    bool compareTagTypes (long long const & value,
        TagOperation operation,
        long long const & criteria) const override
    {
        switch (operation)
        {
        case TagOperation::Equal:
            return value == criteria;

        case TagOperation::LessThan:
            return value < criteria;

        case TagOperation::LessThanOrEqual:
            return value <= criteria;

        case TagOperation::GreaterThan:
            return value > criteria;

        case TagOperation::GreaterThanOrEqual:
            return value >= criteria;

        default:
            return false;
        }
    }
};

template <typename T>
class DoubleTagType : public TagType<T, double>
{
protected:
    DoubleTagType (double const & value,
        TagOperation operation)
    : TagType<T, double>(value, operation)
    { }

    bool compareTagTypes (double const & value,
        TagOperation operation,
        double const & criteria) const override
    {
        switch (operation)
        {
        case TagOperation::Equal:
            return value == criteria;

        case TagOperation::LessThan:
            return value < criteria;

        case TagOperation::LessThanOrEqual:
            return value <= criteria;

        case TagOperation::GreaterThan:
            return value > criteria;

        case TagOperation::GreaterThanOrEqual:
            return value >= criteria;

        default:
            return false;
        }
    }
};

template <typename T>
class BoolTagType : public TagType<T, bool>
{
protected:
    BoolTagType (bool const & value,
        TagOperation operation)
    : TagType<T, bool>(value, operation)
    { }

    bool compareTagTypes (bool const & value,
        TagOperation operation,
        bool const & criteria) const override
    {
        switch (operation)
        {
        case TagOperation::Equal:
            return value == criteria;

        default:
            return false;
        }
    }
};

class LogLevel : public StringTagType<LogLevel>
{
public:
    static constexpr char key[] = "log_level";

    LogLevel (std::string const & value,
        TagOperation operation = TagOperation::None)
    : StringTagType(value, operation)
    { }
};

inline std::map<std::string, std::unique_ptr<Tag>> & getDefaultTags ()
{
    static std::map<std::string, std::unique_ptr<Tag>> tags;
    return tags;
}

inline void addDefaultTag (Tag const & tag)
{
    auto & tags = getDefaultTags();
    tags[tag.key()] = tag.clone();
}

struct FilterClause
{
    std::vector<std::unique_ptr<Tag>> normalLiterals;
    std::vector<std::unique_ptr<Tag>> invertedLiterals;
};

inline std::map<int, FilterClause> & getFilterClauses ()
{
    static std::map<int, FilterClause> clauses;
    return clauses;
}

inline int createFilterClause ()
{
    static int currentId = 0;
    ++currentId;
    auto & clauses = getFilterClauses();
    clauses[currentId] = FilterClause();

    return currentId;
}

inline void addFilterLiteral (int filterId,
    Tag const & tag,
    bool normal = true)
{
    auto & clauses = getFilterClauses();
    if (clauses.contains(filterId))
    {
        if (normal)
        {
            clauses[filterId].normalLiterals.push_back(
                tag.clone());
        }
        else
        {
            clauses[filterId].invertedLiterals.push_back(
                tag.clone());
        }
    }
}

inline void clearFilterClause (int filterId)
{
    auto & clauses = getFilterClauses();
    clauses.erase(filterId);
}

class LogStream : public std::stringstream
{
public:
    LogStream (std::string const & filename,
        std::ios_base::openmode mode = ios_base::app)
    : mProceed(true), mFile(filename, mode)
    { }

    LogStream (LogStream const & other) = delete;

    LogStream (LogStream && other)
    : std::stringstream(std::move(other)),
    mProceed(other.mProceed), mFile(std::move(other.mFile))
    { }

    ~LogStream ()
    {
        if (not mProceed)
        {
            return;
        }
        mFile << this->str();
        mFile << std::endl;
    }

    LogStream & operator = (LogStream const & rhs) = delete;
    LogStream & operator = (LogStream && rhs) = delete;

    void ignore ()
    {
        mProceed = false;
    }

private:
    bool mProceed;
    std::fstream mFile;
};

inline LogStream log (std::vector<Tag const *> tags = {})
{
    auto const now = std::chrono::system_clock::now();
    std::time_t const tmNow = std::chrono::system_clock::to_time_t(now);
    auto const ms = duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    LogStream ls("application.log");
    ls << std::put_time(std::gmtime(&tmNow), "%Y-%m-%dT%H:%M:%S.")
        << std::setw(3) << std::setfill('0') << std::to_string(ms.count());

    std::map<std::string, Tag const *> activeTags;
    for (auto const & defaultTag: getDefaultTags())
    {
        activeTags[defaultTag.first] = defaultTag.second.get();
    }
    for (auto const & tag: tags)
    {
        activeTags[tag->key()] = tag;
    }
    for (auto const & activeEntry: activeTags)
    {
        ls << " " << activeEntry.second->text();
    }
    ls << " ";

    bool proceed = true;
    for (auto const & clause: getFilterClauses())
    {
        proceed = false;
        bool allLiteralsMatch = true;
        for (auto const & normal: clause.second.normalLiterals)
        {
            // We need to make sure that the tag is
            // present and with the correct value.
            if (not activeTags.contains(normal->key()))
            {
                allLiteralsMatch = false;
                break;
            }
            if (not activeTags[normal->key()]->match(*normal))
            {
                allLiteralsMatch = false;
                break;
            }
        }
        if (not allLiteralsMatch)
        {
            continue;
        }
        for (auto const & inverted: clause.second.invertedLiterals)
        {
            // We need to make sure that the tag is either
            // not present or has a mismatched value.
            if (activeTags.contains(inverted->key()))
            {
                if (activeTags[inverted->key()]->match(*inverted))
                {
                    allLiteralsMatch = false;
                }
                break;
            }
        }
        if (allLiteralsMatch)
        {
            proceed = true;
            break;
        }
    }

    if (not proceed)
    {
        ls.ignore();
    }
    return ls;
}

inline auto log (Tag const & tag1)
{
    return log({&tag1});
}

inline auto log (Tag const & tag1,
    Tag const & tag2)
{
    return log({&tag1, &tag2});
}

inline auto log (Tag const & tag1,
    Tag const & tag2,
    Tag const & tag3)
{
    return log({&tag1, &tag2, &tag3});
}

} // namespace MereMemo

#endif // MEREMEMO_LOG_H
