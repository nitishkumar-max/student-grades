#include "StringUtils.h"

#include <cctype>

bool naturalLess(const std::string& a, const std::string& b) {
    std::size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned char ca = a[i], cb = b[j];
        if (std::isdigit(ca) && std::isdigit(cb)) {
            std::size_t ie = i, je = j;
            while (ie < a.size() && std::isdigit(static_cast<unsigned char>(a[ie]))) ++ie;
            while (je < b.size() && std::isdigit(static_cast<unsigned char>(b[je]))) ++je;
            std::size_t is = i, js = j;                     // skip leading zeros
            while (is + 1 < ie && a[is] == '0') ++is;
            while (js + 1 < je && b[js] == '0') ++js;
            if (ie - is != je - js) return ie - is < je - js;
            int cmp = a.compare(is, ie - is, b, js, je - js);
            if (cmp != 0) return cmp < 0;
            i = ie;
            j = je;
        } else {
            int la = std::tolower(ca), lb = std::tolower(cb);
            if (la != lb) return la < lb;
            ++i;
            ++j;
        }
    }
    return a.size() - i < b.size() - j;
}
