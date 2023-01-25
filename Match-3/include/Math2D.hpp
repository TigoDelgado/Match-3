#ifndef __Match3_Math_2D_h__
#define __Match3_Math_2D_h__

struct Vector2f
{
    float x, y;
    Vector2f() : x(0.0f), y(0.0f) {}
    Vector2f(float p_x, float p_y) : x(p_x), y(p_y) {}

    Vector2f operator * (float f)
    {
        return Vector2f {x * f, y * f};
    }

    Vector2f operator * (Vector2f v)
    {
        return Vector2f {x * v.x, y * v.y};
    }
};

struct Coordinates
{
    int x, y;
    Coordinates() : x(0), y(0) {}
    Coordinates(int p_x, int p_y) : x(p_x), y(p_y) {}
};

#endif //__Match3_Math_2D_h__