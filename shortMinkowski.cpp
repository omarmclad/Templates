//A and B are already convex hulls
//A and B have consistent CCW ordering
//A.size() >= 3 and B.size() >= 3

void reorder_poly(vector<Pt>& P) {
    int pos = 0;
    for (int i = 1; i < (int)P.size(); i++) {
        if (P[i].y < P[pos].y ||
            (abs(P[i].y - P[pos].y) < EPS && P[i].x < P[pos].x))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

vector<Pt> minkowski_sum(vector<Pt> A, vector<Pt> B) {
    if (A.empty()) return B;
    if (B.empty()) return A;

    if (A.size() <= 2 || B.size() <= 2) {
        vector<Pt> res;
        for (auto a : A)
            for (auto b : B)
                res.push_back(a + b);
        return convex_hull(res);
    }

    reorder_poly(A);
    reorder_poly(B);

    int n = A.size(), m = B.size();

    A.push_back(A[0]);
    A.push_back(A[1]);

    B.push_back(B[0]);
    B.push_back(B[1]);

    vector<Pt> res;

    int i = 0, j = 0;

    while (i < n || j < m) {
        res.push_back(A[i] + B[j]);

        if (i == n) {
            j++;
        }
        else if (j == m) {
            i++;
        }
        else {
            double crs =
                cross(A[i + 1] - A[i],
                      B[j + 1] - B[j]);

            if (crs >= -EPS) i++;
            if (crs <= EPS) j++;
        }
    }

    return res;
}
