#ifndef MSGPACKSEARCH_ERROR_H
#define MSGPACKSEARCH_ERROR_H

#include <exception>
#include <string>

namespace msgpacksearch {

class bad_object_type : public std::exception {

public:
    bad_object_type(const std::string& message) : message(message) {};

    virtual const char *what() const throw() {
        return message.c_str();
    }

private:
    std::string message;
};

}


#endif //MSGPACKSEARCH_ERROR_H

