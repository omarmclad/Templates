#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

template<typename ftype = double>
struct point3d {
    ftype x, y, z;

    point3d() : x(0), y(0), z(0) {}
    point3d(ftype x, ftype y, ftype z) : x(x), y(y), z(z) {}

    point3d& operator+=(const point3d &t) { x += t.x; y += t.y; z += t.z; return *this; }
    point3d& operator-=(const point3d &t) { x -= t.x; y -= t.y; z -= t.z; return *this; }
    point3d& operator*=(ftype t) { x *= t; y *= t; z *= t; return *this; }
    point3d& operator/=(ftype t) { x /= t; y /= t; z /= t; return *this; }

    point3d operator+(const point3d &t) const { return point3d(*this) += t; }
    point3d operator-(const point3d &t) const { return point3d(*this) -= t; }
    point3d operator*(ftype t) const { return point3d(*this) *= t; }
    point3d operator/(ftype t) const { return point3d(*this) /= t; }

    ftype norm() const { return x * x + y * y + z * z; }
    ftype abs() const { return std::sqrt(norm()); }

    // Lexicographical sort with EPS support for floats
    bool operator<(const point3d &t) const {
        if (std::abs(x - t.x) > EPS) return x < t.x;
        if (std::abs(y - t.y) > EPS) return y < t.y;
        return z < t.z - EPS;
    }

    bool operator==(const point3d &t) const {
        return std::abs(x - t.x) <= EPS && 
               std::abs(y - t.y) <= EPS && 
               std::abs(z - t.z) <= EPS;
    }
};

template<typename ftype>
point3d<ftype> operator*(ftype a, point3d<ftype> b) { return b * a; }

template<typename ftype>
ftype dot(const point3d<ftype>& a, const point3d<ftype>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename ftype>
point3d<ftype> cross(const point3d<ftype>& a, const point3d<ftype>& b) {
    return point3d<ftype>(a.y * b.z - a.z * b.y,
                           a.z * b.x - a.x * b.z,
                           a.x * b.y - a.y * b.x);
}

// Scalar Triple Product: a • (b × c) -> Signed Volume of Parallelepiped
template<typename ftype>
ftype triple(const point3d<ftype>& a, const point3d<ftype>& b, const point3d<ftype>& c) {
    return dot(a, cross(b, c));
}

// Intersects 3 Planes using Cramer's Rule
// Returns {true, point} if unique intersection exists, {false, {0,0,0}} if parallel/coplanar.
template<typename ftype>
pair<bool, point3d<double>> intersect_planes(point3d<ftype> a1, point3d<ftype> n1, 
                                            point3d<ftype> a2, point3d<ftype> n2, 
                                            point3d<ftype> a3, point3d<ftype> n3) {
    point3d<double> x(n1.x, n2.x, n3.x);
    point3d<double> y(n1.y, n2.y, n3.y);
    point3d<double> z(n1.z, n2.z, n3.z); 
    point3d<double> d(dot(a1, n1), dot(a2, n2), dot(a3, n3));
    
    double det = triple(point3d<double>(n1.x, n1.y, n1.z),
                        point3d<double>(n2.x, n2.y, n2.z),
                        point3d<double>(n3.x, n3.y, n3.z));

    if (std::abs(det) < EPS) return {false, {0, 0, 0}}; // Parallel or degenerate planes

    point3d<double> pt(triple(d, y, z),
                       triple(x, d, z),
                       triple(x, y, d));
    return {true, pt / det};
}
