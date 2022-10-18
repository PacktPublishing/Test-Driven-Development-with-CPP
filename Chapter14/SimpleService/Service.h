#ifndef SIMPLESERVICE_SERVICE_H
#define SIMPLESERVICE_SERVICE_H

#include <string>

namespace SimpleService
{

class Service
{
public:
    void start ();

    std::string handleRequest (std::string const & user,
        std::string const & path,
        std::string const & request);
};

} // namespace SimpleService

#endif // SIMPLESERVICE_SERVICE_H
