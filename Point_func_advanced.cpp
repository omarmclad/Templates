#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 1. SETUP & TYPE DEFINITIONS
// ============================================================================
// Using std::pair<double, double> instead of a custom struct:
// - Provides built-in operator< for sorting, std::set, std::map, std::unique.
// - x and y macros allow clean access syntax (pt.x, pt.y) instead of pt.first, pt.second.
using Pt = pair<double, double>;
#define x first
#define y second

const double EPS = 1e-9;
const double PI = acos(-1.0);

// ============================================================================
// 2. PRECISION HANDLING & MATH HELPERS
// ============================================================================

// Returns: +1 if val > EPS, -1 if val < -EPS, 0 if |val| <= EPS
// Used for floating-point comparisons to prevent precision noise bugs.
int sign(double val) { 
    return (val > EPS) - (val < -EPS); 
}

// Clamps input into [-1.0, 1.0] before calling acos().
// Crucial: Floating-point inaccuracy can produce values like 1.0000000000000002,
// which causes raw acos() to return NaN (Not a Number) without throwing an error.
double safe_acos(double val) { 
    return acos(max(-1.0, min(1.0, val))); 
}

// ============================================================================
// 3. VECTOR ARITHMETIC & OPERATORS
// ============================================================================
// Allows direct vector operations: (a + b), (a - b), (a * k), (a / k).

Pt operator+(Pt a, Pt b) { return {a.x + b.x, a.y + b.y}; }
Pt operator-(Pt a, Pt b) { return {a.x - b.x, a.y - b.y}; }
Pt operator*(Pt a, double k) { return {a.x * k, a.y * k}; }
Pt operator*(double k, Pt a) { return {a.x * k, a.y * k}; }
Pt operator/(Pt a, double k) { return {a.x / k, a.y / k}; }

// ============================================================================
// 4. VECTOR PRODUCTS, NORMS & TRANSFORMATIONS
// ============================================================================

// Dot Product: a • b = a.x * b.x + a.y * b.y = |a| * |b| * cos(theta)
// - > 0: Angle < 90 deg (acute)
// - = 0: Perpendicular (90 deg)
// - < 0: Angle > 90 deg (obtuse)
double dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }

// 2D Cross Product: a × b = a.x * b.y - a.y * b.x = |a| * |b| * sin(theta)
// - Represents the signed area of the parallelogram formed by a and b.
// - > 0: b is to the left of vector a (counter-clockwise turn)
// - = 0: a and b are collinear (parallel or anti-parallel)
// - < 0: b is to the right of vector a (clockwise turn)
double cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }

// 3-Point Cross Product: (b - a) × (c - a)
// Determines orientation of point c relative to directed line segment ab:
// - > 0: c lies to the left of ab (CCW)
// - = 0: c is collinear with ab
// - < 0: c lies to the right of ab (CW)
double cross(Pt a, Pt b, Pt c) { return cross(b - a, c - a); }

// Squared magnitude (length^2) of vector a: x^2 + y^2.
// Prefer norm() over len() when comparing lengths to avoid sqrt() precision loss and overhead.
double norm(Pt a) { return dot(a, a); }

// Euclidean length (magnitude) of vector a: sqrt(x^2 + y^2).
double len(Pt a) { return sqrt(norm(a)); }

// Euclidean distance between two points a and b.
double dist(Pt a, Pt b) { return len(a - b); }

// Rotates vector a by 90 degrees counter-clockwise: (x, y) -> (-y, x)
Pt rot90(Pt a) { return {-a.y, a.x}; }

// Converts vector a to a unit vector (length = 1) in the same direction.
// Handles zero-vector safely.
Pt unit(Pt a) {
    double l = len(a);
    return (l < EPS) ? Pt{0, 0} : a / l;
}

// Vector Projection: Projects point p onto line vector b starting from origin.
// Formula: proj_b(p) = b * [(p • b) / |b|^2]
Pt project_onto(Pt p, Pt b) {
    double l2 = norm(b);
    return (l2 < EPS) ? Pt{0, 0} : b * (dot(p, b) / l2);
}

