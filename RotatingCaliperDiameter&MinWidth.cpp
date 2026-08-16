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

// ============================================================================
// ADDITIONAL GEOMETRY EXTENSIONS
// ============================================================================

/*
 * Distance from point P to line segment AB.
 * Logic:
 * - Project AP onto AB using dot product.
 * - If dot(AB, AP) < 0, closest point on segment is A.
 * - If dot(BA, BP) < 0, closest point on segment is B.
 * - Otherwise, projection falls strictly between A and B, so return perpendicular line distance.
 */
double dist_to_segment(Pt p, Pt a, Pt b) {
    Pt ab = b - a, ap = p - a;
    if (dot(ab, ap) < 0) return len(ap); // P projects behind A
    Pt ba = a - b, bp = p - b;
    if (dot(ba, bp) < 0) return len(bp); // P projects beyond B
    return dist_to_line(p, a, b);        // Perpendicular distance to segment
}

/*
 * ANTIPODAL PAIRS - O(N)
 * Generates all pairs of vertices (i, j) that can be simultaneously touched 
 * by two parallel supporting lines framing the convex hull.
 * Crucial base primitive for all rotating calipers techniques.
 */
vector<pair<int, int>> get_antipodal_pairs(const vector<Pt>& h) {
    int n = h.size();
    vector<pair<int, int>> pairs;
    if (n <= 1) return pairs;
    if (n == 2) { pairs.push_back({0, 1}); return pairs; }

    int j = 1;
    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        // Move j as long as edge vector h[i]->h[ni] forms a positive cross product with vector h[j]->h[j+1]
        while (cross(h[ni] - h[i], h[(j + 1) % n] - h[j]) > EPS) {
            j = (j + 1) % n;
        }
        pairs.push_back({i, j});
        pairs.push_back({ni, j});
        
        // Handle parallel edges: if parallel, both vertices on opposite side form antipodal pairs
        if (abs(cross(h[ni] - h[i], h[(j + 1) % n] - h[j])) <= EPS) {
            pairs.push_back({i, (j + 1) % n});
            pairs.push_back({ni, (j + 1) % n});
        }
    }
    return pairs;
}

/*
 * SMALLEST ENCLOSING RECTANGLE (Min Area / Min Perimeter Bounding Box) - O(N)
 * Uses 4 rotating calipers (one along an edge, three tangent at top, right, and left extremes).
 *
 * Concepts:
 * - Base edge: (h[i], h[ni])
 * - Top pointer: Maximizes perpendicular height from base edge (cross product).
 * - Right pointer: Maximizes projection along edge direction (dot product).
 * - Left pointer: Minimizes projection along edge direction (dot product).
 */
struct BoundingBoxResult {
    double min_area;
    double min_perimeter;
};

BoundingBoxResult min_bounding_box(const vector<Pt>& h) {
    int n = h.size();
    if (n <= 2) return {0.0, 0.0};

    double min_area = 1e18, min_peri = 1e18;
    int top = 1, right = 1, left = 1;

    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        Pt edge = h[ni] - h[i];
        double edge_len = len(edge);

        // 1. Advance 'top' to maximize area / height relative to base edge
        while (cross(edge, h[(top + 1) % n] - h[i]) > cross(edge, h[top] - h[i]) - EPS) {
            top = (top + 1) % n;
        }
        // 2. Advance 'right' to maximize projection along base edge
        while (dot(edge, h[(right + 1) % n] - h[i]) > dot(edge, h[right] - h[i]) - EPS) {
            right = (right + 1) % n;
        }
        if (i == 0) left = right;
        // 3. Advance 'left' to minimize projection along base edge
        while (dot(edge, h[(left + 1) % n] - h[i]) < dot(edge, h[left] - h[i]) + EPS) {
            left = (left + 1) % n;
        }

        // Calculate bounding box dimensions along the current base angle
        double height = abs(cross(edge, h[top] - h[i])) / edge_len;
        double width = (dot(edge, h[right] - h[i]) - dot(edge, h[left] - h[i])) / edge_len;

        min_area = min(min_area, height * width);
        min_peri = min(min_peri, 2.0 * (height + width));
    }

    return {min_area, min_peri};
}

