#include "Service.h"

#include "LogTags.h"

#include <MereMemo/Log.h>

void SimpleService::Service::start ()
{
    MereMemo::FileOutput appFile("logs");
    MereMemo::addLogOutput(appFile);

    MereMemo::log(info) << "Service is starting.";
}

std::string SimpleService::Service::handleRequest (
    std::string const & user,
    std::string const & path,
    std::string const & request)
{
    MereMemo::log(debug, User(user), LogPath(path))
        << "Received: " << Request(request);

    std::string response;
    if (request == "Hello")
    {
        response = "Hi, " + user;
    }
    else
    {
        response = "Unrecognized request.";
    }

    MereMemo::log(debug, User(user), LogPath(path))
        << "Sending: " << Response(response);
    return response;
}
