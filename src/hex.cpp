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

    bool Hex::operator==(Hex const &rhs) const {
        return EQUAL_DOUBLE(q, rhs.q) && EQUAL_DOUBLE(r, rhs.r) && EQUAL_DOUBLE(s, rhs.s);
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

void Hex::scale(int k) {
    q *= k;
    r *= k;
    s *= k;
}

void Hex::rotate_left() {
    int q_ = q;

    q = -s;
    s = -r;
    r = -q;
}

void Hex::rotate_right() {
    int q_ = q;

    q = -r;
    r = -s;
    s = -q_;
}

int Hex::length() const { return int((abs(q) + abs(r) + abs(s)) / 2.0); }

int Hex::distance(const Hex &h) const {
    auto copy = *this;
    copy -= h;

    return copy.length();
}

Hex Hex::direction(int d) const { return K::directions[d]; }

Hex Hex::neighbor(int d) const {
    auto const &direction_hex = direction(d);

    Hex h = *this;
    h += direction_hex;

    return h;
}

Hex Hex::diagonal_neighbor(int d) const {
    auto const &direction_hex = diagonals[d];

    Hex h = *this;
    h += direction_hex;

    return h;
}

std::ostream &operator<<(std::ostream &os, Hex const &h) {
    return os << "(" << h.q << ", " << h.r << ", " << h.s << ")";
}
}
