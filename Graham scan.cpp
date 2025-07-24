vector<P> grahamScan(vector<P> pts) {
    int n = pts.size();
    if (n <= 1) return pts;

    // Remove duplicates
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());

    // Step 1: find the pivot (lowest y, then x)
    int pivot = 0;
    for (int i = 1; i < pts.size(); ++i) {
        if (make_pair(pts[i].y, pts[i].x) < make_pair(pts[pivot].y, pts[pivot].x))
            pivot = i;
    }
    swap(pts[0], pts[pivot]);
    P base = pts[0];

    // Step 2: sort by polar angle around the pivot
    sort(pts.begin() + 1, pts.end(), [&](const P& a, const P& b) {
        int cp = base.triangle(a, b);
        if (cp == 0) return base.dist(a) < base.dist(b); // closer first
        return cp > 0; // left turn preferred
    });

    // Step 3: Graham scan stack
    vector<P> hull;
    for (const P& pt : pts) {
        while (hull.size() >= 2 &&
               hull[hull.size() - 2].triangle(hull.back(), pt) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pt);
    }

    return hull;
}
double rotatingCalipersDiameter(const vector<P>& pts) {
    vector<P> hull = grahamScan(pts);
    int n = hull.size();
    if (n == 1) return 0;
    if (n == 2) return hull[0].dist(hull[1]);

    double maxDist = 0;
    int j = 1;

    for (int i = 0; i < n; i++) {
        // Move j while distance increases
        while ((hull[i].dist(hull[(j + 1) % n])) > (hull[i].dist(hull[j]))) {
            j = (j + 1) % n;
        }

        maxDist = max(maxDist, hull[i].dist(hull[j]));
    }

    return maxDist;
}
double rotatingCalipersMinWidth(const vector<P>& pts) {
    vector<P> hull = grahamScan(pts);
    int n = hull.size();
    if (n <= 2) return 0;

    double minWidth = 1e18;
    int j = 1;

    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;

        // Move j while height increases
        while ((abs(hull[i].triangle(hull[ni], hull[(j + 1) % n]))) >
               (abs(hull[i].triangle(hull[ni], hull[j])))) {
            j = (j + 1) % n;
        }

        // Once j is optimal, measure height
        double height = hull[j].dist_to_line(hull[i], hull[ni]);
        minWidth = min(minWidth, height);
    }

    return minWidth;
}
