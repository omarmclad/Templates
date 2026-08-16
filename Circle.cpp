using Pt = pair<double, double>;
#define x first
#define y second

const double EPS = 1e-9;

int sign(double val) { return (val > EPS) - (val < -EPS); }

Pt operator+(Pt a, Pt b) { return {a.x + b.x, a.y + b.y}; }
Pt operator-(Pt a, Pt b) { return {a.x - b.x, a.y - b.y}; }
Pt operator*(Pt a, double k) { return {a.x * k, a.y * k}; }
Pt operator/(Pt a, double k) { return {a.x / k, a.y / k}; }

double dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }
double cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }
double norm(Pt a) { return dot(a, a); }
double len(Pt a) { return sqrt(norm(a)); }
double dist(Pt a, Pt b) { return len(a - b); }

struct circle {
    Pt p;
    double r;

    circle() : p({0, 0}), r(0) {}
    circle(Pt p, double r) : p(p), r(r) {}

    // Circumcircle / MEC of 3 points
    circle(Pt a, Pt b, Pt c) {
        double d = 2.0 * cross(b - a, c - a);
        if (abs(d) < EPS) { 
            // Collinear: pick max-distance pair as diameter
            double d1 = dist(a, b), d2 = dist(a, c), d3 = dist(b, c);
            if (d1 >= d2 && d1 >= d3)      { p = (a + b) / 2.0; r = d1 / 2.0; }
            else if (d2 >= d1 && d2 >= d3) { p = (a + c) / 2.0; r = d2 / 2.0; }
            else                           { p = (b + c) / 2.0; r = d3 / 2.0; }
        } else {
            // Direct circumcenter formula
            double a2 = norm(a), b2 = norm(b), c2 = norm(c);
            p.x = (a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y)) / d;
            p.y = (a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x)) / d;
            r = dist(p, a);
        }
    }
};

