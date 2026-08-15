// Gemini Enhanced code
using Pt = pair<double, double>;
#define x first
#define y second

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Precision & Math Utilities
int sign(double val) { return (val > EPS) - (val < -EPS); }
double safe_acos(double val) { return acos(max(-1.0, min(1.0, val))); }

// Pair Vector Operators
Pt operator+(Pt a, Pt b) { return {a.x + b.x, a.y + b.y}; }
Pt operator-(Pt a, Pt b) { return {a.x - b.x, a.y - b.y}; }
Pt operator*(Pt a, double k) { return {a.x * k, a.y * k}; }
Pt operator/(Pt a, double k) { return {a.x / k, a.y / k}; }

double cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }
double dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }
double norm(Pt a) { return dot(a, a); }
double len(Pt a) { return sqrt(norm(a)); }
double dist(Pt a, Pt b) { return len(a - b); }
Pt rot90(Pt a) { return {-a.y, a.x}; }

// Line Intersection
bool line_intersection(Pt a, Pt b, Pt c, Pt d, Pt &res) {
    Pt ab = b - a, cd = d - c;
    double det = cross(ab, cd);
    if (abs(det) < EPS) return false;
    double t = cross(c - a, cd) / det;
    res = a + ab * t;
    return true;
}

struct circle {
    Pt p;
    double r;
    circle() : p({0, 0}), r(0) {}
    circle(Pt p, double r) : p(p), r(r) {}
};

// Relation: 0 (Identical), 1 (Inside), 2 (Int-Tangent), 3 (2-Points), 4 (Ext-Tangent), 5 (Outside)
int circle_circle_relation(Pt a, double r, Pt b, double R) {
    double d = dist(a, b);
    if (sign(d) == 0 && sign(r - R) == 0) return 0;
    if (sign(d - r - R) > 0)  return 5;
    if (sign(d - r - R) == 0) return 4;
    double l = fabs(r - R);
    if (sign(d - r - R) < 0 && sign(d - l) > 0) return 3;
    if (sign(d - l) == 0) return 2;
    if (sign(d - l) < 0) return 1;
    return -1;
}

double circle_circle_area(Pt a, double r1, Pt b, double r2) {
    double d = dist(a, b);
    if (r1 + r2 <= d + EPS) return 0;
    if (r1 + d <= r2 + EPS) return PI * r1 * r1;
    if (r2 + d <= r1 + EPS) return PI * r2 * r2;

    double t1 = safe_acos((r1 * r1 + d * d - r2 * r2) / (2.0 * r1 * d));
    double t2 = safe_acos((r2 * r2 + d * d - r1 * r1) / (2.0 * r2 * d));

    return r1 * r1 * (t1 - sin(2.0 * t1) / 2.0) +
           r2 * r2 * (t2 - sin(2.0 * t2) / 2.0);
}

vector<Pt> circle_circle_intersection(Pt a, double r1, Pt b, double r2) {
    int rel = circle_circle_relation(a, r1, b, r2);
    if (rel == 0 || rel == 1 || rel == 5) return {};
    double d = dist(a, b);
    double u = (r1 * r1 - r2 * r2 + d * d) / (2.0 * d);
    double h = sqrt(max(0.0, r1 * r1 - u * u));
    Pt v = (b - a) / d;
    Pt p_mid = a + v * u;
    if (rel == 2 || rel == 4) return {p_mid};
    Pt rot = rot90(v) * h;
    return {p_mid + rot, p_mid - rot};
}
//========================================================================================//
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
