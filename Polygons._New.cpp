#include <bits/stdc++.h>
using namespace std;

// Point representation using std::pair<double, double>
using pt = pair<double, double>;

// ============================================================================
// 1. PREREQUISITE VECTOR HELPERS
// ============================================================================

pt add(const pt &a, const pt &b) { return {a.first + b.first, a.second + b.second}; }
pt sub(const pt &a, const pt &b) { return {a.first - b.first, a.second - b.second}; }
pt mul(const pt &a, double k)    { return {a.first * k, a.second * k}; }

// 2D Cross Product: a x b = a.x * b.y - a.y * b.x
double cross(const pt &a, const pt &b) { return a.first * b.second - a.second * b.first; }

// 3-Point Cross Product: (b - a) x (c - a)
// > 0: c is to the left of directed line ab (CCW)
// = 0: c is collinear with line ab
// < 0: c is to the right of directed line ab (CW)
double cross(const pt &a, const pt &b, const pt &c) { return cross(sub(b, a), sub(c, a)); }

// Dot Product: a • b = a.x * b.x + a.y * b.y
double dot(const pt &a, const pt &b) { return a.first * b.first + a.second * b.second; }

// Squared Euclidean distance between two points
double dist2(const pt &a, const pt &b) {
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return dx * dx + dy * dy;
}

// ============================================================================
// 2. CORE POLYGON ALGORITHMS
// ============================================================================

// Shoelace Formula: Computes area of any non-self-intersecting polygon. O(N)
double polygonArea(const vector<pt> &poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i)
        area += cross(poly[i], poly[(i + 1) % n]);
    return fabs(area) / 2.0;
}

// Point-in-Polygon (Ray Casting + Boundary Check)
// Returns: 0 = Outside, 1 = On Boundary Edge, 2 = Strictly Inside. O(N)
int pointInPolygon(const vector<pt> &poly, const pt &p) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt a = poly[i], b = poly[(i + 1) % n];
        pt ab = sub(b, a), ap = sub(p, a), bp = sub(p, b);

        // 1. Boundary check: collinear (cross == 0) and within segment bounding box
        if (fabs(cross(ab, ap)) < 1e-9 && dot(ap, ab) >= -1e-9 && dot(bp, ab) <= 1e-9)
            return 1;

        // 2. Ray-Casting: test intersection with horizontal ray extending right from p
        if ((a.second > p.second) != (b.second > p.second)) {
            double x_int = a.first + (b.first - a.first) * (p.second - a.second) / (b.second - a.second);
            if (x_int > p.first)
                inside = !inside;
        }
    }
    return inside ? 2 : 0;
}

// Polygon Cut (Single Line Cut): Cuts poly by infinite directed line a -> b.
// Keeps the sub-polygon lying to the LEFT (CCW side) of line a -> b. O(N)
vector<pt> polygonCut(const vector<pt> &poly, pt a, pt b) {
    vector<pt> res;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt curr = poly[i], next = poly[(i + 1) % n];
        
        double side_curr = cross(a, b, curr);
        double side_next = cross(a, b, next);

        bool in_curr = side_curr >= -1e-9; // On or left of cut line
        bool in_next = side_next >= -1e-9;

        if (in_curr) res.push_back(curr);

        // Edge crosses cutting line: compute exact linear intersection point
        if (in_curr != in_next) {
            double t = side_curr / (side_curr - side_next);
            res.push_back(add(curr, mul(sub(next, curr), t)));
        }
    }
    return res;
}

// Polygon Clipping (Sutherland-Hodgman): Intersects subject polygon with clipper.
// NOTE: `clipper` vertices MUST be ordered Counter-Clockwise (CCW). O(N * M)
vector<pt> clipPolygon(const vector<pt> &subject, const vector<pt> &clipper) {
    vector<pt> res = subject;
    int n = clipper.size();
    for (int i = 0; i < n; ++i) {
        pt a = clipper[i], b = clipper[(i + 1) % n];
        res = polygonCut(res, a, b);
    }
    return res;
}

// ============================================================================
// 3. ADVANCED CONVEX POLYGON FUNCTIONS
// ============================================================================

// Point in Convex Polygon Test in O(log N)
// Precondition: poly vertices must be given in strict CCW order, with poly[0] as origin.
// Returns: true if point p is strictly inside or on the boundary.
bool pointInConvexPolygon(const vector<pt> &poly, const pt &p) {
    int n = poly.size();
    if (n < 3) return false;
    
    // Check if p is to the right of ray poly[0]->poly[1] or poly[0]->poly[n-1]
    if (cross(poly[0], poly[1], p) < -1e-9 || cross(poly[0], poly[n - 1], p) > 1e-9)
        return false;

    // Binary search for the wedge containing p
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (cross(poly[0], poly[mid], p) >= -1e-9) l = mid;
        else r = mid;
    }

    // Test if p lies inside the triangle formed by poly[0], poly[l], poly[l+1]
    return cross(poly[l], poly[l + 1], p) >= -1e-9;
}

// Maximum Diameter of a Convex Polygon using Rotating Calipers. O(N)
double convexPolygonDiameter(const vector<pt> &poly) {
    int n = poly.size();
    if (n <= 1) return 0;
    if (n == 2) return sqrt(dist2(poly[0], poly[1]));

    double max_d2 = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (cross(sub(poly[ni], poly[i]), sub(poly[(j + 1) % n], poly[i])) > 
               cross(sub(poly[ni], poly[i]), sub(poly[j], poly[i]))) {
            j = (j + 1) % n;
        }
        max_d2 = max({max_d2, dist2(poly[i], poly[j]), dist2(poly[ni], poly[j])});
    }
    return sqrt(max_d2);
}
