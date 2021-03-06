///////////////////////////////////////////////////////////////////////////////
//                                 Hex                                       //
///////////////////////////////////////////////////////////////////////////////

// https://github.com/lchsk/knight-libs
// v0.2.0

#pragma once

#include <cmath>
#include <vector>

#define EQUAL_DOUBLE(a, b) (abs(a - b) < 0.00001)
#define IS_ZERO_DOUBLE(a) (abs(a) < 0.00001)
#define NON_ZERO_DOUBLE(a) (!IS_ZERO_DOUBLE(a))

namespace K {

struct Point {
    Point(double x, double y);
    Point();

    bool operator==(Point const &rhs) const;
    bool operator!=(Point const &rhs) const;

    double x;
    double y;
};

struct Orientation {
    const double f0;
    const double f1;
    const double f2;
    const double f3;
    const double b0;
    const double b1;
    const double b2;
    const double b3;
    const double start_angle;

    Orientation(double f0, double f1, double f2, double f3, double b0,
                double b1, double b2, double b3, double start_angle)
        : f0(f0), f1(f1), f2(f2), f3(f3), b0(b0), b1(b1), b2(b2), b3(b3),
          start_angle(start_angle) {}
};

struct Layout {
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation, Point size, Point origin)
        : orientation(orientation), size(size), origin(origin) {}
};

struct Hex {
    Hex(double q, double r, double s) : q(q), r(r), s(s) {
        if (round(q + r + s) != 0)
            throw "q + r + s must be 0";
    }

    Hex() : Hex(0, 0, 0) {}

    const std::string to_str() const;
    const std::string to_float_str() const;

    Point const to_point(Layout const &layout) const;
    void to_int();
    Hex lerp(const Hex &, double) const;
    const std::vector<Hex> path_to(const Hex &) const;

    bool operator==(Hex const &rhs) const;

    Hex &operator+=(const Hex &rhs);
    Hex operator+(const Hex &rhs);

    Hex &operator-=(const Hex &rhs);
    Hex operator-(const Hex &rhs);

    void scale(int k);
    void rotate_left();
    void rotate_right();

    int length() const;
    int distance(const Hex &h) const;

    Hex direction(int) const;
    Hex neighbor(int) const;
    Hex diagonal_neighbor(int) const;
    const std::vector<Point> polygon_corners(Layout const &layout) const;

    inline static const K::Orientation ORIENTATION_POINTY =
        K::Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
                       sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
    inline static const K::Orientation ORIENTATION_FLAT =
        K::Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0,
                       0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

    static Hex from_point(Layout const &layout, const Point &p);
    static Point corner_offset(Layout const &layout, int corner);

    double q;
    double r;
    double s;
};

static const K::Hex directions[] = {Hex(1, 0, -1), Hex(1, -1, 0),
                                    Hex(0, -1, 1), Hex(-1, 0, 1),
                                    Hex(-1, 1, 0), Hex(0, 1, -1)};
static const K::Hex diagonals[] = {Hex(2, -1, -1), Hex(1, -2, 1),
                                   Hex(-1, -1, 2), Hex(-2, 1, 1),
                                   Hex(-1, 2, -1), Hex(1, 1, -2)};

template <class T> inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
}

namespace std {
template <> struct hash<K::Hex> {
    inline size_t operator()(K::Hex const &v) const {
        size_t seed = 0;
        K::hash_combine(seed, v.q);
        K::hash_combine(seed, v.r);
        K::hash_combine(seed, v.s);

        return seed;
    }
};
}
