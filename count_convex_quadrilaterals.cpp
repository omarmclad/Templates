int count_convex_quadrilaterals(vector<P> &v) {
    int n = v.size();
    int c = 0;
    
    // For each point as pivot
    for (int i = 0; i < n; i++) {
        vector<P> hold;
        // Translate points relative to pivot
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            hold.push_back(v[j] - v[i]);
        }
        
        // Sort by polar angle
        O = {0, 0};
        sortx(hold);
        
        int sz = hold.size();
        // Duplicate points for circular sweep
        hold.insert(hold.end(), hold.begin(), hold.begin() + sz);
        
        // Sweep to count convex quadrilaterals
        int k = 0;
        for (int j = 0; j < sz; j++) {
            k = max(k, j + 1);
            // Find points in same or next half-plane (convex condition)
            while (k < j + sz && (hold[j] * hold[k] > 0 || (hold[j] * hold[k] == 0 && (hold[j] & hold[k]) > 0))) {
                k++;
            }
            int cnt = k - j - 1; // Number of points in valid sector
            c += (cnt * (cnt - 1)) / 2; // Choose 2 points for quadrilateral
        }
    }
    // Each quadrilateral is counted 4 times (once per vertex)
    return c / 4;
}