// Reflects point p across the line defined by vector b starting from origin.
// Formula: 2 * proj_b(p) - p
Pt reflect(Pt p, Pt b) {
    return project_onto(p, b) * 2.0 - p;
}

// Returns the unsigned angle in radians [0, PI] between vectors a and b.
double angle_with(Pt a, Pt b) {
    double la = len(a), lb = len(b);
    if (la < EPS || lb < EPS) return 0.0;
    return safe_acos(dot(a, b) / (la * lb));
}

// ============================================================================
// 5. DISTANCE COMPUTATIONS (LINE, RAY, SEGMENT)
// ============================================================================

// Perpendicular distance from point p to infinite line passing through points a and b.
// Formula: |(b - a) × (p - a)| / |b - a| (Height of triangle abp)
double dist_to_line(Pt p, Pt a, Pt b) {
    Pt ab = b - a;
    double l = len(ab);
    return (l < EPS) ? dist(p, a) : abs(cross(ab, p - a)) / l;
}

// Shortest distance from point p to ray ab (starts at a, extends infinitely through b).
// Checks if the perpendicular projection falls behind origin point a.
double dist_to_ray(Pt p, Pt a, Pt b) {
    Pt ab = b - a, ap = p - a;
    double l2 = norm(ab);
    if (l2 < EPS) return len(ap); // Degenerate ray (a == b)
    double u = dot(ap, ab) / l2;  // Projection ratio along ray
    if (u < 0.0) return len(ap);   // Projection falls behind a -> closest point is a
    return abs(cross(ab, ap)) / sqrt(l2); // Perpendicular distance
}

// Shortest distance from point p to finite line segment ab.
// Handles three regions:
// 1. Projection before a (u < 0) -> distance to endpoint a
// 2. Projection beyond b (u > 1) -> distance to endpoint b
// 3. Projection on segment (0 <= u <= 1) -> perpendicular distance
double dist_to_segment(Pt p, Pt a, Pt b) {
    Pt ab = b - a, ap = p - a;
    double l2 = norm(ab);
    if (l2 < EPS) return len(ap); // Degenerate segment (a == b)
    double u = dot(ap, ab) / l2;
    if (u < 0.0) return len(ap);
    if (u > 1.0) return dist(p, b);
    return abs(cross(ab, ap)) / sqrt(l2);
}

// ============================================================================
// 6. INTERSECTIONS & SEGMENT/RAY DISTANCES
// ============================================================================

// Returns true if point p lies strictly on segment ab.
// Checks both collinearity (cross == 0) and bounding box inclusion (dot <= 0).
bool on_segment(Pt p, Pt a, Pt b) {
    return sign(cross(a, b, p)) == 0 && sign(dot(a - p, b - p)) <= 0;
}

// Returns true if segment ab and segment cd intersect (properly or touching).
bool segment_intersection(Pt a, Pt b, Pt c, Pt d) {
    double c1 = cross(a, b, c), c2 = cross(a, b, d);
    double c3 = cross(c, d, a), c4 = cross(c, d, b);

    // Proper straddle check: c and d are on opposite sides of ab AND a and b are on opposite sides of cd
    if (sign(c1) * sign(c2) < 0 && sign(c3) * sign(c4) < 0) return true;

    // Boundary/collinear endpoint touch checks
    if (on_segment(c, a, b) || on_segment(d, a, b)) return true;
    if (on_segment(a, c, d) || on_segment(b, c, d)) return true;
    return false;
}

// Minimum distance between segment ab and segment cd.
// If they intersect, distance is 0. Otherwise, min of 4 point-to-segment distances.
double dist_segments(Pt a, Pt b, Pt c, Pt d) {
    if (segment_intersection(a, b, c, d)) return 0.0;
    return min({
        dist_to_segment(c, a, b),
        dist_to_segment(d, a, b),
        dist_to_segment(a, c, d),
        dist_to_segment(b, c, d)
    });
}

