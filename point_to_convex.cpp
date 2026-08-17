bool pointInConvexPolygon(const vector<pt> &poly, const pt &p) {
    int n = poly.size();
    if (n < 3) return false;
    
    // Check if p is to the right of ray poly[0]->poly[1] or poly[0]->poly[n-1]
    if (cross(poly[0], poly[1], p) < -1e-9 || cross(poly[0], poly[n - 1], p) > 1e-9)
        return false;

    // Binary search for the wedge containing p
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (cross(poly[0], poly[mid], p) >= -1e-9) l = mid;
        else r = mid;
    }

    // Test if p lies inside the triangle formed by poly[0], poly[l], poly[l+1]
    return cross(poly[l], poly[l + 1], p) >= -1e-9;
}
int orientation(const pt &a, const pt &b, const pt &c) {
    double crs = cross(a, b, c);
    if (crs > 1e-9) return 1;    // CCW
    if (crs < -1e-9) return -1;  // CW
    return 0;                    // Collinear
}
// Shortest Euclidean distance from point p to line segment ab
double distToSegment(const pt &a, const pt &b, const pt &p) {
    pt ab = sub(b, a), ap = sub(p, a), bp = sub(p, b);
    if (dot(ap, ab) <= 0) return sqrt(dist2(p, a));
    if (dot(bp, ab) >= 0) return sqrt(dist2(p, b));
    return fabs(cross(ab, ap)) / sqrt(dist2(a, b));
}
// FIX 3: Replaced flawed binary search with Dan Sunday's O(log N) tangent algorithm.
// Simple binary search fails on cyclic orientation sequences across arbitrary midpoints.
pair<pt, int> pointPolyTangent(const vector<pt> &p, const pt &Q, int dir, int l, int r) {
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        bool pvs = orientation(Q, p[mid], p[mid - 1]) != -dir;
        bool nxt = orientation(Q, p[mid], p[mid + 1]) != -dir;
        if (pvs && nxt) return {p[mid], mid};
        if (!(pvs || nxt)) {
            auto p1 = pointPolyTangent(p, Q, dir, mid + 1, r);
            auto p2 = pointPolyTangent(p, Q, dir, l, mid - 1);
            return orientation(Q, p1.first, p2.first) == dir ? p1 : p2;
        }
        if (!pvs) {
            if (orientation(Q, p[mid], p[l]) == dir) r = mid - 1;
            else if (orientation(Q, p[l], p[r]) == dir) r = mid - 1;
            else l = mid + 1;
        }
        if (!nxt) {
            if (orientation(Q, p[mid], p[l]) == dir) l = mid + 1;
            else if (orientation(Q, p[l], p[r]) == dir) r = mid - 1;
            else l = mid + 1;
        }
    }
    pair<pt, int> ret = {p[l], l};
    for (int i = l + 1; i <= r; i++) {
        if (orientation(Q, ret.first, p[i]) != dir) ret = {p[i], i};
    }
    return ret;
}

pair<int, int> tangentsFromPointToConvexPolygon(const vector<pt> &p, const pt &Q) {
    int ccw = pointPolyTangent(p, Q, 1, 0, (int)p.size() - 1).second;
    int cw = pointPolyTangent(p, Q, -1, 0, (int)p.size() - 1).second;
    return {ccw, cw};
}

double distFromPointToConvexPolygon(const vector<pt> &poly, const pt &z) {
    int n = poly.size();
    if (n == 0) return 0;
    if (n <= 3) {
        double ans = numeric_limits<double>::infinity();
        for (int i = 0; i < n; i++)
            ans = min(ans, distToSegment(poly[i], poly[(i + 1) % n], z));
        return ans;
    }

    auto [r, l] = tangentsFromPointToConvexPolygon(poly, z);
    if (l > r) r += n;

    double ans = numeric_limits<double>::infinity();
    while (l < r) {
        int mid = (l + r) >> 1;
        double left_d2 = dist2(poly[mid % n], z);
        double right_d2 = dist2(poly[(mid + 1) % n], z);
        ans = min({ans, left_d2, right_d2});
        if (left_d2 < right_d2) r = mid;
        else l = mid + 1;
    }

    ans = sqrt(ans);
    ans = min(ans, distToSegment(poly[l % n], poly[(l + 1) % n], z));
    ans = min(ans, distToSegment(poly[l % n], poly[(l - 1 + n) % n], z));
    return ans;
}
