struct Point
{
    int x, y;
    void input()
    {
        cin >> x >> y;
    }
    P operator-(const P &b) const
    {
        return {x - b.x, y - b.y};
    }
    void operator-=(const P &b)
    {
        x -= b.x;
        y -= b.y;
    }
    int operator*(const P &b) const
    {
        return x * b.y - y * b.x;
    }
    int triangle(const P &b, const P &c) const
    {
        return (b - *this) * (c - *this);
    }
};