// Minimum distance between ray AB (starts at A) and ray CD (starts at C).
double dist_rays(Pt A, Pt B, Pt C, Pt D) {
    Pt AB = B - A, CD = D - C;
    if (norm(AB) < EPS && norm(CD) < EPS) return dist(A, C);
    if (norm(AB) < EPS) return dist_to_ray(A, C, D);
    if (norm(CD) < EPS) return dist_to_ray(C, A, B);

    double crs = cross(AB, CD);
    Pt AC = C - A;

    if (abs(crs) > EPS) { // Non-parallel rays: solve for parameter intersection t and s
        double t = cross(AC, CD) / crs;
        double s = cross(AC, AB) / crs;
        if (t >= -EPS && s >= -EPS) return 0.0; // Rays intersect
    } else { // Parallel or collinear rays
        if (abs(cross(C - A, CD)) < EPS) { // Collinear line check
            double proj1 = dot(C - A, AB) / norm(AB);
            double proj2 = dot(A - C, CD) / norm(CD);
            if (proj1 >= -EPS || proj2 >= -EPS) return 0.0; // Overlapping rays
        }
    }
    // No intersection -> minimum distance from origin endpoints to opposite ray
    return min(dist_to_ray(A, C, D), dist_to_ray(C, A, B));
}

// ============================================================================
// 7. GEOMETRIC CONSTRUCTIONS & TRANSFORMATIONS
// ============================================================================

// Returns a unit vector that bisects the angle formed by vectors a and b.
// Useful for finding angle bisectors or interior bisectors of a triangle.
Pt bisector(Pt a, Pt b) {
    Pt ua = unit(a), ub = unit(b);
    Pt sum = ua + ub;
    if (norm(sum) < EPS) return {0, 0}; // Opposite directions (180 deg)
    return unit(sum);
}

// Constructs 4 vertices of a square given opposite diagonal corners a and c in CCW order.
vector<Pt> get_square_from_diagonal(Pt a, Pt c) {
    Pt o = (a + c) / 2.0;       // Center of square
    Pt v = rot90(a - o);        // Rotate vector (a - o) by 90 deg
    return {a, o + v, c, o - v};
}

// Shifts point a by vector v.
Pt translate(Pt a, Pt v) { return a + v; }

// Scales point a relative to a reference center point `ref` by `factor`.
Pt scale(Pt a, Pt ref, double factor) { return ref + (a - ref) * factor; }

// Rotates point a around reference point `ref` by `angle` radians (CCW).
Pt rotate(Pt a, Pt ref, double angle) {
    Pt v = a - ref;
    Pt c = {cos(angle), sin(angle)};
    Pt res = {v.x * c.x - v.y * c.y, v.x * c.y + v.y * c.x};
    return ref + res;
}

// Interpolates position along vector ab matching the projection ratio of point r.
Pt general_transform(Pt a, Pt acpy, Pt b, Pt bcpy, Pt r) {
    if (a == b) return acpy;
    Pt ab = b - a, ar = r - a;
    double t = dot(ar, ab) / norm(ab);
    return acpy + (bcpy - acpy) * t;
}

// ============================================================================
// 8. POLAR ANGLE SORTING
// ============================================================================

Pt O_ref; // Global origin/pivot point for polar sorting

// Comparator for sorting points counter-clockwise around pivot point O_ref:
// 1. Splits points into upper half-plane (y >= 0) and lower half-plane (y < 0).
// 2. Uses 2D cross product for orientation comparison within the same half-plane.
// 3. Breaks collinear ties by distance (closer points first) to ensure deterministic sorting.
bool polar_cmp(Pt a, Pt b) {
    Pt va = a - O_ref, vb = b - O_ref;
    
    // Half-plane indicator: 1 if in upper half-plane or positive x-axis, 0 if lower
    bool half_a = (va.y > 0 || (va.y == 0 && va.x >= 0));
    bool half_b = (vb.y > 0 || (vb.y == 0 && vb.x >= 0));

    if (half_a != half_b) return half_a > half_b;
    double crs = cross(va, vb);
    if (abs(crs) > EPS) return crs > 0; // CCW comparison
    return norm(va) < norm(vb);        // Collinear tie-breaker by squared distance
}
