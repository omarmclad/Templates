double polygonArea(const vector<P> &poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i)
        area += poly[i] * poly[(i + 1) % n];
    return fabs(area) / 2.0;
}
bool intersectSegments(P a, P b, P c, P d, P &intersect) {
    double d1 = (b - a) * (d - c);  // cross(b-a, d-c)
    double d2 = (c - a) * (d - c);
    double d3 = (b - a) * (c - a);

    if (fabs(d1) < 1e-9) return false; // Parallel

    double t1 = d2 / d1;
    double t2 = d3 / d1;
    intersect = a + (b - a) * t1;

    if (t1 < -1e-9 || t1 > 1 + 1e-9 || t2 < -1e-9 || t2 > 1 + 1e-9) return false;

    return true;
}
vector<P> polygonCut(const vector<P> &poly, P a, P b) {
    vector<P> result;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        P curr = poly[i];
        P next = poly[(i + 1) % n];
        double side_curr = (b - a) * (curr - a);
        double side_next = (b - a) * (next - a);
        bool inside_curr = side_curr > 1e-9;
        bool inside_next = side_next > 1e-9;

        if (inside_curr) result.push_back(curr);
        if (inside_curr != inside_next) {
            P r;
            intersectSegments(a, b, curr, next, r);
            result.push_back(r);
        }
    }
    return result;
}
vector<P> clipPolygon(const vector<P> &subject, const vector<P> &clipper) {
    vector<P> result = subject;
    int n = clipper.size();
    for (int i = 0; i < n; ++i) {
        P a = clipper[i];
        P b = clipper[(i + 1) % n];
        result = polygonCut(result, a, b);  // cut against one edge
    }
    return result;
}
// Returns:
//  0 => outside
//  1 => on the edge
//  2 => inside
int point_in_polygon(const vector<P> &poly, const P &pt) {
    int n = poly.size();
    bool inside = false;
    
    for (int i = 0; i < n; ++i) {
        P a = poly[i], b = poly[(i + 1) % n];

        // Check if point lies exactly on the edge (segment)
        P ab = b - a;
        P ap = pt - a;
        P bp = pt - b;
        if (fabs(ab * ap) < 1e-9 && (ap & ab) >= 0 && (bp & ab) <= 0)
            return 1;  // On edge

        // Ray casting: check for crossing horizontal ray to the right
        if ((a.y > pt.y) != (b.y > pt.y)) {
            double x_intersect = a.x + (b.x - a.x) * (pt.y - a.y) / (b.y - a.y);
            if (x_intersect > pt.x)
                inside = !inside;
        }
    }
    return inside ? 2 : 0;
}
