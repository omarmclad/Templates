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
