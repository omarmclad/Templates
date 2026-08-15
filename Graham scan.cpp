#include <bits/stdc++.h>
using namespace std;

using Pt = pair<double, double>;
#define x first
#define y second

const double EPS = 1e-9;

Pt operator+(Pt a, Pt b) { return {a.x + b.x, a.y + b.y}; }
Pt operator-(Pt a, Pt b) { return {a.x - b.x, a.y - b.y}; }
Pt operator*(Pt a, double k) { return {a.x * k, a.y * k}; }
Pt operator/(Pt a, double k) { return {a.x / k, a.y / k}; }

double dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }
double cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }
double cross(Pt a, Pt b, Pt c) { return cross(b - a, c - a); }
double norm(Pt a) { return dot(a, a); }
double len(Pt a) { return sqrt(norm(a)); }
double dist(Pt a, Pt b) { return len(a - b); }

// Distance from point p to infinite line ab
double dist_to_line(Pt p, Pt a, Pt b) {
    return abs(cross(a, b, p)) / dist(a, b);
}

// Monotone Chain Convex Hull (CCW Order, strict hull)
vector<Pt> convex_hull(vector<Pt>& pts) {
    int n = pts.size(), k = 0;
    if (n <= 1) return pts;
    vector<Pt> h(2 * n);
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = pts.size();

    // Lower hull
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], pts[i]) <= EPS) k--;
        h[k++] = pts[i];
    }
    // Upper hull
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(h[k - 2], h[k - 1], pts[i]) <= EPS) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}

// Maximum pairwise distance (Diameter) using Rotating Calipers - O(N)
double rotating_calipers_diameter(vector<Pt>& pts) {
    vector<Pt> hull = convex_hull(pts);
    int n = hull.size();
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return dist(hull[0], hull[1]);

    double max_d = 0;
    int j = 1;

    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        // Advance j while the triangle area with edge (i, ni) increases
        while (abs(cross(hull[i], hull[ni], hull[(j + 1) % n])) > 
               abs(cross(hull[i], hull[ni], hull[j])) + EPS) {
            j = (j + 1) % n;
        }
        max_d = max({max_d, dist(hull[i], hull[j]), dist(hull[ni], hull[j])});
    }
    return max_d;
}

// Minimum bounding width using Rotating Calipers - O(N)
double rotating_calipers_min_width(vector<Pt>& pts) {
    vector<Pt> hull = convex_hull(pts);
    int n = hull.size();
    if (n <= 2) return 0;

    double min_w = 1e18;
    int j = 1;

    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        while (abs(cross(hull[i], hull[ni], hull[(j + 1) % n])) > 
               abs(cross(hull[i], hull[ni], hull[j])) + EPS) {
            j = (j + 1) % n;
        }
        double height = dist_to_line(hull[j], hull[i], hull[ni]);
        min_w = min(min_w, height);
    }
    return min_w;
}
