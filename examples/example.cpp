#include <iostream>
#include "../namegen.hpp"

int main()
{
    std::cout << dasmig::ng::instance().get_name().append_surname() << std::endl;

    return 0;
}