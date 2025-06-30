using pii = pair<int, int>;

int cross(const pii& O, const pii& A, const pii& B) {
    return (A.first - O.first) * (B.second - O.second) -
           (A.second - O.second) * (B.first - O.first);
}

vector<pii> convexHull(vector<pii> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    int n = pts.size();
    if (n <= 2) return pts;

    vector<pii> hull;

    // Lower hull
    for (const pii& p : pts) {
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull.back(), p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }

    // Upper hull
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while ((int)hull.size() > lower_size &&
               cross(hull[hull.size() - 2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }

    hull.pop_back();  // remove duplicated start point
    return hull;
}
