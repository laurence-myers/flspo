#ifndef FL_STUDIO_PLUGIN_ORGANIZER_ERROR_H
#define FL_STUDIO_PLUGIN_ORGANIZER_ERROR_H

#include <exception>
#include <string>

class UnexpectedValueError: public std::exception {
    [[nodiscard]]
    const char * what () const throw() override
    {
        return "Unexpected value";
    }
};

#endif //FL_STUDIO_PLUGIN_ORGANIZER_ERROR_H
