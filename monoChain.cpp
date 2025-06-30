vector<P> convexHull(vector<P> pts) {
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    int n = pts.size();
    if (n <= 2) return pts;
    vector<P> hull;
    // Lower hull
    for (const P& p : pts) {
        while (hull.size() >= 2 &&
               hull[hull.size() - 2].triangle(hull.back(), p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    // Upper hull
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while ((int)hull.size() > lower_size &&
               hull[hull.size() - 2].triangle(hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back();  // Last point equals first one
    return hull;
}
