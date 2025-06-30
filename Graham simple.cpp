//Return idx of the point in original vector
using T = int;
using P = pair<T, T>;
using vP = vector<P>;
using vi = vector<int>;
T cross(const P& a, const P& b) {
    return a.first * b.second - a.second * b.first;
}

T cross(const P& o, const P& a, const P& b) {
    return cross({a.first - o.first, a.second - o.second}, 
                 {b.first - o.first, b.second - o.second});
}

T abs2(const P& p) {
    return p.first * p.first + p.second * p.second;
}

vector<int> grahamScanidx(const vector<pair<int,int>>& v) {
    int n = v.size();
    int pivot = (min_element(all(v)) - v.begin());
    vi cand, hull{pivot};
    
    for (int i = 0; i < n; i++) {
        if (v[i] != v[pivot]) cand.push_back(i);
    }

    sort(all(cand), [&](int a, int b) {
        P x = {v[a].first - v[pivot].first, v[a].second - v[pivot].second};
        P y = {v[b].first - v[pivot].first, v[b].second - v[pivot].second};
        T t = cross(x, y);
        return t != 0 ? t > 0 : abs2(x) < abs2(y);
    });

    for (auto c : cand) {
        while (hull.size() > 1 && cross(v[hull[hull.size()-2]], v[hull.back()], v[c]) <= 0)
            hull.pop_back();
        hull.push_back(c);
    }

    return hull;
}
