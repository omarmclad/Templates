int pointLocationTest(int x1,int y1,int x2,int y2,int x3,int y3){
    int val = (y2 - y1)*(x3 - x1) - (y3 - y1)*(x2 - x1);
    if(val == 0) return 0;
    return (val > 0 ? 1 : -1);
}
bool onSegment(int x1, int y1, int x2, int y2, int x, int y) {
    return min(x1,x2) <= x && x <= max(x1,x2) &&
           min(y1,y2) <= y && y <= max(y1,y2);
}
bool checkIntersection(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
    int d1 = pointLocationTest(x1,y1,x2,y2,x3,y3);
    int d2 = pointLocationTest(x1,y1,x2,y2,x4,y4);
    int d3 = pointLocationTest(x3,y3,x4,y4,x1,y1);
    int d4 = pointLocationTest(x3,y3,x4,y4,x2,y2);
    if(d1*d2 < 0 && d3*d4 < 0) return true;
    if(d1 == 0 && onSegment(x1,y1,x2,y2,x3,y3)) return true;
    if(d2 == 0 && onSegment(x1,y1,x2,y2,x4,y4)) return true;
    if(d3 == 0 && onSegment(x3,y3,x4,y4,x1,y1)) return true;
    if(d4 == 0 && onSegment(x3,y3,x4,y4,x2,y2)) return true;

    return false;
}
// Function to output the result of intersection
void segmentIntersectionOutput(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
    int d1 = pointLocationTest(x1,y1,x2,y2,x3,y3);
    int d2 = pointLocationTest(x1,y1,x2,y2,x4,y4);
    int d3 = pointLocationTest(x3,y3,x4,y4,x1,y1);
    int d4 = pointLocationTest(x3,y3,x4,y4,x2,y2);

    // General intersection (not collinear)
    if(d1 * d2 < 0 && d3 * d4 < 0) {
        // Convert to floating point if you want precise intersection
        double A1 = y2 - y1, B1 = x1 - x2, C1 = A1 * x1 + B1 * y1;
        double A2 = y4 - y3, B2 = x3 - x4, C2 = A2 * x3 + B2 * y3;
        double det = A1 * B2 - A2 * B1;

        if(abs(det) < 1e-9) {
            cout << "-1\n"; // Lines are parallel and non-intersecting
            return;
        }

        double ix = (B2 * C1 - B1 * C2) / det;
        double iy = (A1 * C2 - A2 * C1) / det;

        cout << fixed << setprecision(10) << ix << ' ' << iy << '\n';
        return;
    }
    // Collinear cases — check point-wise
    vector<pair<int,int>> points;

    if(d1 == 0 && onSegment(x1,y1,x2,y2,x3,y3)) points.emplace_back(x3,y3);
    if(d2 == 0 && onSegment(x1,y1,x2,y2,x4,y4)) points.emplace_back(x4,y4);
    if(d3 == 0 && onSegment(x3,y3,x4,y4,x1,y1)) points.emplace_back(x1,y1);
    if(d4 == 0 && onSegment(x3,y3,x4,y4,x2,y2)) points.emplace_back(x2,y2);

    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if(points.empty()) {
        cout << "-1\n";
    }
    else if(points.size() == 1) {
        cout << points[0].first << ' ' << points[0].second << '\n';
    }
    else {
        // Return the overlapping segment as two points in sorted order
        pair<int,int> a = points.front();
        pair<int,int> b = points.back();
        if (a > b) swap(a, b);
        cout << a.first << ' ' << a.second << ' ' << b.first << ' ' << b.second << '\n';
    }
}
