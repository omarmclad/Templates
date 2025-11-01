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
long double PI=acos(-1LL);
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
    double area() { return PI * r * r; }
    bool operator==(const circle &o) const {
        constexpr double EPS = 1e-12;
        return p == o.p && fabs(r - o.r) < EPS;
    }
};
//5 - outside and do not intersect
//4 - intersect outside in one point
//3 - intersect in 2 points
//2 - intersect inside in one point
//1 - inside and do not intersect
int circle_circle_relation(P a, double r, P b, double R) {
    double d = a.dist(b);
    if (sign(d - r - R) > 0)  return 5;
    if (sign(d - r - R) == 0) return 4;
    double l = fabs(r - R);
    if (sign(d - r - R) < 0 && sign(d - l) > 0) return 3;
    if (sign(d - l) == 0) return 2;
    if (sign(d - l) < 0) return 1;
    assert(0); return -1;
}
double circle_circle_area(P a, double r1, P b, double r2) {
    double d = (a - b).length();
    if(r1 + r2 < d + EPS) return 0;
    if(r1 + d < r2 + EPS) return PI * r1 * r1;
    if(r2 + d < r1 + EPS) return PI * r2 * r2;
    double theta_1 = acos((r1 * r1 + d * d - r2 * r2) / (2 * r1 * d)), 
    	theta_2 = acos((r2 * r2 + d * d - r1 * r1)/(2 * r2 * d));
    return r1 * r1 * (theta_1 - sin(2 * theta_1)/2.) + r2 * r2 * (theta_2 - sin(2 * theta_2)/2.);
}
