#ifndef MEREMEMO_TESTS_UTIL_H
#define MEREMEMO_TESTS_UTIL_H

#include <string>
#include <string_view>
#include <vector>

struct Util
{
    static std::string randomString ();

    static bool isTextInFile (
        std::string_view text,
        std::string_view fileName,
        std::vector<std::string> const & wantedTags = {},
        std::vector<std::string> const & unwantedTags = {});
};

#endif // MEREMEMO_TESTS_UTIL_H
