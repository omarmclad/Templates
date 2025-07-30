const double EPS = 1e-9;

int sign(double x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(double k) const { return Point(x * k, y * k); }
    Point operator/(double k) const { return Point(x / k, y / k); }

    double cross(const Point &o) const { return x * o.y - y * o.x; }
    double dist(const Point &o) const {
        double dx = x - o.x, dy = y - o.y;
        return sqrt(dx * dx + dy * dy);
    }
    Point rot90() const { return Point(-y, x); } // 90 deg CCW
};

bool line_intersection(Point a, Point b, Point c, Point d, Point &res) {
    Point ab = b - a, cd = d - c;
    double det = ab.cross(cd);
    if (abs(det) < EPS) return false; // Parallel or collinear
    double t = (c - a).cross(cd) / det;
    res = a + ab * t;
    return true;
}

struct circle {
    Point p; // center
    double r;

    circle() {}
    circle(Point p, double r) : p(p), r(r) {}

    // Construct circle through three points
    circle(Point a, Point b, Point c) {
        Point ab_mid = (a + b) / 2.0;
        Point ac_mid = (a + c) / 2.0;

        Point ab_perp = (b - a).rot90();
        Point ac_perp = (c - a).rot90();

        // bool ok = line_intersection(ab_mid, ab_mid + ab_perp, ac_mid, ac_mid + ac_perp, p);
        // if(!ok){
        //     cout<<-1<<endl;
        //     return;
        // }

        r = p.dist(a);
    }
};
// given n points, find the minimum enclosing circle of the points
// call convex_hull() before this for faster solution
// expected O(n)
// use those 
// #include <random>    // for mt19937, random_device, shuffle
// #include <chrono>    // for steady_clock
circle minimum_enclosing_circle(vector<Point> &p) {
    //–– set up a C++17‑compatible RNG
    static std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    //–– shuffle the points
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
