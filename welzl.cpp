double EPS=1e-9;
int sign(double x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}
struct Point {
    double x, y;
    Point() {}
    Point(double _x, double _y) : x(_x), y(_y) {}
 
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(double k) const { return Point(x * k, y * k); }
    Point operator/(double k) const { return Point(x / k, y / k); }
 
    double cross(const Point &o) const { return x * o.y - y * o.x; }
    double dist(const Point &o) const {
        double dx = x - o.x, dy = y - o.y;
        return sqrt(dx*dx + dy*dy);
    }
    Point rot90() const { return Point(-y, x); }
};
 
// intersect line a→b with c→d, return intersection in res
bool line_intersection(const Point &a, const Point &b, 
                       const Point &c, const Point &d, 
                       Point &res) {
    Point ab = b - a, cd = d - c;
    double det = ab.cross(cd);
    if (fabs(det) < EPS) return false;
    double t = (c - a).cross(cd) / det;
    res = a + ab * t;
    return true;
}
 
struct circle {
    Point p;   // center
    double r;  // radius
    circle() : p(Point(0,0)), r(0) {}
    circle(Point _p, double _r) : p(_p), r(_r) {}
 
    // circumcircle of a,b,c
    circle(const Point &a, const Point &b, const Point &c) {
        Point ab_mid = (a + b) / 2.0;
        Point ac_mid = (a + c) / 2.0;
        Point ab_perp = (b - a).rot90();
        Point ac_perp = (c - a).rot90();
 
        bool ok = line_intersection(ab_mid, ab_mid + ab_perp,
                                    ac_mid, ac_mid + ac_perp, p);
        assert(ok);  // assume non‐collinear
        r = p.dist(a);
    }
};
 
// base‐case helper: build the unique circle through up to 3 boundary points
circle make_circle(const vector<Point> &R) {
    if (R.empty()) 
        return circle(Point(0,0), 0);
    else if (R.size() == 1)
        return circle(R[0], 0);
    else if (R.size() == 2)
        return circle((R[0] + R[1]) / 2.0, R[0].dist(R[1]) / 2.0);
    else
        return circle(R[0], R[1], R[2]);
}
 
// check if p is inside (or on) circle c
bool in_circle(const Point &p, const circle &c) {
    return sign(p.dist(c.p) - c.r) <= 0;
}
 
// Welzl’s recursive algorithm
circle welzl(vector<Point> &P, vector<Point> R = {}, int n = -1) {
    if (n < 0) n = (int)P.size();
    if (n == 0 || R.size() == 3)
        return make_circle(R);
 
    // pick P[n-1] at random (we shuffle before calling welzl)
    Point p = P[n-1];
    circle D = welzl(P, R, n-1);
 
    if (in_circle(p, D))
        return D;
 
    R.push_back(p);
    return welzl(P, R, n-1);
}
 
// wrapper: shuffle + call
circle minimum_enclosing_circle(vector<Point> pts) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(pts.begin(), pts.end(), rng);
    return welzl(pts);
}
