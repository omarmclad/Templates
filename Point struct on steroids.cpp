struct P
{
    double x, y;

    void input() {
        cin >> x >> y;
    }
    // Vector subtraction: this - b
    P operator-(const P &b) const {
        return {x - b.x, y - b.y};
    }

    // Subtract and assign: this -= b
    void operator-=(const P &b) {
        x -= b.x;
        y -= b.y;
    }

    // Vector addition: this + b
    P operator+(const P &b) const {
        return {x + b.x, y + b.y};
    }

    // Add and assign: this += b
    void operator+=(const P &b) {
        x += b.x;
        y += b.y;
    }

    // Scalar multiplication: this * k
    P operator*(double k) const {
        return {x * k, y * k};
    }

    // Scalar division: this / k (⚠️ Integer division)
    P operator/(double k) const {
        return {x / k, y / k};
    }

    // 2D Cross product: this × b = x*b.y - y*b.x
    // Geometric meaning: area of parallelogram formed by this and b (signed)
    double operator*(const P &b) const {
        return x * b.y - y * b.x;
    }

    // Dot product: this • b = x*b.x + y*b.y
    // Geometric meaning: projection component, used for angle and length
    double operator&(const P &b) const {
        return x * b.x + y * b.y;
    }

    // Signed double area of triangle (this, b, c)
    // Formula: (b - this) × (c - this)
    double triangle(const P &b, const P &c) const {
        return (b - *this) * (c - *this);
    }

    // Euclidean distance between two points: √[(x1 - x2)² + (y1 - y2)²]
    double dist(const P &b) const {
        return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
    }

    // Squared length (norm²) of vector: x² + y²
    double length2() const {
        return x * x + y * y;
    }

    // Length (magnitude) of vector: √(x² + y²)
    double length() const {
        return sqrt(length2());
    }

    // Unit vector: (x / |v|, y / |v|), direction with length = 1
    P unit() const {
        double len = length();
        return {x / len, y / len};
    }

    // Projection of this onto vector b
    // Formula: proj_b(this) = b * [(this • b) / |b|²]
    P project_onto(const P &b) const {
        double dot = (*this & b);
        double len2 = b.length2();
        return b * (dot / (double)len2);
    }

    // Distance from point to line through points a and b
    // Formula: |(b - a) × (this - a)| / |b - a|
    // Geometric meaning: height from point to line
    //1. Distance from *this to infinite line AB
    double dist_to_line(const P &a, const P &b) const {
        P ab = b - a;
        P ap = *this - a;
        return abs((ab * ap)) / ab.length();
    }
    // 2) Distance to ray AB (starts at A, goes through B):
    double dist_to_ray(const P &a, const P &b) const {
        P ab = b - a, ap = *this - a;
        double ab2 = ab.length2();
        double u   = (ap & ab) / ab2;
        if (u < 0.0) {
            // if projection falls "behind" A, closest point is A
            return ap.length();
        }
        // else drop perpendicular
        return fabs(ab * ap) / ab.length();
    }
    // 3) Distance to segment AB (finite [A,B]):
    double dist_to_segment(const P &a, const P &b) const {
        P ab = b - a, ap = *this - a;
        double ab2 = ab.length2();
        double u   = (ap & ab) / ab2;
        if (u < 0.0) {
            // before A
            return ap.length();
        }
        else if (u > 1.0) {
            // beyond B
            return (*this - b).length();
        }
        // between A and B
        return fabs(ab * ap) / ab.length();
    }
    // Distance between two segments AB and CD
    // to use instead of std::
    // P::dist_to_segment(P a, P b, P c, P d);
    static double dist_segments(const P &a, const P &b, const P &c, const P &d) {
        // Check if segments intersect (cross product test)
        auto ccw = [](const P &a, const P &b, const P &c) {
            return (b - a) * (c - a) > 0;
        };
        bool intersect = (ccw(a, b, c) != ccw(a, b, d)) && (ccw(c, d, a) != ccw(c, d, b));
        if (intersect) return 0.0;

        // Else, return min of 4 point-to-segment distances
        return min({
            c.dist_to_segment(a, b),
            d.dist_to_segment(a, b),
            a.dist_to_segment(c, d),
            b.dist_to_segment(c, d)
        });
    }
    // Distance between two rays AB and CD
    static double dist_rays(const P &A, const P &B, const P &C, const P &D) {
        const double EPS = 1e-9;
        P AB = B - A;
        P CD = D - C;

        if (AB.length2() < EPS && CD.length2() < EPS)
            return A.dist(C);
        if (AB.length2() < EPS)
            return A.dist_to_ray(C, D);
        if (CD.length2() < EPS)
            return C.dist_to_ray(A, B);

        double cross = AB * CD;
        P AC = C - A;

        if (fabs(cross) > EPS) {
            double t = (AC * CD) / cross;
            double s = (AC * AB) / cross;
            if (t >= -EPS && s >= -EPS) return 0.0;
        } else {
            // Parallel or collinear
            if (fabs((C - A) * CD) < EPS) {
                // Collinear
                double proj1 = ((C - A) & AB) / AB.length2();
                double proj2 = ((A - C) & CD) / CD.length2();
                if (proj1 >= -EPS || proj2 >= -EPS) return 0.0;
            }
        }

        // No valid intersection; get min point-to-ray distance
        return min({
            A.dist_to_ray(C, D),
            C.dist_to_ray(A, B)
        });
    }
    double angle_with(const P &b) const 
    {
        return acos((*this & b) / (length() * b.length()));
    }
    // Point equality: returns true if (x, y) == (b.x, b.y)
    bool operator==(const P &b) const {
        return x == b.x && y == b.y;
    }

    // Lexicographical comparison for sorting (by x, then y)
    bool operator<(const P &b) const {
        return tie(x, y) < tie(b.x, b.y);
    }
};
long double pi=acos(-1L);
double eps=1e-9;
void VoidVortexOfNothingness(){
    P p1,p2,p3,p4;
    p1.input();
    p2.input();
    p3.input();
    p4.input();
    cout<<fx(7)<<P::dist_rays(p1,p2,p3,p4)<<endl;
}
signed main()
{
    //freopen("mex.in", "r", stdin);
    // freopen("distance2.in", "r", stdin);
    // freopen("distance2.out", "w", stdout);
   francesco_bernoulli
   int t=1;
    // cin>>t;
   while(t--){
   VoidVortexOfNothingness();
   }

    return 0;
}
//CIAO_McQUEEN//
