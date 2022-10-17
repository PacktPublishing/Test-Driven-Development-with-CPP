#include "Util.h"

#include <chrono>
#include <fstream>
#include <random>

std::string Util::randomString ()
{
    static bool firstCall = true;
    static std::mt19937 rng;
    if (firstCall)
    {
        // We only need to set the seed once.
        firstCall = false;

        unsigned int seed = static_cast<int>(
            std::chrono::system_clock::now().
            time_since_epoch().count());
        rng.seed(seed);
    }
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 10000);

    return std::to_string(dist(rng));
}

bool Util::isTextInFile (
    std::string_view text,
    std::string_view fileName,
    std::vector<std::string> const & wantedTags,
    std::vector<std::string> const & unwantedTags)
{
    std::ifstream logfile(fileName.data());
    std::string line;
    while (getline(logfile, line))
    {
        if (line.find(text) != std::string::npos)
        {
            for (auto const & tag: wantedTags)
            {
                if (line.find(tag) == std::string::npos)
                {
                    return false;
                }
            }
            for (auto const & tag: unwantedTags)
            {
                if (line.find(tag) != std::string::npos)
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