/*
 * MINKOWSKI SUM OF TWO CONVEX POLYGONS - O(N + M)
 * Computes polygon C = A + B = { a + b | a in A, b in B }.
 *
 * Algorithm:
 * 1. Reorder vertices of A and B so lowest-leftmost vertex is first.
 * 2. Merge edge vectors of A and B in polar angle order (like two-pointer merge in MergeSort).
 */
vector<Pt> minkowski_sum(vector<Pt> A, vector<Pt> B) {
    auto reorder = [](vector<Pt>& P) {
        size_t pos = 0;
        for (size_t i = 1; i < P.size(); i++) {
            if (P[i].y < P[pos].y || (abs(P[i].y - P[pos].y) < EPS && P[i].x < P[pos].x)) {
                pos = i;
            }
        }
        rotate(P.begin(), P.begin() + pos, P.end());
    };

    reorder(A); reorder(B);
    int n = A.size(), m = B.size();
    // Append wrap-around points for seamless transition
    A.push_back(A[0]); A.push_back(A[1]);
    B.push_back(B[0]); B.push_back(B[1]);

    vector<Pt> res;
    int i = 0, j = 0;
    while (i < n || j < m) {
        res.push_back(A[i] + B[j]);
        // Compare polar angle of current edge vector of A vs current edge vector of B
        double crs = cross(A[i + 1] - A[i], B[j + 1] - B[j]);
        if (crs >= -EPS && i < n) i++;
        if (crs <= EPS && j < m) j++;
    }
    return convex_hull(res);
}

/*
 * MINIMUM DISTANCE BETWEEN TWO CONVEX POLYGONS - O(N + M)
 * Uses Minkowski Difference: dist(A, B) = dist(Origin (0,0), A - B)
 * Where A - B = Minkowski Sum of A and (-B).
 */
bool point_in_convex(Pt p, const vector<Pt>& h) {
    int n = h.size();
    for (int i = 0; i < n; i++) {
        if (cross(h[(i + 1) % n] - h[i], p - h[i]) < -EPS) return false;
    }
    return true;
}

double min_dist_convex_polygons(vector<Pt> A, vector<Pt> B) {
    // Negate polygon B coordinates to convert subtraction into Minkowski addition: A + (-B)
    vector<Pt> negB = B;
    for (auto& p : negB) p = Pt{-p.x, -p.y};
    
    vector<Pt> C = minkowski_sum(A, negB);
    
    // If origin is inside the Minkowski difference, polygons overlap (distance is 0)
    if (point_in_convex({0, 0}, C)) return 0.0;

    // Otherwise, minimum distance is distance from (0,0) to boundary of C
    double min_d = 1e18;
    int n = C.size();
    for (int i = 0; i < n; i++) {
        min_d = min(min_d, dist_to_segment({0, 0}, C[i], C[(i + 1) % n]));
    }
    return min_d;
}

/*
 * MAXIMUM DISTANCE BETWEEN TWO CONVEX POLYGONS - O(N + M)
 * Uses two simultaneous calipers resting on both polygons, rotating them in sync.
 */
double max_dist_convex_polygons(const vector<Pt>& A, const vector<Pt>& B) {
    int n = A.size(), m = B.size();
    if (n == 0 || m == 0) return 0.0;

    int ymaxA = 0, yminB = 0;
    for (int i = 1; i < n; i++) if (A[i].y > A[ymaxA].y) ymaxA = i;
    for (int i = 1; i < m; i++) if (B[i].y < B[yminB].y) yminB = i;

    double max_d = 0;
    int i = ymaxA, j = yminB;
    for (int step = 0; step < n + m; step++) {
        max_d = max(max_d, dist(A[i], B[j]));
        int ni = (i + 1) % n;
        int nj = (j + 1) % m;
        // Compare edge directions to decide which caliper moves forward first
        if (cross(A[ni] - A[i], B[j] - B[nj]) > EPS) {
            i = ni;
        } else {
            j = nj;
        }
    }
    return max_d;
}

