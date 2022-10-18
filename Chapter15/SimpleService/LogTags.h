#ifndef SIMPLESERVICE_LOGTAGS_H
#define SIMPLESERVICE_LOGTAGS_H

#include <MereMemo/Log.h>

namespace SimpleService
{

inline MereMemo::LogLevel error("error");
inline MereMemo::LogLevel info("info");
inline MereMemo::LogLevel debug("debug");

class User : public MereMemo::StringTagType<User>
{
public:
    static constexpr char key[] = "user";

    User (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

class LogPath : public MereMemo::StringTagType<LogPath>
{
public:
    static constexpr char key[] = "logpath";

    LogPath (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

class Request : public MereMemo::StringTagType<Request>
{
public:
    static constexpr char key[] = "request";

    Request (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

class Response : public MereMemo::StringTagType<Response>
{
public:
    static constexpr char key[] = "response";

    Response (std::string const & value,
        MereMemo::TagOperation operation =
            MereMemo::TagOperation::None)
    : StringTagType(value, operation)
    { }
};

} // namespace SimpleService

#endif // SIMPLESERVICE_LOGTAGS_H
