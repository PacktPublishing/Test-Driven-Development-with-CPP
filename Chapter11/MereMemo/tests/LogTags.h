#ifndef MEREMEMO_TESTS_LOGTAGS_H
#define MEREMEMO_TESTS_LOGTAGS_H

#include "../Log.h"

inline MereMemo::LogLevel error("error");
inline MereMemo::LogLevel info("info");
inline MereMemo::LogLevel debug("debug");

class Color : public MereMemo::StringTagType<Color>
{
public:
    static constexpr char key[] = "color";

    Color (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

inline Color red("red");
inline Color green("green");
inline Color blue("blue");

class Size : public MereMemo::StringTagType<Size>
{
public:
    static constexpr char key[] = "size";

    Size (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

inline Size small("small");
inline Size medium("medium");
inline Size large("large");

class Count : public MereMemo::IntTagType<Count>
{
public:
    static constexpr char key[] = "count";

    Count (int value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : IntTagType(value, operation)
    { }
};

class Identity : public MereMemo::LongLongTagType<Identity>
{
public:
    static constexpr char key[] = "id";

    Identity (long long value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : LongLongTagType(value, operation)
    { }
};

class Scale : public MereMemo::DoubleTagType<Scale>
{
public:
    static constexpr char key[] = "scale";

    Scale (double value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : DoubleTagType(value, operation)
    { }
};

class CacheHit : public MereMemo::BoolTagType<CacheHit>
{
public:
    static constexpr char key[] = "cache_hit";

    CacheHit (bool value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : BoolTagType(value, operation)
    { }
};

inline CacheHit cacheHit(true);
inline CacheHit cacheMiss(false);

#endif // MEREMEMO_TESTS_LOGTAGS_H
