struct P
{
    int x, y;

    void input() {
        cin >> x >> y;
    }

    // Vector subtraction: this - b
    P operator-(const P &b) const {
        return {x - b.x, y - b.y};
    }

    // Subtract and assign: this -= b
    void operator-=(const P &b) {
        x -= b.x;
        y -= b.y;
    }

    // Vector addition: this + b
    P operator+(const P &b) const {
        return {x + b.x, y + b.y};
    }

    // Add and assign: this += b
    void operator+=(const P &b) {
        x += b.x;
        y += b.y;
    }

    // Scalar multiplication: this * k
    P operator*(int k) const {
        return {x * k, y * k};
    }

    // Scalar division: this / k (⚠️ Integer division)
    P operator/(int k) const {
        return {x / k, y / k};
    }

    // 2D Cross product: this × b = x*b.y - y*b.x
    // Geometric meaning: area of parallelogram formed by this and b (signed)
    int operator*(const P &b) const {
        return x * b.y - y * b.x;
    }

    // Dot product: this • b = x*b.x + y*b.y
    // Geometric meaning: projection component, used for angle and length
    int operator&(const P &b) const {
        return x * b.x + y * b.y;
    }

    // Signed double area of triangle (this, b, c)
    // Formula: (b - this) × (c - this)
    int triangle(const P &b, const P &c) const {
        return (b - *this) * (c - *this);
    }

    // Euclidean distance between two points: √[(x1 - x2)² + (y1 - y2)²]
    double dist(const P &b) const {
        return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
    }

    // Squared length (norm²) of vector: x² + y²
    int length2() const {
        return x * x + y * y;
    }

    // Length (magnitude) of vector: √(x² + y²)
    double length() const {
        return sqrt(length2());
    }

    // Unit vector: (x / |v|, y / |v|), direction with length = 1
    P unit() const {
        double len = length();
        return {x / len, y / len};
    }

    // Projection of this onto vector b
    // Formula: proj_b(this) = b * [(this • b) / |b|²]
    P project_onto(const P &b) const {
        int dot = (*this & b);
        int len2 = b.length2();
        return b * (dot / (double)len2);
    }

    // Distance from point to line through points a and b
    // Formula: |(b - a) × (this - a)| / |b - a|
    // Geometric meaning: height from point to line
    double dist_to_line(const P &a, const P &b) const {
        P ab = b - a;
        P ap = *this - a;
        return abs((ab * ap)) / ab.length();
    }

    // Point equality: returns true if (x, y) == (b.x, b.y)
    bool operator==(const P &b) const {
        return x == b.x && y == b.y;
    }

    // Lexicographical comparison for sorting (by x, then y)
    bool operator<(const P &b) const {
        return tie(x, y) < tie(b.x, b.y);
    }
};