/*
 * MAXIMUM INSCRIBED TRIANGLE AREA - O(N)
 * Finds 3 vertices (a, b, c) of convex polygon that maximize triangle area.
 * Uses 3 pointers: as 'a' moves, 'b' and 'c' advance monotonically around polygon.
 */
double max_triangle_area(const vector<Pt>& h) {
    int n = h.size();
    if (n < 3) return 0;
    double max_a = 0;
    int b = 1, c = 2;

    auto area = [&](int i, int j, int k) {
        return abs(cross(h[i], h[j], h[k])) * 0.5;
    };

    for (int a = 0; a < n; a++) {
        while (area(a, b, (c + 1) % n) >= area(a, b, c)) c = (c + 1) % n;
        while (area(a, (b + 1) % n, c) >= area(a, b, c)) b = (b + 1) % n;
        max_a = max(max_a, area(a, b, c));
    }
    return max_a;
}

/*
 * MAXIMUM INSCRIBED QUADRILATERAL AREA - O(N^2)
 * Fixes a diagonal (i, j) and finds optimal vertex on each side of diagonal using 2 calipers.
 */
double max_quadrilateral_area(const vector<Pt>& h) {
    int n = h.size();
    if (n < 4) return 0;
    double max_a = 0;

    auto area = [&](int i, int j, int k) {
        return abs(cross(h[i], h[j], h[k])) * 0.5;
    };

    for (int i = 0; i < n; i++) {
        int p1 = (i + 1) % n;
        int p2 = (i + 3) % n;
        for (int j = (i + 2) % n; j != i; j = (j + 1) % n) {
            // Advance pointer on top side of diagonal (i, j)
            while (area(i, j, (p1 + 1) % n) >= area(i, j, p1)) p1 = (p1 + 1) % n;
            // Advance pointer on bottom side of diagonal (i, j)
            while (area(i, j, (p2 + 1) % n) >= area(i, j, p2)) p2 = (p2 + 1) % n;
            max_a = max(max_a, area(i, j, p1) + area(i, j, p2));
        }
    }
    return max_a;
}

/*
 * TANGENTS FROM POINT TO CONVEX POLYGON - O(log N)
 * Uses binary search on CCW convex polygon vertices to find both tangent indices.
 * Returns pair {idx_left_tangent, idx_right_tangent}.
 */
pair<int, int> tangents_to_convex(Pt p, const vector<Pt>& h) {
    int n = h.size();

    auto is_above = [&](Pt a, Pt b, Pt c) { return cross(b - a, c - a) > EPS; };
    auto is_below = [&](Pt a, Pt b, Pt c) { return cross(b - a, c - a) < -EPS; };

    auto find_tangent = [&](bool is_left) {
        int l = 0, r = n;
        while (l < r) {
            int mid = (l + r) / 2;
            Pt cur = h[mid], next = h[(mid + 1) % n], prev = h[(mid - 1 + n) % n];

            bool cur_dir = is_left ? cross(cur - p, next - cur) >= -EPS 
                                  : cross(cur - p, next - cur) <= EPS;
            bool prev_dir = is_left ? cross(cur - p, prev - cur) >= -EPS 
                                   : cross(cur - p, prev - cur) <= EPS;

            // Found tangent vertex where both adjacent edges stay on one side of ray P->h[mid]
            if (cur_dir && prev_dir) return mid;

            bool p_above_mid = is_left ? is_above(p, h[l], h[mid]) 
                                       : is_below(p, h[l], h[mid]);
            bool mid_above_next = is_left ? is_above(p, h[mid], next) 
                                          : is_below(p, h[mid], next);

            if (!cur_dir) {
                if (p_above_mid && !mid_above_next) r = mid;
                else l = mid + 1;
            } else {
                if (!p_above_mid && mid_above_next) l = mid + 1;
                else r = mid;
            }
        }
        return l % n;
    };

    return {find_tangent(true), find_tangent(false)};
}