// Expected O(N) Minimum Enclosing Circle
circle minimum_enclosing_circle(vector<Pt> &pts) {
    static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(pts.begin(), pts.end(), rng);

    int n = pts.size();
    if (n == 0) return circle();
    if (n == 1) return circle(pts[0], 0);

    circle c(pts[0], 0);

    for (int i = 1; i < n; i++) {
        if (sign(dist(pts[i], c.p) - c.r) > 0) {
            c = circle(pts[i], 0);
            for (int j = 0; j < i; j++) {
                if (sign(dist(pts[j], c.p) - c.r) > 0) {
                    c = circle((pts[i] + pts[j]) / 2.0, dist(pts[i], pts[j]) / 2.0);
                    for (int k = 0; k < j; k++) {
                        if (sign(dist(pts[k], c.p) - c.r) > 0) {
                            c = circle(pts[i], pts[j], pts[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}


double arc_area(Pt center, double r, double alpha, double beta) {
    return r * r * (beta - alpha) 
         + r * center.x * (sin(beta) - sin(alpha)) 
         - r * center.y * (cos(beta) - cos(alpha));
}

// Computes total union area of N overlapping circles
double circle_union_area(vector<circle>& circles) {
    int n = circles.size();

    // 1. Filter out zero-radius and fully contained / duplicate circles
    vector<circle> c;
    for (int i = 0; i < n; i++) {
        if (circles[i].r <= EPS) continue; // Ignore point-circles
        bool contained = false;
        for (int j = 0; j < n; j++) {
            if (i == j || circles[j].r <= EPS) continue;
            double d = dist(circles[i].p, circles[j].p);
            
            // Circle i is completely inside Circle j
            if (circles[j].r >= circles[i].r + d - EPS) {
                // FIXED: Check for strictly IDENTICAL circles (d ~ 0 and r_i ~ r_j)
                if (d <= EPS && abs(circles[i].r - circles[j].r) <= EPS) {
                    if (i > j) continue; // Keep the circle with smaller index j
                }
                contained = true;
                break;
            }
        }
        if (!contained) c.push_back(circles[i]);
    }

    n = c.size();
    double total_area = 0;

    // 2. Process each circle to isolate exposed boundary arcs
    for (int i = 0; i < n; i++) {
        vector<pair<double, double>> covered;

        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            double d = dist(c[i].p, c[j].p);
            
            // No intersection or touch only at boundary
            if (d >= c[i].r + c[j].r - EPS || d <= abs(c[i].r - c[j].r) + EPS) continue;

            // Angle of vector connecting centers
            double alpha = atan2(c[j].p.y - c[i].p.y, c[j].p.x - c[i].p.x);
            
            // Half-angle subtended by the intersection sector
            double beta = safe_acos((c[i].r * c[i].r + d * d - c[j].r * c[j].r) / (2.0 * c[i].r * d));

            double l = alpha - beta;
            double r = alpha + beta;

            // Normalize angular range into [-PI, PI]
            if (l < -PI) {
                covered.push_back({l + 2 * PI, PI});
                covered.push_back({-PI, r});
            } else if (r > PI) {
                covered.push_back({l, PI});
                covered.push_back({-PI, r - 2 * PI});
            } else {
                covered.push_back({l, r});
            }
        }

        // Sentinel endpoint to complete sweep
        covered.push_back({PI, PI});
        sort(covered.begin(), covered.end());

        // 3. Angular sweep: integrate over uncovered gaps
        double cur_l = -PI;
        for (const auto& interval : covered) {
            if (interval.first > cur_l) {
                total_area += arc_area(c[i].p, c[i].r, cur_l, interval.first);
            }
            cur_l = max(cur_l, interval.second);
        }
    }

    return total_area / 2.0;
}
//================================================================//
// Constants and helper functions
const double EPS = 1e-10;

int sign(double x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}

bool line_intersection(const P &a, const P &dir_a, const P &c, const P &dir_c, P &res) {
    double den = dir_a * dir_c;  // Cross product
    if (std::abs(den) < EPS) return false;  // Lines are parallel
    P ca = c - a;
    double t = (ca * dir_c) / den;
    res = a + dir_a * t;
    return true;
}

struct circle {
    P p;       // Center
    double r;  // Radius

    circle() : p(P{0, 0}), r(0) {}
    circle(P _p, double _r) : p(_p), r(_r) {}

    // Circumcircle of three points a, b, c
    circle(const P &a, const P &b, const P &c) {
        P ab = b - a;
        P ac = c - a;
        double cross = ab * ac;  // Cross product to check collinearity
        if (std::abs(cross) < EPS) {  // Points are collinear
            double d1 = a.dist(b);
            double d2 = a.dist(c);
            double d3 = b.dist(c);
            if (d1 >= d2 && d1 >= d3) {
                p = (a + b) / 2.0;
                r = d1 / 2.0;
            } else if (d2 >= d1 && d2 >= d3) {
                p = (a + c) / 2.0;
                r = d2 / 2.0;
            } else {
                p = (b + c) / 2.0;
                r = d3 / 2.0;
            }
        } else {
            // Compute circumcenter using perpendicular bisectors
            P ab_mid = (a + b) / 2.0;
            P ac_mid = (a + c) / 2.0;
            P ab_perp = ab.rot90();
            P ac_perp = ac.rot90();
            bool ok = line_intersection(ab_mid, ab_perp, ac_mid, ac_perp, p);
            if (!ok) {
                // Fallback for numerical stability
                p = (a + b + c) / 3.0;
                r = std::max({a.dist(p), b.dist(p), c.dist(p)});
            } else {
                r = p.dist(a);  // Radius is distance from center to any vertex
            }
        }
    }
};

circle minimum_enclosing_circle(std::vector<P> &p) {
    // Random shuffle to improve average-case performance
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::shuffle(p.begin(), p.end(), rng);

    int n = p.size();
    if (n == 0) return circle();
    if (n == 1) return circle(p[0], 0);

    // Initialize circle with the first point
    circle c(p[0], 0);

    // Iterate through all points
    for (int i = 1; i < n; i++) {
        if (sign(p[i].dist(c.p) - c.r) > 0) {  // p[i] is outside current circle
            c = circle(p[i], 0);  // Start new circle with p[i]
            for (int j = 0; j < i; j++) {
                if (sign(p[j].dist(c.p) - c.r) > 0) {  // p[j] is outside
                    c = circle((p[i] + p[j]) / 2.0, p[i].dist(p[j]) / 2.0);  // Circle with diameter p[i] to p[j]
                    for (int k = 0; k < j; k++) {
                        if (sign(p[k].dist(c.p) - c.r) > 0) {  // p[k] is outside
                            c = circle(p[i], p[j], p[k]);  // Circumcircle of p[i], p[j], p[k]
                        }
                    }
                }
            }
        }
    }
    return c;
}
