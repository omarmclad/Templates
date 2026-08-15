template<typename ftype>
struct point3d {
    ftype x, y, z;
    point3d() : x(0), y(0), z(0) {}
    point3d(ftype x, ftype y, ftype z): x(x), y(y), z(z) {}

    point3d& operator+=(const point3d &t) { x += t.x; y += t.y; z += t.z; return *this; }
    point3d& operator-=(const point3d &t) { x -= t.x; y -= t.y; z -= t.z; return *this; }
    point3d& operator*=(ftype t) { x *= t; y *= t; z *= t; return *this; }
    point3d& operator/=(ftype t) { x /= t; y /= t; z /= t; return *this; }

    point3d operator+(const point3d &t) const { return point3d(*this) += t; }
    point3d operator-(const point3d &t) const { return point3d(*this) -= t; }
    point3d operator*(ftype t) const { return point3d(*this) *= t; }
    point3d operator/(ftype t) const { return point3d(*this) /= t; }

    // Magnitude / Length
    ftype norm() const { return x * x + y * y + z * z; }
    double abs() const { return std::sqrt(norm()); }

    bool operator<(const point3d &t) const {
        if (x != t.x) return x < t.x;
        if (y != t.y) return y < t.y;
        return z < t.z;
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

template<typename ftype>
ftype triple(const point3d<ftype>& a, const point3d<ftype>& b, const point3d<ftype>& c) {
    return dot(a, cross(b, c));
}

// 3-Plane Intersection using Cramer's Rule
template<typename ftype>
point3d<ftype> intersect_planes(point3d<ftype> a1, point3d<ftype> n1, 
                                point3d<ftype> a2, point3d<ftype> n2, 
                                point3d<ftype> a3, point3d<ftype> n3) {
    point3d<ftype> x(n1.x, n2.x, n3.x);
    point3d<ftype> y(n1.y, n2.y, n3.y);
    point3d<ftype> z(n1.z, n2.z, n3.z); 
    point3d<ftype> d(dot(a1, n1), dot(a2, n2), dot(a3, n3));
    
    ftype det = triple(n1, n2, n3);
    // Check if det == 0 before dividing in context with EPS
    return point3d<ftype>(triple(d, y, z),
                          triple(x, d, z),
                          triple(x, y, d)) / det;
}
//==========================================================//
struct point3d {
    ftype x, y, z;
    point3d() {}
    point3d(ftype x, ftype y, ftype z): x(x), y(y), z(z) {}
    point3d& operator+=(const point3d &t) {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }
    point3d& operator-=(const point3d &t) {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }
    point3d& operator*=(ftype t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    point3d& operator/=(ftype t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
    point3d operator+(const point3d &t) const {
        return point3d(*this) += t;
    }
    point3d operator-(const point3d &t) const {
        return point3d(*this) -= t;
    }
    point3d operator*(ftype t) const {
        return point3d(*this) *= t;
    }
    point3d operator/(ftype t) const {
        return point3d(*this) /= t;
    }
    ftype dot(point3d a, point3d b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    point3d cross(point3d a, point3d b) {
        return point3d(a.y * b.z - a.z * b.y,
                       a.z * b.x - a.x * b.z,
                       a.x * b.y - a.y * b.x);
    }
    ftype triple(point3d a, point3d b, point3d c) {
        return dot(a, cross(b, c));
    }
    point3d intersect(point3d a1, point3d n1, point3d a2, point3d n2, point3d a3, point3d n3) {
    point3d x(n1.x, n2.x, n3.x);
    point3d y(n1.y, n2.y, n3.y);
    point3d z(n1.z, n2.z, n3.z); 
    point3d d(dot(a1, n1), dot(a2, n2), dot(a3, n3));
    return point3d(triple(d, y, z),
                   triple(x, d, z),
                   triple(x, y, d)) / triple(n1, n2, n3);
    }
};
point3d operator*(ftype a, point3d b) {
    return b * a;
}
