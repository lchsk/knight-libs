#include <fstream>

#include "hex.hpp"

namespace K {
Point::Point(double x, double y) : x(x), y(y) {}

Point const Hex::to_pixel(Layout const &layout) const {
    Orientation const &o = layout.orientation;
    Point const &size = layout.size;
    Point const &origin = layout.origin;

    const double x = (o.f0 * q + o.f1 * r) * size.x;
    const double y = (o.f2 * q + o.f3 * r) * size.y;

    return Point(x + origin.x, y + origin.y);
}

Hex &Hex::operator+=(const Hex &rhs) {
    q += rhs.q;
    r += rhs.r;
    s += rhs.s;

    return *this;
}

Hex Hex::operator+(const Hex &rhs) {
    *this += rhs;
    return *this;
}

Hex &Hex::operator-=(const Hex &rhs) {
    q -= rhs.q;
    r -= rhs.r;
    s -= rhs.s;

    return *this;
}

Hex Hex::operator-(const Hex &rhs) {
    *this -= rhs;
    return *this;
}

std::ostream &operator<<(std::ostream &os, Hex const &h) {
    return os << "(" << h.q << ", " << h.r << ", " << h.s << ")";
}
}
