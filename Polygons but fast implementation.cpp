using pt = pair<double, double>;

pt add(const pt &a, const pt &b) {
    return {a.first + b.first, a.second + b.second};
}

pt sub(const pt &a, const pt &b) {
    return {a.first - b.first, a.second - b.second};
}

pt mul(const pt &a, double k) {
    return {a.first * k, a.second * k};
}

double dot(const pt &a, const pt &b) {
    return a.first * b.first + a.second * b.second;
}

double cross(const pt &a, const pt &b) {
    return a.first * b.second - a.second * b.first;
}

double cross(const pt &a, const pt &b, const pt &c) {
    return cross(sub(b, a), sub(c, a));
}

double polygonArea(const vector<pt> &poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i)
        area += cross(poly[i], poly[(i + 1) % n]);
    return fabs(area) / 2.0;
}

bool intersectSegments(pt a, pt b, pt c, pt d, pt &intersect) {
    pt ab = sub(b, a), cd = sub(d, c);
    double d1 = cross(ab, cd);
    if (fabs(d1) < 1e-9) return false;
    double d2 = cross(sub(c, a), cd);
    double d3 = cross(ab, sub(c, a));
    double t1 = d2 / d1;
    double t2 = d3 / d1;
    if (t1 < -1e-9 || t1 > 1 + 1e-9 || t2 < -1e-9 || t2 > 1 + 1e-9) return false;
    intersect = add(a, mul(ab, t1));
    return true;
}

vector<pt> polygonCut(const vector<pt> &poly, pt a, pt b) {
    vector<pt> result;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt curr = poly[i];
        pt next = poly[(i + 1) % n];
        double side_curr = cross(sub(b, a), sub(curr, a));
        double side_next = cross(sub(b, a), sub(next, a));
        bool inside_curr = side_curr > 1e-9;
        bool inside_next = side_next > 1e-9;
        if (inside_curr) result.push_back(curr);
        if (inside_curr != inside_next) {
            pt r;
            if (intersectSegments(a, b, curr, next, r))
                result.push_back(r);
        }
    }
    return result;
}

vector<pt> clipPolygon(const vector<pt> &subject, const vector<pt> &clipper) {
    vector<pt> result = subject;
    int n = clipper.size();
    for (int i = 0; i < n; ++i) {
        pt a = clipper[i];
        pt b = clipper[(i + 1) % n];
        result = polygonCut(result, a, b);
    }
    return result;
}

// 0 = outside, 1 = on edge, 2 = inside
int pointInPolygon(const vector<pt> &poly, const pt &p) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt a = poly[i], b = poly[(i + 1) % n];
        pt ab = sub(b, a), ap = sub(p, a), bp = sub(p, b);
        if (fabs(cross(ab, ap)) < 1e-9 && dot(ap, ab) >= 0 && dot(bp, ab) <= 0)
            return 1;
        if ((a.second > p.second) != (b.second > p.second)) {
            double x_int = a.first + (b.first - a.first) * (p.second - a.second) / (b.second - a.second);
            if (x_int > p.first)
                inside = !inside;
        }
    }
    return inside ? 2 : 0;
}
