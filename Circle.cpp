const double EPS = 1e-9;

int sign(double x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}
bool line_intersection(P a, P b, P c, P d, P &res) {
    P ab = b - a, cd = d - c;
    double det = ab * cd;
    if (abs(det) < 1e-12) return false; // Parallel or collinear
    double t = ((c - a) * cd) / det;
    res = a + ab * t;
    return true;
}

struct circle {
    P p; // center
    double r;

    circle() {}
    circle(P p, double r) : p(p), r(r) {}

    // Construct circle through three points
    circle(P a, P b, P c) {
        P ab_mid = (a + b) / 2.0;
        P ac_mid = (a + c) / 2.0;

        P ab_perp = (b - a).rot90();
        P ac_perp = (c - a).rot90();

        bool ok = line_intersection(ab_mid, ab_mid + ab_perp, ac_mid, ac_mid + ac_perp, p);
        assert(ok);
        r = p.dist(a);
    }

    bool operator==(const circle &o) const {
        constexpr double EPS = 1e-12;
        return p == o.p && fabs(r - o.r) < EPS;
    }
};
// given n points, find the minimum enclosing circle of the points
// call convex_hull() before this for faster solution
// expected O(n)
// use those 
// #include <random>    // for mt19937, random_device, shuffle
// #include <chrono>    // for steady_clock
circle minimum_enclosing_circle(vector<P> &p) {
    //–– set up RNG
    static std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );

    std::shuffle(p.begin(), p.end(), rng);
    int n = p.size();
    circle c(p[0], 0);

    for (int i = 1; i < n; i++) {
        if (sign(p[i].dist(c.p) - c.r) > 0) {
            c = circle(p[i], 0);
            for (int j = 0; j < i; j++) {
                if (sign(p[j].dist(c.p) - c.r) > 0) {
                    c = circle((p[i] + p[j]) / 2.0, p[i].dist(p[j]) / 2.0);
                    for (int k = 0; k < j; k++) {
                        if (sign(p[k].dist(c.p) - c.r) > 0) {
                            c = circle(p[i], p[j], p[k]);
                        }
                    }
                }
            }
        }
    }

    return c;
}

