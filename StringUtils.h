#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

// "Natural" string order: digits are compared as numbers, so Name2 comes
// before Name10. Letters are compared without regard to case.
bool naturalLess(const std::string& a, const std::string& b);

#endif
