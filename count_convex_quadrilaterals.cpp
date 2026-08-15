using Pt = pair<ll, ll>;
#define x first
#define y second

// Cross product of (b - a) x (c - a) using __int128_t to handle coords up to 2e9
ll cross(Pt a, Pt b, Pt c) {
    __int128 val = (__int128)(b.x - a.x) * (c.y - a.y) - (__int128)(b.y - a.y) * (c.x - a.x);
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}

// Combination helper: N choose R
ll nCr(ll n, int r) {
    if (n < r) return 0;
    if (r == 3) return n * (n - 1) * (n - 2) / 6;
    if (r == 4) return n * (n - 1) * (n - 2) * (n - 3) / 24;
    return 0;
}

struct QuadClassification {
    ll total_4tuples = 0;
    ll valid_simple_quads = 0; // total - collinear_3 - collinear_4 (Djoser problem answer)
    ll convex_strict = 0;      // strictly convex (total - concave - collinear_3 - collinear_4)
    ll concave = 0;            // 1 point strictly inside triangle of other 3
    ll collinear_3 = 0;        // 3 points collinear, 1 off-line
    ll collinear_4 = 0;        // All 4 points collinear
};

QuadClassification classify_all_quads(const vector<Pt>& pts) {
    int n = pts.size();
    QuadClassification res;
    if (n < 4) return res;

    res.total_4tuples = nCr(n, 4);

    // ========================================================================
    // 1. COUNT COLLINEAR 4-TUPLES (3-collinear and 4-collinear)
    // ========================================================================
    set<vector<int>> unique_lines;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            vector<int> line_pts;
            for (int k = 0; k < n; k++) {
                if (cross(pts[i], pts[j], pts[k]) == 0) {
                    line_pts.push_back(k);
                }
            }
            if (line_pts.size() >= 3) {
                unique_lines.insert(line_pts);
            }
        }
    }

    for (const auto& line : unique_lines) {
        ll k = line.size();
        res.collinear_4 += nCr(k, 4);
        res.collinear_3 += nCr(k, 3) * (n - k);
    }

    // ========================================================================
    // 2. COUNT CONCAVE 4-TUPLES (Triangles containing 1 point strictly inside)
    // ========================================================================
    vector<vector<bitset<300>>> left_of(n, vector<bitset<300>>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            for (int k = 0; k < n; k++) {
                if (cross(pts[i], pts[j], pts[k]) > 0) {
                    left_of[i][j].set(k);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                int a = i, b = j, c = k;
                if (cross(pts[a], pts[b], pts[c]) < 0) swap(b, c); // Ensure CCW
                if (cross(pts[a], pts[b], pts[c]) == 0) continue; // Skip collinear base

                int inside_count = (left_of[a][b] & left_of[b][c] & left_of[c][a]).count();
                res.concave += inside_count;
            }
        }
    }

    // ========================================================================
    // 3. DERIVE METRICS
    // ========================================================================
    res.valid_simple_quads = res.total_4tuples - res.collinear_3 - res.collinear_4;
    res.convex_strict = res.total_4tuples - res.concave - res.collinear_3 - res.collinear_4;

    return res;
}
