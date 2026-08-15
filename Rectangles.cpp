#include <bits/stdc++.h>
using namespace std;

using pt = pair<long long, long long>;
#define x first
#define y second

// Axis-Aligned Rectangle defined by Bottom-Left (bl) and Top-Right (tr)
struct Rect {
    pt bl, tr;
};

// 1. Area of a single rectangle. O(1)
long long area(const Rect &r) {
    long long width = max(0LL, r.tr.x - r.bl.x);
    long long height = max(0LL, r.tr.y - r.bl.y);
    return width * height;
}

// 2. Overlapping Intersection Rectangle of R1 and R2. O(1)
// Returns a degenerate rectangle {{0, 0}, {0, 0}} if there is no overlap.
Rect intersectRect(const Rect &r1, const Rect &r2) {
    long long x1 = max(r1.bl.x, r2.bl.x);
    long long y1 = max(r1.bl.y, r2.bl.y);
    long long x2 = min(r1.tr.x, r2.tr.x);
    long long y2 = min(r1.tr.y, r2.tr.y);

    if (x1 >= x2 || y1 >= y2) return {{0, 0}, {0, 0}}; // No overlap
    return {{x1, y1}, {x2, y2}};
}

// 3. Total Union Area of 2 Rectangles. O(1)
long long unionArea2(const Rect &r1, const Rect &r2) {
    return area(r1) + area(r2) - area(intersectRect(r1, r2));
}

// 4. Minimum Bounding Box enclosing a set of rectangles. O(N)
Rect boundingBox(const vector<Rect> &rects) {
    if (rects.empty()) return {{0, 0}, {0, 0}};
    long long min_x = rects[0].bl.x, min_y = rects[0].bl.y;
    long long max_x = rects[0].tr.x, max_y = rects[0].tr.y;
    for (const auto &r : rects) {
        min_x = min(min_x, r.bl.x);
        min_y = min(min_y, r.bl.y);
        max_x = max(max_x, r.tr.x);
        max_y = max(max_y, r.tr.y);
    }
    return {{min_x, min_y}, {max_x, max_y}};
}

// 5. Point Containment Test. O(1)
bool containsPoint(const Rect &r, const pt &p) {
    return p.x >= r.bl.x && p.x <= r.tr.x && p.y >= r.bl.y && p.y <= r.tr.y;
}

// 6. Check if R1 is strictly inside R2. O(1)
bool containsRect(const Rect &r2, const Rect &r1) {
    return r1.bl.x >= r2.bl.x && r1.tr.x <= r2.tr.x &&
           r1.bl.y >= r2.bl.y && r1.tr.y <= r2.tr.y;
}
