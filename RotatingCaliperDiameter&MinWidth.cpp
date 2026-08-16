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
    double d = dist(a, b);
    if (d < EPS) return dist(p, a); // Handles degenerate line (a == b)
    return abs(cross(a, b, p)) / d;
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
 * Distance from Point P to Line Segment AB.
 * Logic: Projects P onto vector AB using dot product.
 * Safety: Handles A == B cleanly via dist_to_line.
 * Time Complexity: O(1)
 */
double dist_to_segment(Pt p, Pt a, Pt b) {
    if (dist(a, b) < EPS) return dist(p, a); // Degenerate segment
    Pt ab = b - a, ap = p - a;
    if (dot(ab, ap) <= 0) return len(ap); // P projects behind endpoint A
    Pt ba = a - b, bp = p - b;
    if (dot(ba, bp) <= 0) return len(bp); // P projects beyond endpoint B
    return dist_to_line(p, a, b);        // Perpendicular projection falls on segment
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
BoundingBoxResult min_bounding_box(const vector<Pt>& h) {
    int n = h.size();
    if (n <= 1) return {0.0, 0.0};
    if (n == 2) {
        double d = dist(h[0], h[1]);
        return {0.0, 2.0 * d};
    }

    double min_area = 1e18, min_peri = 1e18;
    int top = 1, right = 1, left = 1;

    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        Pt edge = h[ni] - h[i];
        double edge_len = len(edge);
        if (edge_len < EPS) continue;
        // 1. Advance 'top' to maximize area / height relative to base edge
        while (cross(edge, h[(top + 1) % n] - h[i]) > cross(edge, h[top] - h[i])) {
            top = (top + 1) % n;
        }
        // 2. Advance 'right' to maximize projection along base edge
        while (dot(edge, h[(right + 1) % n] - h[i]) > dot(edge, h[right] - h[i])) {
            right = (right + 1) % n;
        }
        if (i == 0) left = right;
        // 3. Advance 'left' to minimize projection along base edge
        while (dot(edge, h[(left + 1) % n] - h[i]) <= dot(edge, h[left] - h[i])) {
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

// ============================================================================
// 3. MINKOWSKI SUM & POLYGON DISTANCES
// ============================================================================

/*
 * Minkowski Sum of two convex polygons A and B: C = { a + b | a ∈ A, b ∈ B }.
 * Algorithm: Polar angle merge of edge vectors.
 * Edge Cases Handled: Empty polygons, single points, line segments, duplicate edges.
 * Preconditions: Polygons should be convex.
 * Time Complexity: O(N + M)
 */
vector<Pt> minkowski_sum(vector<Pt> A, vector<Pt> B) {
    if (A.empty() || B.empty()) return {};

    // Remove zero-length edges safely without destroying 1-point polygons
    auto clean = [&](vector<Pt>& P) {
        if (P.size() <= 1) return;
        vector<Pt> Q;
        for (size_t i = 0; i < P.size(); i++) {
            if (len(P[(i + 1) % P.size()] - P[i]) > EPS)
                Q.push_back(P[i]);
        }
        if (!Q.empty()) P.swap(Q);
    };

    clean(A); clean(B);
    A = convex_hull(A);
    B = convex_hull(B);

    int n = A.size(), m = B.size();
    if (n == 0 || m == 0) return {};
    if (n == 1) { vector<Pt> res = B; for (auto& p : res) p = p + A[0]; return res; }
    if (m == 1) { vector<Pt> res = A; for (auto& p : res) p = p + B[0]; return res; }

    auto rotate_lowest = [&](vector<Pt>& P) {
        int pos = 0;
        for (size_t i = 1; i < P.size(); i++) {
            if (P[i].y < P[pos].y - EPS || 
               (abs(P[i].y - P[pos].y) <= EPS && P[i].x < P[pos].x)) {
                pos = i;
            }
        }
        rotate(P.begin(), P.begin() + pos, P.end());
    };

    rotate_lowest(A);
    rotate_lowest(B);

    vector<Pt> ea(n), eb(m);
    for (int i = 0; i < n; i++) ea[i] = A[(i + 1) % n] - A[i];
    for (int i = 0; i < m; i++) eb[i] = B[(i + 1) % m] - B[i];

    vector<Pt> res;
    res.reserve(n + m);

    int i = 0, j = 0;
    Pt cur = A[0] + B[0];
    res.push_back(cur);

    while (i < n || j < m) {
        Pt move;
        if (i == n) { move = eb[j++]; }
        else if (j == m) { move = ea[i++]; }
        else {
            double cr = cross(ea[i], eb[j]);
            if (cr > EPS) { move = ea[i++]; }
            else if (cr < -EPS) { move = eb[j++]; }
            else {
                move = ea[i] + eb[j];
                i++; j++;
            }
        }
        cur = cur + move;
        res.push_back(cur);
    }
    res.pop_back();
    return convex_hull(res);
}

/*
/*
 * Point Inclusion in Convex Polygon.
 * Preconditions: Polygon 'h' MUST be in strictly CCW order.
 * Safety: Handles N = 1 (point check) and N = 2 (segment check).
 * Time Complexity: O(N)
 */
bool point_in_convex(Pt p, const vector<Pt>& h) {
    int n = h.size();
    if (n == 0) return false;
    if (n == 1) return dist(p, h[0]) <= EPS;
    if (n == 2) return dist_to_segment(p, h[0], h[1]) <= EPS;

    for (int i = 0; i < n; i++) {
        // If P is strictly to the right of any edge, it lies outside
        if (cross(h[i], h[(i + 1) % n], p) < -EPS) return false;
    }
    return true;
}

/*
 * Minimum Euclidean Distance between Two Convex Polygons.
 * Formula: dist(A, B) = min_dist_from_origin_to(A + (-B))
 * Returns 0 if polygons overlap or touch.
 * Time Complexity: O(N + M)
 */
double min_dist_convex(const vector<Pt>& A, const vector<Pt>& B) {
    // Negate B to transform subtraction into Minkowski Addition: A + (-B)
    vector<Pt> negB = B;
    for (auto& p : negB) p = Pt{-p.x, -p.y};

    vector<Pt> C = minkowski_sum(A, negB);
    if (C.empty()) return 0;

    // Origin (0,0) inside Minkowski Difference -> Polygons intersect
    if (point_in_convex({0, 0}, C)) return 0;
    if (C.size() == 1) return dist({0, 0}, C[0]);
    if (C.size() == 2) return dist_to_segment({0, 0}, C[0], C[1]);

    // Minimum distance from Origin (0,0) to boundary of Minkowski Polygon C
    double ans = 1e18;
    int n = C.size();
    for (int i = 0; i < n; i++) {
        ans = min(ans, dist_to_segment({0, 0}, C[i], C[(i + 1) % n]));
    }
    return ans;
}
/*
 * Maximum Distance between Two Convex Polygons.
 * Returns the maximum pairwise distance between any vertex of A and vertex of B.
 * Time Complexity: O(N + M)
 */
double max_dist_convex(const vector<Pt>& A, const vector<Pt>& B) {
    vector<Pt> negB = B;
    for (auto& p : negB) p = Pt{-p.x, -p.y};

    vector<Pt> C = minkowski_sum(A, negB);
    double ans = 0;
    for (auto p : C) ans = max(ans, len(p));
    return ans;
}

// ============================================================================
// 4. INSCRIBED POLYGON AREA MAXIMIZATION
// ============================================================================

// Maximum Area Triangle inscribed inside a Convex Polygon.
// Algorithm: Dobkin-Snyder (Rotating Calipers)
// Preconditions: Input must be a CCW Convex Hull (n >= 3).
// Time Complexity: O(N)
double max_triangle_area(const vector<Pt>& h) {
    int n = h.size();
    if (n < 3) return 0.0;

    auto area = [&](int i, int j, int k) {
        return abs(cross(h[i], h[j], h[k])) * 0.5;
    };

    double max_a = 0;
    int b = 1, c = 2;

    // Anchor the first point 'a'
    for (int a = 0; a < n; a++) {
        // Fix pointers if they overlap
        if (a == b) b = (a + 1) % n;
        if (b == c) c = (b + 1) % n;

        // Converge b and c for the current 'a'
        while (true) {
            bool moved = false;
            
            // Advance c as long as it strictly increases the area
            while (area(a, b, (c + 1) % n) > area(a, b, c) + EPS) {
                c = (c + 1) % n;
                moved = true;
            }
            // Advance b as long as it strictly increases the area
            while (area(a, (b + 1) % n, c) > area(a, b, c) + EPS) {
                b = (b + 1) % n;
                moved = true;
            }
            
            // If neither b nor c moved in this pass, they are optimal for 'a'
            if (!moved) break;
        }

        // Record the fully optimized triangle for this 'a'
        max_a = max(max_a, area(a, b, c));
    }
    
    return max_a;
}

/*
 * Maximum Area Quadrilateral inscribed inside a Convex Polygon.
 * Algorithm: Fix diagonal (i, j) and find max area vertices on both sides using calipers.
 * Preconditions: Input must be a CCW Convex Hull.
 * Safety: Explicit index inequalities prevent duplicate/overlapping vertex selections.
 * Time Complexity: O(N^2)
 */
double max_quadrilateral_area(const vector<Pt>& h) {
    int n = h.size();
    if (n < 4) return 0.0;

    auto area = [&](int i, int j, int k) {
        return abs(cross(h[i], h[j], h[k])) * 0.5;
    };

    double max_a = 0;

    for (int i = 0; i < n; i++) {
        int p1 = (i + 1) % n;
        int p2 = (i + 3) % n;

        for (int j = (i + 2) % n; j != i; j = (j + 1) % n) {
            if (j == (i + 1) % n) continue;

            // Advance top pointer p1 for diagonal (i, j)
            while ((p1 + 1) % n != j && area(i, j, (p1 + 1) % n) >= area(i, j, p1) - EPS) {
                p1 = (p1 + 1) % n;
            }
            // Advance bottom pointer p2 for diagonal (i, j)
            while ((p2 + 1) % n != i && area(i, j, (p2 + 1) % n) >= area(i, j, p2) - EPS) {
                p2 = (p2 + 1) % n;
            }

            // Ensure valid distinct quadrilateral vertices
            if (p1 != i && p1 != j && p2 != i && p2 != j && p1 != p2) {
                max_a = max(max_a, area(i, j, p1) + area(i, j, p2));
            }
        }
    }
    return max_a;
}

// ============================================================================
// 5. TANGENTS FROM AN EXTERNAL POINT TO A CONVEX POLYGON
// ============================================================================

/*
 * Finds Left and Right Tangent Vertex Indices from Point P to Convex Polygon 'h'.
 * Returns pair {left_tangent_idx, right_tangent_idx}.
 * Time Complexity: O(N) linear scan (safe against edge-case binary search traps).
 */
pair<int, int> tangents_to_convex(Pt p, const vector<Pt>& h) {
    int n = h.size();
    if (n == 0) return {-1, -1};
    if (n == 1) return {0, 0};

    auto is_tangent = [&](int i, bool is_left) {
        Pt prev = h[(i - 1 + n) % n];
        Pt next = h[(i + 1) % n];
        Pt cur = h[i];
        double cr1 = cross(cur - p, next - cur);
        double cr2 = cross(cur - p, prev - cur);
        if (is_left) return cr1 >= -EPS && cr2 >= -EPS;
        return cr1 <= EPS && cr2 <= EPS;
    };

    int l_idx = 0, r_idx = 0;
    for (int i = 0; i < n; i++) {
        if (is_tangent(i, true)) l_idx = i;
        if (is_tangent(i, false)) r_idx = i;
    }
    return {l_idx, r_idx};
}
