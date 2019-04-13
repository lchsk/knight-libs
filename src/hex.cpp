#include <fstream>

#include "hex.hpp"

namespace K {
Point::Point(double x, double y) : x(x), y(y) {}
Point::Point() : x(0), y(0) {}
bool Point::operator==(Point const &rhs) const {
    return EQUAL_DOUBLE(x, rhs.x) && EQUAL_DOUBLE(y, rhs.y);
}
bool Point::operator!=(Point const &rhs) const { return operator==(rhs); }

Point const Hex::to_point(Layout const &layout) const {
    const Orientation &o = layout.orientation;
    const Point &size = layout.size;
    const Point &origin = layout.origin;

    const double x = (o.f0 * q + o.f1 * r) * size.x;
    const double y = (o.f2 * q + o.f3 * r) * size.y;

    return Point(static_cast<int>(x + origin.x),
                 static_cast<int>(y + origin.y));
}

bool Hex::operator==(Hex const &rhs) const {
    return EQUAL_DOUBLE(q, rhs.q) && EQUAL_DOUBLE(r, rhs.r) &&
           EQUAL_DOUBLE(s, rhs.s);
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
void Hex::to_int() {
    int qi = round(q);
    int ri = round(r);
    int si = round(s);

    double q_diff = abs(qi - q);
    double r_diff = abs(ri - r);
    double s_diff = abs(si - s);

    if (q_diff > r_diff && q_diff > s_diff) {
        qi = -ri - si;
    } else {
        if (r_diff > s_diff) {
            ri = -qi - si;
        } else {
            si = -qi - ri;
        }
    }

    q = qi;
    r = ri;
    s = si;
}

Hex Hex::lerp(const Hex &b, double t) const {
    Hex a = *this;

    a.q = a.q * (1.0 - t) + b.q * t;
    a.r = a.r * (1.0 - t) + b.r * t;
    a.s = a.s * (1.0 - t) + b.s * t;

    return a;
}

const std::vector<Hex> Hex::path_to(const Hex &b) const {
    std::vector<Hex> results;

    const auto a_nudge = Hex(q + 0.000001, r + 0.000001, s - 0.000002);
    const auto b_nudge = Hex(b.q + 0.000001, b.r + 0.000001, b.s - 0.000002);

    const auto dist = distance(b);
    const double step = 1.0 / std::max(dist, 1);

    for (int i = 0; i <= dist; i++) {
        Hex h = a_nudge.lerp(b_nudge, step * i);
        h.to_int();
        results.push_back(h);
    }

    return results;
}

const std::vector<Point> Hex::polygon_corners(Layout const &layout) const {
    std::vector<Point> corners;

    const Point center = to_point(layout);

    for (int i = 0; i < 6; i++) {
        const Point offset = Hex::corner_offset(layout, i);
        corners.push_back(Point(center.x + offset.x, center.y + offset.y));
    }

    return corners;
}

Hex Hex::from_point(Layout const &layout, const Point &p) {
    Orientation const &o = layout.orientation;
    const Point &size = layout.size;
    const Point &origin = layout.origin;

    const Point pt =
        Point((p.x - origin.x) / size.x, (p.y - origin.y) / size.y);

    const double q = o.b0 * pt.x + o.b1 * pt.y;
    const double r = o.b2 * pt.x + o.b3 * pt.y;

    return Hex(q, r, -q - r);
}

Point Hex::corner_offset(Layout const &layout, int corner) {
    const Orientation &o = layout.orientation;
    const Point &size = layout.size;

    const double angle = 2.0 * M_PI * (o.start_angle - corner) / 6.0;

    return Point(size.x * cos(angle), size.y * sin(angle));
}

std::ostream &operator<<(std::ostream &os, Hex const &h) {
    return os << "(" << h.q << ", " << h.r << ", " << h.s << ")";
}
}
